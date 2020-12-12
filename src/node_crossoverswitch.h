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

struct CrossOverSwitch : public BaseSwitch {
    virtual ~CrossOverSwitch() {
    }

    void setInNodeTop(NodePtr node) {
        inTop = node;
    }

    void setInNodeRight(NodePtr node) {
        inRight = node;
    }

    void setOutNodeTop(NodePtr node) {
        outTop = node;
    }

    void setOutNodeRight(NodePtr node) {
        outRight = node;
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
        if(node != inTop && node != inRight && node != outTop && node != outRight) {
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

    NodePtr getInNode() const {
        switch(currentState) {
            case SwitchState::BEND_1:
            case SwitchState::BEND_2:
                return inTop;

            case SwitchState::STRAIGHT_1:
            case SwitchState::STRAIGHT_2:
                return inRight;
        }
        throw NodeException{"invalid switch state given!"};
    }

    NodePtr getOutNode() const {
        switch(currentState) {
            case SwitchState::BEND_1:
            case SwitchState::STRAIGHT_1:
                return outTop;

            case SwitchState::BEND_2:
            case SwitchState::STRAIGHT_2:
                return outRight;
        }
        throw NodeException{"invalid switch state given!"};
    }

protected:
    NodePtr inTop;
    NodePtr inRight;
    NodePtr outTop;
    NodePtr outRight;
};
