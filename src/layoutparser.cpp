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

        auto currSymbol = layout->get(curPos);
        auto compDir = getComplementaryDirection(curDir);
        currSymbol->removeJunction(compDir);

        if(currSymbol->isEnd()) {
            if(lastNodeWasBlock == false) {
                throw LayoutParserException{"Termination without block"};
            }
            nodes[startPos].junctions[startDir](NodePtr{});
            return;
        }

        curDir = currSymbol->getNextOpenJunction();
        if(curDir == Direction::UNSET) {
            return;
        }

        currSymbol->removeJunction(curDir);

        if(!currSymbol->isBlockSymbol() && !currSymbol->isSwitch()) {
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

        if(currSymbol->isBlockSymbol()) {
            auto block = std::make_shared<Block>();
            curNode.node = block;
            curNode.junctions[curDir] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};
            block->setInNode(startNode.node);
            lastNodeWasBlock = true;

        } else if(currSymbol->isLeftSwitch()) {
            auto simpleSwitch = std::make_shared<SimpleSwitch>();
            curNode.node = simpleSwitch;
            /*
            if(/ *Weiche stumpf befahren? * /) {
                curNode.junctions[curDir] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutNode(nptr);};
            }else {
                curNode.junctions[curDir] = [simpleSwitch](const NodePtr &nptr) {block->setOutNode(nptr);};
            }
             */
            simpleSwitch->setInNode(startNode.node);
        } else if(currSymbol->isRightSwitch()) {

        } else if(currSymbol->isCrossOverSwitch()) {
            // Prüfen ob DKW einen Antrieb hat!
            auto crossOverSwitch = std::make_shared<CrossOverSwitch>();
            auto curNode = nodes[curPos];
            curNode.node = crossOverSwitch;
            curNode.junctions[curDir] = [crossOverSwitch](const NodePtr &nptr) {crossOverSwitch->setInNodeDiagonal(nptr);};
            crossOverSwitch->setInNodeDiagonal(startNode.node);
        } else if(currSymbol->isThreeWaySwitch()) {
            curNode.node = std::make_shared<ThreeWaySwitch>();
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
    auto dir1 = currSymbol->getNextOpenJunction();
    auto dir2 = currSymbol->getNextJunction();

    currSymbol->removeJunction(dir1);

    auto block = std::make_shared<Block>();

    auto tmp = nodes[pos];
    tmp.node = block;
    tmp.junctions[dir2] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};
    tmp.junctions[dir1] = [block](const NodePtr &nptr) {block->setInNode(nptr);};

    fetchBlockNodes(dir1, pos, true);

    if(!currSymbol->isJunctionSet(dir2)) {
        return;
    }

    fetchBlockNodes(dir2, pos, true);











    Position curPos;
    auto curNode = nodes[curPos];
    auto offset = currSymbol->getDistance(Symbol::getLeftSwitch());
Direction curDir;

   



    auto simpleSwitch = std::make_shared<SimpleSwitch>();

    curNode.node = simpleSwitch;
    curNode.junctions[curDir] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutStraightNode(nptr);};
    curNode.junctions[curDir] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setOutBendNode(nptr);};
    curNode.junctions[curDir] = [simpleSwitch](const NodePtr &nptr) {simpleSwitch->setInNode(nptr);};



    //Direction::TOP_LEFT
    //Direction::TOP
    //Direction::BOTTOM
    //Direction::TOP_RIGHT

    auto threeWaySwitch = std::make_shared<ThreeWaySwitch>();

    curNode.node = threeWaySwitch;
    curNode.junctions[curDir] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutBendLeft(nptr);};
    curNode.junctions[curDir] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutBendRight(nptr);};
    curNode.junctions[curDir] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setOutStraight(nptr);};
    curNode.junctions[curDir] = [threeWaySwitch](const NodePtr &nptr) {threeWaySwitch->setInNode(nptr);};


}






/*
void LayoutParser::collectTrackPoints(Position pos, Direction dir) {



    switch(getDistanceType(openDir, compDir)) {
        // Teil einer Weiche (kann z.B. bei einer Kehrschleife passieren)
        case DistanceType::INVALID:
             posVector.push_back(pos);
             lines.push_back(std::move(posVector));
             currSymbol->removeJunction(openDir);
             collectTrackPoints(pos, openDir);
             return;

        case DistanceType::BEND:
             posVector.push_back(pos);
             dir = openDir;
        case DistanceType::STRAIGHT:
             currSymbol->removeJunction(dir);
             continue; // einfaches Gleis -> weitermachen
     }


    while(true) {
        // Nächsten Koordinaten der Richtung
        pos.setNewPosition(dir);
        auto currSymbol = layout->get(pos);

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        Direction compDir = getComplementaryDirection(dir);
        currSymbol->removeJunction(compDir);

        // Wie viele Verbindungspunkte sind noch offen?
        switch(currSymbol->getOpenJunctionsCount()) {
            case 1: {
               auto openDir = currSymbol->getNextOpenJunction();
               switch(getDistanceType(openDir, compDir)) {
                   // Teil einer Weiche (kann z.B. bei einer Kehrschleife passieren)
                   case DistanceType::INVALID:

                        currSymbol->removeJunction(openDir);
                        collectTrackPoints(pos, openDir);
                        return;

                   case DistanceType::BEND:
                        dir = openDir;
                   case DistanceType::STRAIGHT:
                        currSymbol->removeJunction(dir);
                        continue; // einfaches Gleis -> weitermachen
                }
            }

            case 2:
            case 3: {
                if(currSymbol->isCrossOver() || currSymbol->isCrossOverSwitch()) {
                    //pointsOfInterest.push_back(getNextBendPosition(pos, currSymbol->getNextOpenJunktion()));
                } else {
                    pointsOfInterest.push_back(pos);
                }
                if(currSymbol->isOpenJunctionSet(dir)) {
                    currSymbol->removeJunction(dir);
                    continue; // einfaches Gleis -> weitermachen
                }
                auto ndir = getNextLeftDirection(dir);
                if(currSymbol->isOpenJunctionSet(ndir)) {
                    dir = ndir;
                    continue;
                }
                ndir = getNextRightDirection(dir);
                if(currSymbol->isOpenJunctionSet(ndir)) {
                    dir = ndir;
                    continue;
                }
                throw LayoutParserException("invalid");
            }

            default:
                throw LayoutParserException("invalid case");
        }
    }
}

*/