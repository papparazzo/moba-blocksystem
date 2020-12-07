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
#include "common.h"
#include "node_block.h"
#include "node_crossoverswitch.h"
#include "node_threewayswitch.h"

void LayoutParser::fetchBlockNodes(Direction dir, Position pos, bool lastNodeWasBlock) {

    auto currDir = dir;
    auto currPos = pos;

    while(true) {
        pos.setNewPosition(dir);

        auto currSymbol = layout->get(pos);
        currSymbol->removeJunction(getComplementaryDirection(dir));

        if(currSymbol->isEnd()) {
            if(lastNodeWasBlock == false) {
                throw LayoutParserException{"Termination without block"};
            }
            nodes[currPos].junctions[currDir](NodePtr{});
            return;
        }

        if(currSymbol->isTrack()) {
            dir = currSymbol->getNextOpenJunction();
            currSymbol->removeJunction(dir);

            if(currSymbol->isBlockSymbol()) {

                auto iter = nodes.find(pos);

                auto tmp = nodes[pos];
                if(iter == nodes.end()) {
                    auto block = std::make_shared<Block>();
                }

                // FIXME: Prüfen, ob Block schon gesetzt...
                //if()
                tmp.node = block;

                auto tmp2 = nodes[currPos];
                tmp2.junctions[currDir](block);
                block->setInNode(tmp2.node);
                tmp.junctions[dir] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};

                currPos = pos;
                currDir = dir;
            }
            continue;
        }

        if(currSymbol->isSwitch()) {

        }
    }
}

void LayoutParser::handleSwitch(Position pos) {

    auto currSymbol = layout->get(pos);

    auto iter = nodes.find(pos);

    NodePtr node;

    if(iter == nodes.end()) {
        if(currSymbol->isCrossOverSwitch()) {
            node = std::make_shared<CrossOverSwitch>();;
        } else if(currSymbol->isSwitch()) {
            node = std::make_shared<Switch>();
        } else if(currSymbol->isThreeWaySwitch()) {
            node = std::make_shared<ThreeWaySwitch>();
        }
        nodes[pos] = node;
    } else {
        node = *iter;
    }




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
}

NodePtr LayoutParser::parse(LayoutContainerPtr layout) {

    this->layout = layout;

    Position pos = layout->getNextMatchPosition([](SymbolPtr symbol) {return symbol->isBlockSymbol();});

    auto currSymbol = layout->get(pos);

    auto dir1 = currSymbol->getNextOpenJunction();

    currSymbol->removeJunction(dir1);

    auto tmp = nodes[pos];

    auto block = std::make_shared<Block>();

    tmp.node = block;

    auto dir2 = currSymbol->getNextJunction();

    currSymbol->removeJunction(dir2);

    tmp.junctions[dir2] = [block](const NodePtr &nptr) {block->setOutNode(nptr);};

    tmp.junctions[dir1] = [block](const NodePtr &nptr) {block->setInNode(nptr);};

    fetchBlockNodes(dir1, pos, true);



    // FIXME: Prüfen, ob 2 Eingang nicht bereits schon gesetzt!
    fetchBlockNodes(dir2, pos, true);
}








/*
void LayoutParser::collectTrackPoints(Position pos, Direction dir) {

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