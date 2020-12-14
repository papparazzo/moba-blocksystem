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

      /*  if(!currSymbol->isJunctionSet(dir2)) {
            return;
        }
*/


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

        curSymbol->removeJunction(curDir);

        //FIXME Prüfen ob DKW einen Antrieb hat!
        if(!curSymbol->isBlockSymbol() && !curSymbol->isSwitch() /* && !curSymbol->hasAntrieb*/) {
            continue;
        }

        auto startNode = nodes[startPos];

        auto iter = nodes.find(curPos);
        if(iter != nodes.end()) {
            startNode.junctions[startDir](iter->second.node);
            iter->second.junctions[compDir](startNode.node);
            return;
        }

        lastNodeWasBlock = false;
        auto curNode = nodes[curPos];

        if(curSymbol->isBlockSymbol()) {
            auto block = std::make_shared<Block>();
            curNode.node = block;
            curNode.junctions[curDir] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};
            block->setInNode(startNode.node);
            lastNodeWasBlock = true;
        } else if(curSymbol->isLeftSwitch()) {
            curDir = addLeftSwitch(curNode, curSymbol);




        } else if(curSymbol->isRightSwitch()) {
            curDir = addRightSwitch(curNode, curSymbol);
        } else if(curSymbol->isCrossOverSwitch()) {
            curDir = addCrossOverSwitch(curNode, curSymbol);
        } else if(curSymbol->isThreeWaySwitch()) {
            curDir = addThreeWaySwitch(curNode, curSymbol);
        }

        startNode.junctions[startDir](curNode.node);
        startPos = curPos;
        startDir = curDir;
    }
}

void LayoutParser::parse(LayoutContainerPtr layout) {

    this->layout = layout;

    Position pos = layout->getNextMatchPosition([](SymbolPtr symbol) {return symbol->isBlockSymbol();});

    auto currSymbol = layout->get(pos);
    auto dir1 = currSymbol->getNextJunction();
    auto dir2 = currSymbol->getNextJunction(dir1);

    currSymbol->removeJunction(dir1);

    auto block = std::make_shared<Block>();

    auto tmp = nodes[pos];
    tmp.node = block;
    tmp.junctions[dir1] = [block](const NodePtr &nptr) {block->setInNode(nptr);};
    tmp.junctions[dir2] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};

    fetchBlockNodes(dir1, pos, true);

    if(!currSymbol->isJunctionSet(dir2)) {
        return;
    }
    fetchBlockNodes(dir2, pos, true);
}

Direction LayoutParser::addLeftSwitch(NodeJunctions &curNode, SymbolPtr currSymbol) {
    auto offset = currSymbol->getDistance(Symbol{Symbol::LEFT_SWITCH});

    auto simpleSwitch = std::make_shared<SimpleSwitch>();

    curNode.node = simpleSwitch;
    curNode.junctions[Direction::TOP + offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutStraightNode(nptr);};
    curNode.junctions[Direction::TOP_LEFT + offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutBendNode(nptr);};
    curNode.junctions[Direction::BOTTOM + offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setInNode(nptr);};

    return Direction::TOP;
}

Direction LayoutParser::addRightSwitch(NodeJunctions &curNode, SymbolPtr currSymbol) {
    auto offset = currSymbol->getDistance(Symbol{Symbol::RIGHT_SWITCH});

    Direction curDir;

    auto simpleSwitch = std::make_shared<SimpleSwitch>();

    curNode.node = simpleSwitch;
    curNode.junctions[Direction::TOP + offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutStraightNode(nptr);};
    curNode.junctions[Direction::TOP_RIGHT+ offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutBendNode(nptr);};
    curNode.junctions[Direction::BOTTOM + offset] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setInNode(nptr);};
    return curDir;
}

Direction LayoutParser::addThreeWaySwitch(NodeJunctions &curNode, SymbolPtr currSymbol) {
    auto offset = currSymbol->getDistance(Symbol{Symbol::THREE_WAY_SWITCH});

    Direction curDir;

    auto threeWaySwitch = std::make_shared<ThreeWaySwitch>();

    curNode.node = threeWaySwitch;
    curNode.junctions[Direction::TOP_LEFT + offset] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutBendLeft(nptr);};
    curNode.junctions[Direction::TOP_RIGHT + offset] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutBendRight(nptr);};
    curNode.junctions[Direction::TOP + offset] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutStraight(nptr);};
    curNode.junctions[Direction::BOTTOM + offset] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setInNode(nptr);};
    return curDir;
}

Direction LayoutParser::addCrossOverSwitch(NodeJunctions &curNode, SymbolPtr currSymbol) {
    auto offset = currSymbol->getDistance(Symbol{Symbol::CROSS_OVER_SWITCH});

    Direction curDir;

    auto crossOverSwitch = std::make_shared<CrossOverSwitch>();

    curNode.node = crossOverSwitch;
    curNode.junctions[Direction::TOP + offset] = [crossOverSwitch](const NodePtr &nptr) {crossOverSwitch->setInNodeTop(nptr);};
    curNode.junctions[Direction::BOTTOM + offset] = [crossOverSwitch](const NodePtr &nptr) {crossOverSwitch->setOutNodeTop(nptr);};
    curNode.junctions[Direction::TOP_RIGHT + offset] = [crossOverSwitch](const NodePtr &nptr) {crossOverSwitch->setInNodeRight(nptr);};
    curNode.junctions[Direction::BOTTOM_LEFT + offset] = [crossOverSwitch](const NodePtr &nptr) {crossOverSwitch->setOutNodeRight(nptr);};
    return curDir;
}
