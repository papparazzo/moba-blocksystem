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

#include "node_base_switch.h"

struct ThreeWaySwitch : public BaseSwitch {

    virtual ~ThreeWaySwitch() {
    }

    void setInNode(NodePtr node) {
        in = node;
    }

    void setOutStraight(NodePtr node) {
        outStraight = node;
    }

    void setOutBendLeft(NodePtr node) {
        outBendLeft = node;
    }

    void setOutBendRight(NodePtr node) {
        outBendRight = node;
    }

    SwitchState turnSwitch() {
        switch(currentState) {
            case SwitchState::BEND_1:
                return currentState = SwitchState::STRAIGHT_1;

            case SwitchState::STRAIGHT_1:
                return currentState = SwitchState::BEND_2;

            case SwitchState::BEND_2:
                return currentState = SwitchState::STRAIGHT_2;

            case SwitchState::STRAIGHT_2:
                return currentState = SwitchState::BEND_1;
        }
        throw NodeException{"invalid switch state given!"};
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(node != in && node != outStraight && node != outBendLeft && node != outBendRight) {
            throw NodeException{"invalid node given!"};
        }

        if(node == in && currentState == SwitchState::BEND_2) {
            return outBendLeft;
        }

        if(node == in && currentState == SwitchState::BEND_1) {
            return outBendRight;
        }

        if(node == in) {
            return outStraight;
        }

        if(node == outBendLeft && currentState == SwitchState::BEND_2) {
            return in;
        }

        if(node == outBendRight && currentState == SwitchState::BEND_1) {
            return in;
        }

        if(node == outStraight && (currentState == SwitchState::STRAIGHT_1 || currentState == SwitchState::STRAIGHT_2)) {
            return in;
        }

        return NodePtr{};
    }

protected:
    NodePtr in;
    NodePtr outStraight;
    NodePtr outBendLeft;
    NodePtr outBendRight;
};
