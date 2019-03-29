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

#include "node_switch.h"

NodeSwitch::NodeSwitch(NodePtr in, SwitchState state) : in{in}, currentState{state} {
}

NodeSwitch::NodeSwitch(SwitchState state) : currentState{state} {
}

NodeSwitch::~NodeSwitch() {
}

void NodeSwitch::setInNode(NodePtr node) {
    in = node;
}

void NodeSwitch::setOutStraightNode(NodePtr node) {
    outStraight = node;
}

void NodeSwitch::setOutBendNode(NodePtr node) {
    outBend = node;
}

bool NodeSwitch::turnSwitch(NodeSwitch::SwitchState state) {
    if(state == currentState) {
        return false;
    }
    currentState = state;
    return true;
}

NodePtr NodeSwitch::getJunctionNode(NodePtr node) const {
    if(node == outStraight && currentState == SwitchState::STRAIGHT) {
        return in;
    }
    if(node == outBend && currentState == SwitchState::BEND) {
        return in;
    }
    if(node == in && currentState == SwitchState::BEND) {
        return outBend;
    }
    if(node == in && currentState == SwitchState::STRAIGHT) {
        return outStraight;
    }
    if(node == outStraight || node == outBend) {
        return NodePtr{};
    }
    throw NodeException("invalid node given!");
}

