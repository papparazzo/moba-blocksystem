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

#pragma once

#include <vector>
#include <list>
#include <exception>

#include "node.h"
#include "moba/position.h"
#include "moba/container.h"
#include "moba/symbol.h"
#include "moba/direction.h"
#include "common.h"

class LayoutParserException : public std::exception {
    public:
        explicit LayoutParserException(const std::string &err) throw() : what_{err} {
        }

        LayoutParserException() throw() : what_{"Unknown error"} {
        }

        virtual ~LayoutParserException() throw() {
        }

        virtual const char *what() const throw() {
            return this->what_.c_str();
        }

    private:
        std::string what_;
};

class LayoutParser {

    /**
     * Der Startknoten
     */
    NodePtr node;

    /**
     * Der zu parsende Gleisplan
     */
    LayoutContainer layout;

    /**
     * Enthält sämtliche Weichen und Kreuzungen. Also Startpunkte von denen man aus
     * den Gleisplan parsen kann.
     */
    std::list<Position> pointsOfInterest;

    /**
     * Parst den Gleisplan bis zu einem Endgleis. Alle Positionen
     * von gebogenen Gleisen weden in einem Vektor gespeichert. Bei einem Endgleis wird
     * die Funktion beendet, bei einer Weiche wird beim komplimentären Punkt ganz normal
     * weiter gemacht und die Position im Vector pointsOfInterest gespeichert.
     *
     * @param pos Aktuelle Position beim Parsen
     * @param dir Richtung in die weiter geparst werden soll
     */
    void collectTrackPoints(Position pos, Direction dir);

    /**
     * Hangelt sich am Gleisplan entlang bis kein gerades Gleis mehr gefunden wurde,
     * Bzw. bis es kein Symbol mehr gibt, welches in die Richtung "dir" weiterführt
     *
     * @param pos Startposition
     * @param dir Richtung in der gesucht werden soll
     * @return Position des Symbols welches kein gerades Gleis mehr darstellt
     */
    Position getNextBendPosition(Position pos, Direction dir);

    /**
     * Ermittelt einen definierten Startpunkt. Beginnt oben links und gibt Position
     * des ersten Symbols zurück, welches entweder Weiche oder ein Endstück ist.
     *
     * @return
     */
    Position getRealStartPosition();

public:
    virtual ~LayoutParser();

    /**
     * Parst einen gesamten Gleisplan und liefert einen Baum zurück
     *
     * @param layout zwei-dimensionales Array mit dem Gleisplan
     * @return NodePtr
     */
    NodePtr parse(LayoutContainer layout);
};



