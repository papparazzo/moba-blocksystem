/*
 *  Project:    moba-blocksystem
 *
 *  Copyright (C) 2020 Stefan Paproth <pappi-@gmx.de>
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

struct DrivingDirection {
    enum DrivingDirectionEnum {
        UNKNOWN,
        FORWARD,
        BACKWARD
    };

    DrivingDirection() : value{UNKNOWN} {
    }

    DrivingDirection(DrivingDirectionEnum val) : value{val} {
    }

    void toggle() {
        value = DrivingDirection::flip(value);
    }

    static DrivingDirectionEnum flip(DrivingDirection d) {
        return flip(d.value);
    }

    static DrivingDirectionEnum flip(DrivingDirectionEnum d) {
        switch(d) {
            case BACKWARD:
                return FORWARD;

            case FORWARD:
                return BACKWARD;

            case UNKNOWN:
                return UNKNOWN;

            default:
                throw moba::common::UnsupportedOperationException{"Not supported."};
        }
    }

    DrivingDirectionEnum value;
};

