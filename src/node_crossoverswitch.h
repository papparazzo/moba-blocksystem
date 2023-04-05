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

#include "node.h"
#include "moba/direction.h"

struct CrossOverSwitch: public Node {

    using Node::Node;

    virtual ~CrossOverSwitch() {
    }

    void setJunctionNode(Direction dir, NodePtr node) {
        switch(dir) {
            case Direction::TOP:
                outTop = node;
                return;

            case Direction::TOP_RIGHT:
                outRight = node;
                return;

            case Direction::BOTTOM:
                inBottom = node;
                return;

            case Direction::BOTTOM_LEFT:
                inLeft = node;
                return;
        }
        throw NodeException{"invalid direction given!"};
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(node != outTop && node != outRight && node != inBottom && node != inLeft) {
            throw NodeException{"invalid node given!"};
        }

        auto activeIn = getInNode();
        auto activeOut = getOutNode();

        if(node == activeIn) {
            return activeOut;
        }

        if(node == activeOut) {
            return activeIn;
        }

        return NodePtr{};
    }

    NodePtr getJunctionNode(Direction dir) const {
        switch(dir) {
            case Direction::TOP:
                return outTop;

            case Direction::TOP_RIGHT:
                return outRight;

            case Direction::BOTTOM:
                return inBottom;

            case Direction::BOTTOM_LEFT:
                return inLeft;
        }
        throw NodeException{"invalid direction given!"};
    }

    NodePtr getInNode() const {
        switch(currentState) {
            case SwitchStand::BEND_1:
            case SwitchStand::BEND_2:
                return outTop;

            case SwitchStand::STRAIGHT_1:
            case SwitchStand::STRAIGHT_2:
                return outRight;
        }
        throw NodeException{"invalid switch state given!"};
    }

    NodePtr getOutNode() const {
        switch(currentState) {
            case SwitchStand::BEND_1:
            case SwitchStand::STRAIGHT_1:
                return inBottom;

            case SwitchStand::BEND_2:
            case SwitchStand::STRAIGHT_2:
                return inLeft;
        }
        throw NodeException{"invalid switch state given!"};
    }

protected:
    NodePtr outTop;
    NodePtr outRight;
    NodePtr inBottom;
    NodePtr inLeft;
};
