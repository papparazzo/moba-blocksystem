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

#include "node_threewayswitch.h"

ThreeWaySwitch::ThreeWaySwitch(NodePtr in, ThreeWaySwitch::SwitchState state) : in{in}, currentState{state} {
}

ThreeWaySwitch::~ThreeWaySwitch() {
}

void ThreeWaySwitch::setOutStraight(NodePtr node) {
    outStraight = node;
}

void ThreeWaySwitch::setOutBendMinor(NodePtr node) {
    outBendMinor = node;
}

void ThreeWaySwitch::setOutBendMajor(NodePtr node) {
    outBendMajor = node;
}

bool ThreeWaySwitch::turnSwitch(SwitchState state) {
    if(state == currentState) {
        return false;
    }
    currentState = state;
    return true;
}

ThreeWaySwitch::SwitchState ThreeWaySwitch::turnSwitch() {
    switch(currentState) {
        case BEND_MINOR:
            currentState = ThreeWaySwitch::STRAIGHT_1;
            break;

        case STRAIGHT_1:
            currentState = ThreeWaySwitch::BEND_MAJOR;
            break;

        case BEND_MAJOR:
            currentState = ThreeWaySwitch::STRAIGHT_2;
            break;

        case STRAIGHT_2:
            currentState = ThreeWaySwitch::BEND_MINOR;
            break;
    }
    return currentState;
}

NodePtr ThreeWaySwitch::getJunctionNode(NodePtr node) const {
    if(node != in && node != outStraight && node != outBendMinor && node != outBendMajor) {
        throw NodeException("invalid node given!");
    }

    if(node == in && currentState == ThreeWaySwitch::BEND_MAJOR) {
        return outBendMajor;
    }

    if(node == in && currentState == ThreeWaySwitch::BEND_MINOR) {
        return outBendMinor;
    }

    if(node == in) {
        return outStraight;
    }

    if(node == outBendMajor && currentState == ThreeWaySwitch::BEND_MAJOR) {
        return in;
    }

    if(node == outBendMinor && currentState == ThreeWaySwitch::BEND_MINOR) {
        return in;
    }

    if(node == outStraight && currentState == ThreeWaySwitch::STRAIGHT_1) {
        return in;
    }

    if(node == outStraight && currentState == ThreeWaySwitch::STRAIGHT_2) {
        return in;
    }

    return NodePtr{};
}

