/*
 *  Project:    moba-svg
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
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

#include <memory>
#include <vector>
#include <exception>

#include "moba/symbol.h"
#include "moba/container.h"
#include "moba/position.h"
#include <moba-common/exception.h>

using LayoutContainer = std::shared_ptr<Container<std::shared_ptr<Symbol>>>;

enum class BlockDirection {
    IN_TO_OUT,
    OUT_TO_OUT,
    BOTH
};

class DrivingDirection {
public:
    enum DrivingDirectionEnum {
        FORWARD,
        BACKWARD
    };

    DrivingDirection(DrivingDirectionEnum val) : value{val} {
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

            default:
                throw moba::common::UnsupportedOperationException{"Not supported."};
        }
    }

protected:
    DrivingDirectionEnum value;


/*
    @Override
    public String toJsonString(boolean formated, int indent)
    throws JSONException, IOException {
        StringBuilder b = new StringBuilder();
        b.append('"');
        b.append(DrivingDirection.values()[value].toString());
        b.append('"');
        return b.toString();
    }
 * */
};

