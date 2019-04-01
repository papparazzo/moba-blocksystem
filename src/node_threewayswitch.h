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

class ThreeWaySwitch;

using ThreeWaySwitchPtr = std::shared_ptr<ThreeWaySwitch>;

class ThreeWaySwitch : public Node {
public:
    enum SwitchState {
        BEND_MINOR,
        STRAIGHT_1,
        BEND_MAJOR,
        STRAIGHT_2,
    };

    ThreeWaySwitch(NodePtr in, SwitchState state);

    virtual ~ThreeWaySwitch();

    void setOutStraight(NodePtr node);

    void setOutBendMinor(NodePtr node);

    void setOutBendMajor(NodePtr node);

    bool turnSwitch(SwitchState state);

    SwitchState turnSwitch();

    NodePtr getJunctionNode(NodePtr node) const;

private:
    NodePtr in;
    NodePtr outStraight;
    NodePtr outBendMinor;
    NodePtr outBendMajor;

    SwitchState currentState;
};
