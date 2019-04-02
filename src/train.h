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

#include <memory>

#include "common.h"

class Train {
public:
    Train(int address, int speed, DrivingDirection virt, DrivingDirection real);
    virtual ~Train();

    void switchVirtualDirection();
    void switchRealDirection();

    int getSpeed();
    int getAddress();

protected:
    int address;
    int speed;
    bool hasStopped;

    DrivingDirection realDirection;
	DrivingDirection virtualDirection;
};

using TrainPtr = std::shared_ptr<Train>;

