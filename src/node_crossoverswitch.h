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

    void setInNodeDiagonal(NodePtr node) {
        inDiagonal = node;
    }

    void setInNodeVertical(NodePtr node) {
        inVertical = node;
    }

    void setOutNodeDiagonal(NodePtr node) {
        outDiagonal = node;
    }

    void setOutNodeVertical(NodePtr node) {
        outVertical = node;
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
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(node != inDiagonal && node != inVertical && node != outDiagonal && node != outVertical) {
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
                return inDiagonal;

            case SwitchState::STRAIGHT_1:
            case SwitchState::STRAIGHT_2:
                return inVertical;
        }
    }

    NodePtr getOutNode() const {
        switch(currentState) {
            case SwitchState::BEND_1:
            case SwitchState::STRAIGHT_1:
                return outDiagonal;

            case SwitchState::BEND_2:
            case SwitchState::STRAIGHT_2:
                return outVertical;
        }
    }

protected:
    NodePtr inDiagonal;
    NodePtr inVertical;
    NodePtr outDiagonal;
    NodePtr outVertical;
};
