/*
 *  Project:    moba-blocksystem
 *
 *  Copyright (C) 2019 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include <iostream>
#include <memory>

#include "layoutparser.h"
#include "node_block.h"
#include "node_crossoverswitch.h"
#include "node_threewayswitch.h"
#include "node_simpleswitch.h"
#include "moba/shared.h"
#include <cassert>

void LayoutParser::fetchBlockNodes(Direction curDir, Position curPos) {
    auto startDir = curDir;
    auto startPos = curPos;

    while(true) {
        // Schritt weiter zum nächsten Symbol
        curPos.setNewPosition(curDir);

        // Symbol von der aktuellen Position im Gleisplan
        auto curSymbol = layout->get(curPos);
        auto compDir = curDir.getComplementaryDirection();

        assert((curSymbol) && "invalid layout: No symbol at current position");
        assert((curSymbol->symbol.isJunctionSet(compDir)) && "invalid layout: no open junctions");

        curSymbol->symbol.removeJunction(compDir);

        if(curSymbol->symbol.isEnd()) {
            return;
        }

        // Prüfen, ob Symbol keine Weiche und kein Block ist
        auto block = blockContacts->find(curPos);
        if(curSymbol->symbol.isTrack() && block == blockContacts->end()) {
            // FIXME: Prüfen, ob Doppelte Kreuzungsweiche ohne Antrieb
            if(!curSymbol->symbol.isCrossOver() /*|| curSymbol->symbol.isCrossOverSwitch()*/) {
                curDir = curSymbol->symbol.getNextOpenJunction(curDir);
            }
            continue;
        }

        // Ab hier entweder Weiche oder Block! Startknoten ermitteln...
        auto &startNode = nodes[startPos];

        // Existiert an aktueller Stelle schon ein Knoten?
        auto iter = nodes.find(curPos);
        if(iter != nodes.end()) {
            //... wenn ja, diesen mit dem Startknoten verbinden und Funktion verlassen
            startNode.junctions[startDir](iter->second.node);
            iter->second.junctions[compDir](startNode.node);
            return;
        }

        auto &curNode = nodes[curPos];
        NodePtr newNode;
        Symbol sym;

        // Knoten ist ein Blockknoten
        if(block != blockContacts->end()) {
            auto bNode = createBlock(curSymbol->id, block->second);
            sym = curSymbol->symbol;
            sym.reset();
            (*blockNodeMap)[block->second->blockContact] = bNode;
            newNode = bNode;
        }

        curNode.node = newNode;
        auto offset = curSymbol->symbol.getDistance(sym);

        startNode.junctions[startDir](curNode.node);

        Direction dir;

        while((dir = sym.getNextOpenJunction()) != Direction::UNSET) {
            curNode.junctions[dir + offset] = [newNode, dir](const NodePtr &nptr) {newNode->setJunctionNode(dir, nptr);};
            sym.removeJunction(dir);
        }

        sym.reset();

        dir = Direction::TOP_LEFT;

        while((dir = sym.getNextOpenJunction(dir)) != Direction::UNSET) {
            if(dir + offset == compDir) {
                sym.removeJunction(dir);
                curNode.junctions[compDir](startNode.node);
                break;
            }
        }

        dir = Direction::TOP_LEFT;

        while((dir = sym.getNextOpenJunction(dir)) != Direction::UNSET) {
            sym.removeJunction(dir + offset);
            fetchBlockNodes(dir + offset, curPos);
        }
        return;
    }
}

void LayoutParser::parse(LayoutContainerPtr layout, BlockContactDataMapPtr blockContacts, SwitchStandMapPtr switchstates, TrainListPtr trainList) {

    this->layout = layout;
    this->blockContacts = blockContacts;
    this->switchstates = switchstates;
    this->trainList = trainList;

    auto firstBlockContact = blockContacts->begin();
    if(firstBlockContact == blockContacts->end()) {
        throw LayoutParserException{"no blocks found"};
    }

    Position pos = firstBlockContact->first;

    auto curSymbol = layout->get(pos);
    auto dir1 = curSymbol->symbol.getNextJunction();
    auto dir2 = curSymbol->symbol.getNextJunction(dir1);
    auto firstBlockContactData = firstBlockContact->second;

    auto block = createBlock(curSymbol->id, firstBlockContactData);
    auto &tmp = nodes[pos];

    (*blockNodeMap)[firstBlockContactData->blockContact] = block;

    tmp.node = block;
    tmp.junctions[dir1] = [dir1, block](const NodePtr &nptr) {block->setJunctionNode(dir1, nptr);};
    tmp.junctions[dir2] = [dir2, block](const NodePtr &nptr) {block->setJunctionNode(dir2, nptr);};

    curSymbol->symbol.removeJunction(dir1);
    fetchBlockNodes(dir1, pos);

    if(!curSymbol->symbol.isJunctionSet(dir2)) {
        return;
    }

    curSymbol->symbol.removeJunction(dir2);
    fetchBlockNodes(dir2, pos);
}

BlockPtr LayoutParser::createBlock(int id, BlockContactDataPtr contact) {
    // get train from block and set it
    auto iter = trainList->find(contact->trainId);
    auto block = std::make_shared<Block>(id);

    if(iter != trainList->end()) {
        block->setTrain(iter->second);
    }

    return block;
}
