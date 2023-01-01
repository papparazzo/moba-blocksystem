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

#include <exception>
#include <string>
#include <memory>

#include "moba/direction.h"
#include "moba/switchstand.h"

class NodeException: public std::exception {

    std::string what_;

public:
    explicit NodeException(const std::string &err) throw(): what_{err} {
    }

    NodeException() throw(): what_{"Unknown error"} {
    }

    virtual ~NodeException() throw() {
    }

    virtual const char *what() const throw() {
        return this->what_.c_str();
    }
};

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Node {
    Node(unsigned int id, SwitchStand switchStand = SwitchStand::STRAIGHT_1): id{id}, currentState{switchStand} {
    }

    virtual ~Node() {};
    virtual NodePtr getJunctionNode(NodePtr node) const = 0;
    virtual void setJunctionNode(Direction dir, NodePtr node) = 0;

    void turn(SwitchStand stand) {
        currentState = stand;
    }

    unsigned int getId() const {
        return id;
    }

protected:
    unsigned int id;
    SwitchStand currentState;

};
