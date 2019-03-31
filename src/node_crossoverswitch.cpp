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

#include "node_crossoverswitch.h"

CrossOverSwitch::CrossOverSwitch(CrossOverSwitch::SwitchState state) : currentState{state} {
}

CrossOverSwitch::~CrossOverSwitch() {
}

void CrossOverSwitch::setInNodeDiagonal(NodePtr node) {
    inDiagonal = node;
}

void CrossOverSwitch::setInNodeVertical(NodePtr node) {
    inVertical = node;
}

void CrossOverSwitch::setOutNodeDiagonal(NodePtr node) {
    outDiagonal = node;
}

void CrossOverSwitch::setOutNodeVertical(NodePtr node) {
    outVertical = node;
}

bool CrossOverSwitch::turnSwitch(SwitchState state) {
    if(state == currentState) {
        return false;
    }
    currentState = state;
    return true;
}

CrossOverSwitch::SwitchState CrossOverSwitch::turnSwitch() {
    switch(currentState) {
        case BEND_1:
            currentState = CrossOverSwitch::STRAIGHT_1;
            break;

        case STRAIGHT_1:
            currentState = CrossOverSwitch::BEND_2;
            break;

        case BEND_2:
            currentState = CrossOverSwitch::STRAIGHT_2;
            break;

        case STRAIGHT_2:
            currentState = CrossOverSwitch::BEND_1;
            break;
    }
    return currentState;
}

NodePtr CrossOverSwitch::getJunctionNode(NodePtr node) const {
    if(node != inDiagonal && node != inVertical && node != outDiagonal && node != outVertical) {
        throw NodeException("invalid node given!");
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

NodePtr CrossOverSwitch::getInNode() const {
    switch(currentState) {
        case BEND_1:
        case BEND_2:
            return inDiagonal;

        case STRAIGHT_1:
        case STRAIGHT_2:
            return inVertical;
    }
    throw NodeException("invalid state given!");
}

NodePtr CrossOverSwitch::getOutNode() const {
    switch(currentState) {
        case BEND_1:
        case STRAIGHT_1:
            return outDiagonal;

        case BEND_2:
        case STRAIGHT_2:
            return outVertical;
    }
    throw NodeException("invalid state given!");
}
