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

void LayoutParser::fetchBlockNodes(Direction dir, Position pos, std::function<void(const NodePtr&)> fn) {

    // So lange durchlaufen bis entweder Prellbock oder Weiche gefunden worden ist.
    while(true) {
        pos.setNewPosition(dir);

        auto currSymbol = layout->get(pos);
        currSymbol->removeJunction(getComplementaryDirection(dir));

        if(currSymbol->isEnd()) {
            if(this->lastBlock == null) {
                throw LayoutParserException{"Termination without block"};
            }
            //this->lastBlock->setOut({});
            return NodePtr{};
        }

        if(currSymbol->isBlockSymbol()) {
            this->lastBlock = std::make_shared<Block>(lastBlock);
        }

        if(currSymbol->isTrack()) {
            currSymbol->removeJunction
            dir = currSymbol->getNextOpenJunction();
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

    auto dir = currSymbol->getNextOpenJunction();

    //this->lastBlock = std::make_shared<Block>();


    this->fetchBlockNodes();






    return tmpNode;
}









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

