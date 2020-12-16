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

#include <memory>

#include "node.h"
#include "driving_direction.h"
#include "moba/direction.h"

struct Block : public Node {
    Block() : blocked{false} {
    }

    virtual ~Block() {
    }

    void setJunctionNode(Direction dir, NodePtr node) {
        switch(dir) {
            case Direction::TOP:
            case Direction::TOP_RIGHT:
            case Direction::RIGHT:
            case Direction::BOTTOM_RIGHT:
                out = node;
                return;
            
            case Direction::BOTTOM:
            case Direction::BOTTOM_LEFT:
            case Direction::LEFT:
            case Direction::TOP_LEFT:
                in = node;
                return;
        }
        throw NodeException{"invalid direction given!"};
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(blocked) {
            return NodePtr{};
        }

        if(node == in) {
            return out;
        }
        if(node == out) {
            return in;
        }
        throw NodeException{"invalid node given!"};
    }

    void blockIfNextNotFree(NodePtr node) {
        NodePtr tmp;

        if(node == in) {
            tmp = out;
        }
        if(node == out) {
            tmp = in;
        }


    }

    NodePtr getNode() const {
        // Hier irgendwie mit direction arbeiten...

        if(direction.value == DrivingDirection::BACKWARD) {
            return out;
        }

        return in;
    }

protected:
    NodePtr in;
    NodePtr out;

    bool blocked;

    DrivingDirection direction;
};

