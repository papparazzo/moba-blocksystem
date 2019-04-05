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

virtual LayoutParser::~LayoutParser() {

}

void LayoutParser::collectTrackPoints(Position pos, Direction dir) {
    std::vector<Position> posVector;

    // Aktuelle Position speichern (ist quasi der Startpunkt)
    posVector.push_back(pos);

    while(true) {
        // Nächsten Koordinaten der Richtung
        pos.setNewPosition(dir);
        auto currSymbol = layout->get(pos);

        // Verbindugspunkt entfernen (verhindert eine Endlosschleife)
        Direction compDir = getComplementaryDirection(dir);
        currSymbol->removeJunktion(compDir);

        // Wie viele Verbindungspunkte sind noch offen?
        switch(currSymbol->getOpenJunktionsCount()) {
            case 0: // Endgleis -> Funktion verlassen
                posVector.push_back(pos);

                this->node = std::make_shared<Node>();

                lines.push_back(posVector);
                return;

            case 1: {
               auto openDir = currSymbol->getNextOpenJunktion();
               switch(getDistanceType(openDir, compDir)) {
                   // Teil einer Weiche (kann z.B. bei einer Kehrschleife passieren)
                   case DistanceType::INVALID:
                        posVector.push_back(pos);
                        lines.push_back(std::move(posVector));
                        currSymbol->removeJunktion(openDir);
                        collectTrackPoints(pos, openDir);
                        return;

                   case DistanceType::BEND:
                        posVector.push_back(pos);
                        dir = openDir;
                   case DistanceType::STRAIGHT:
                        currSymbol->removeJunktion(dir);
                        continue; // einfaches Gleis -> weitermachen
                }
            }

            case 2:
            case 3: {
                if(currSymbol->isCrossOver() || currSymbol->isCrossOverSwitch()) {
                    pointsOfInterest.push_back(getNextBendPosition(pos, currSymbol->getNextOpenJunktion()));
                } else {
                    pointsOfInterest.push_back(pos);
                }
                if(currSymbol->isOpenJunctionSet(dir)) {
                    currSymbol->removeJunktion(dir);
                    continue; // einfaches Gleis -> weitermachen
                }
                posVector.push_back(pos);
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

Position LayoutParser::getRealStartPosition() {
    Position pos = layout->getNextBoundPosition({0, 0});

    auto currSymbol = layout->get(pos);

    if(!currSymbol->isStartSymbol()) {
        throw LayoutParserException("first symbol is not a start symbol");
    }

    return pos;
}

NodePtr LayoutParser::parse(LayoutContainer layout) {

    this->layout = layout;

    // Startpunkt ermitteln
    pointsOfInterest.push_back(getRealStartPosition());

    // So lange parsen, bis die Liste mit Startpunkten leer ist.
    while(!pointsOfInterest.empty()) {
        // Das erste Symbol der Liste holen und die Startrichtung ermitteln
        auto pos = pointsOfInterest.front();
        auto currSymbol = layout->get(pos);

        Direction dir = currSymbol->getNextOpenJunktion(Direction::TOP);

        // Falls das Symobl keine offenen Verbindungspunkte mehr aufweist dann
        // das Element aus der Liste entfernen und mit dem nächsten Symbol fortfahren
        if(dir == UNSET) {
            pointsOfInterest.pop_front();
            continue;
        }
        currSymbol->removeJunktion(dir);

        collectTrackPoints(pos, dir);
        if(currSymbol->hasOpenJunctionsLeft()) {
            pointsOfInterest.push_back(pos);
        }
        pointsOfInterest.pop_front();
    }
    return node;
}

Position LayoutParser::getNextBendPosition(Position pos, Direction dir) {
    std::shared_ptr<Symbol> currSymbol;
    do {
        // Nächstes Symobl der aus Richtung "dir" ermitteln
        pos.setNewPosition(dir);
        currSymbol = layout->get(pos);
    } while(currSymbol->isOpenJunctionSet(dir));
    return pos;
}

