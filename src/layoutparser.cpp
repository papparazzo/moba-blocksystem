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

void LayoutParser::fetchBlockNodes(Direction curDir, Position curPos, bool lastNodeWasBlock) {

    auto startDir = curDir;
    auto startPos = curPos;

    while(true) {
        curPos.setNewPosition(curDir);

        auto curSymbol = layout->get(curPos);
        auto compDir = curDir.getComplementaryDirection();
        curSymbol->removeJunction(compDir);

        if(curSymbol->isEnd()) {
            if(lastNodeWasBlock == false) {
                throw LayoutParserException{"Termination without block"};
            }
            nodes[startPos].junctions[startDir](NodePtr{});
            return;
        }

        if(!curSymbol->isCrossOver() && !curSymbol->isCrossOverSwitch()) {
            curDir = curSymbol->getNextOpenJunction();
        }

        if(curDir == Direction::UNSET) {
            throw LayoutParserException{"no open junctions left"};
        }

        //FIXME Prüfen ob DKW einen Antrieb hat!
        if(!curSymbol->isBlockSymbol() && !curSymbol->isSwitch() /* && !curSymbol->hasAntrieb*/) {
            continue;
        }

        auto startNode = nodes[startPos];

        auto iter = nodes.find(curPos);
        if(iter != nodes.end()) {
            startNode.junctions[startDir](iter->second.node);
            iter->second.junctions[compDir](startNode.node);
            curSymbol->removeJunction(curDir);
            return;
        }

        lastNodeWasBlock = false;
        auto curNode = nodes[curPos];
        NodePtr newNode;
        Symbol sym;

        if(curSymbol->isBlockSymbol()) {
            newNode = std::make_shared<Block>();
            sym = *curSymbol;
            lastNodeWasBlock = true;
        } else if(curSymbol->isLeftSwitch()) {
            sym = Symbol{Symbol::LEFT_SWITCH};
            newNode = std::make_shared<SimpleSwitch>();
        } else if(curSymbol->isRightSwitch()) {
            sym = Symbol{Symbol::RIGHT_SWITCH};
            newNode = std::make_shared<SimpleSwitch>();
        } else if(curSymbol->isCrossOverSwitch()) {
            sym = Symbol{Symbol::CROSS_OVER_SWITCH};
            newNode = std::make_shared<CrossOverSwitch>();
        } else if(curSymbol->isThreeWaySwitch()) {
            sym = Symbol{Symbol::THREE_WAY_SWITCH};
            newNode = std::make_shared<ThreeWaySwitch>();
        }

        curNode.node = newNode;
        auto offset = curSymbol->getDistance(sym);
        startNode.junctions[startDir](curNode.node);

        Direction dir;

        while((dir = sym.getNextOpenJunction()) != Direction::UNSET) {
            curNode.junctions[dir + offset] = [newNode, dir](const NodePtr &nptr) {newNode->setJunctionNode(dir, nptr);};
            curSymbol->removeJunction(dir);
            if(dir == compDir) {
                curNode.junctions[compDir](startNode.node);
                continue;
            }
            if(dir == curDir) {
                continue;
            }
            fetchBlockNodes(dir, curPos, false);
        }

        startPos = curPos;
        startDir = curDir;
    }
}

void LayoutParser::parse(LayoutContainerPtr layout) {

    this->layout = layout;

    Position pos = layout->getNextMatchPosition([](SymbolPtr symbol) {return symbol->isBlockSymbol();});

    auto curSymbol = layout->get(pos);
    auto dir1 = curSymbol->getNextJunction();
    auto dir2 = curSymbol->getNextJunction(dir1);

    auto block = std::make_shared<Block>();

    auto tmp = nodes[pos];
    tmp.node = block;
    tmp.junctions[dir1] = [dir1, block](const NodePtr &nptr) {block->setJunctionNode(dir1, nptr);};
    tmp.junctions[dir2] = [dir2, block](const NodePtr &nptr) {block->setJunctionNode(dir2, nptr);};

    curSymbol->removeJunction(dir1);
    fetchBlockNodes(dir1, pos, true);

    if(!curSymbol->isJunctionSet(dir2)) {
        return;
    }

    curSymbol->removeJunction(dir2);
    fetchBlockNodes(dir2, pos, true);
}
