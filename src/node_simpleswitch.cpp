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

#include "node_simpleswitch.h"

Switch::Switch(NodePtr in, SwitchState state) : in{in}, currentState{state} {
}

Switch::Switch(SwitchState state) : currentState{state} {
}

Switch::~Switch() {
}

void Switch::setInNode(NodePtr node) {
    in = node;
}

void Switch::setOutStraightNode(NodePtr node) {
    outStraight = node;
}

void Switch::setOutBendNode(NodePtr node) {
    outBend = node;
}

bool Switch::turnSwitch(Switch::SwitchState state) {
    if(state == currentState) {
        return false;
    }
    currentState = state;
    return true;
}

NodePtr Switch::getJunctionNode(NodePtr node) const {
    if(node != in && node != outStraight && node != outBend) {
        throw NodeException{"invalid node given!"};
    }
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
    return NodePtr{};
}

