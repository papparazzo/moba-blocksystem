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
#include "node_block_ext.h"
#include "node_crossoverswitch.h"
#include "node_threewayswitch.h"
#include "node_simpleswitch.h"
#include "moba/shared.h"
#include <cassert>

void LayoutParser::fetchBlockNodes(Direction curDir, Position curPos) {
    auto startDir = curDir;
    auto startPos = curPos;

    //std::cout << std::endl << "start fetch routine" << std::endl;
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
            //std::cout << "Bumper connected. Leaving..." << std::endl << std::endl;
            return;
        }

        auto block = blockContacts->find(curPos);
        auto iters = switchstates->find(curPos);

        /*
        std::cout <<
            curPos << " -> " <<
            (block == blockContacts->end() ? "" : " B ") <<
            (iters == switchstates->end() ? "" : " S ") << std::endl;
         */

        // Prüfen, ob Symbol keine Weiche und kein Block ist
        if(block == blockContacts->end() && iters == switchstates->end()) {
            // Wenn Symbol gerades Gleis oder Kreuzung dann einfach weiter in Richtung
            if(curSymbol->symbol.isBend()) {
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
            curSymbol->symbol.removeJunction(curDir);
            //std::cout << "Node connected. Leaving..." << std::endl << std::endl;
            return;
        }

        auto &curNode = nodes[curPos];
        NodePtr newNode;
        Symbol sym;

        if(block != blockContacts->end()) {
            // ...aktueller Knoten ist ein Block
            auto bNode = createBlock(curSymbol->id, block->second);
            sym = curSymbol->symbol;
            sym.reset();
            (*blockNodeMap)[block->second->blockContact] = bNode;
            newNode = bNode;
        } else {
            // ...aktueller Knoten ist eine Weiche
            assert(iters != switchstates->end() && "no switch state set");

            if(curSymbol->symbol.isLeftSwitch()) {
                sym = Symbol{Symbol::LEFT_SWITCH};
                newNode = std::make_shared<SimpleSwitch>(curSymbol->id, iters->second.switchStand);
            } else if(curSymbol->symbol.isRightSwitch()) {
                sym = Symbol{Symbol::RIGHT_SWITCH};
                newNode = std::make_shared<SimpleSwitch>(curSymbol->id, iters->second.switchStand);
            } else if(curSymbol->symbol.isCrossOverSwitch()) {
                sym = Symbol{Symbol::CROSS_OVER_SWITCH};
                newNode = std::make_shared<CrossOverSwitch>(curSymbol->id, iters->second.switchStand);
            } else if(curSymbol->symbol.isThreeWaySwitch()) {
                sym = Symbol{Symbol::THREE_WAY_SWITCH};
                newNode = std::make_shared<ThreeWaySwitch>(curSymbol->id, iters->second.switchStand);
            }
            (*switcheNodeMap)[iters->second.id] = newNode;
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

        while((dir = sym.getNextOpenJunction()) != Direction::UNSET) {
            sym.removeJunction(dir);
            if(dir + offset == compDir) {
                curNode.junctions[compDir](startNode.node);
                continue;
            }
            
            // Pruefen, ob Junction noch existiert. Bei eine Kehrschleife kann
            // diese durch Rekursion bereits geloescht worden sein
            if(curSymbol->symbol.removeJunction(dir + offset)) {
                fetchBlockNodes(dir + offset, curPos);
            }
        }
        //std::cout << "No open junctions. Leaving..." << std::endl << std::endl;
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

BlockExtPtr LayoutParser::createBlock(int id, BlockContactDataPtr contact) {
    // get train from block and set it
    auto iter = trainList->find(contact->trainId);
    auto block = std::make_shared<BlockExt>(id);

    if(iter != trainList->end()) {
        block->setTrain(iter->second);
    }

    return block;
}
