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

struct BaseSwitch : public Node {
    enum class SwitchState {
        BEND_1,
        STRAIGHT_1,
        BEND_2,
        STRAIGHT_2,
    };

    virtual void setSwitch(SwitchState state) {
        currentState = state;
    }

    virtual SwitchState turnSwitch() = 0;

    virtual ~BaseSwitch() {};

protected:
    SwitchState currentState;
};