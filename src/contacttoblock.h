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

#include "moba/shared.h"
#include "node_block.h"
#include <map>

class ContactToBlock {
public:
    ContactToBlock();
    virtual ~ContactToBlock();

    /**
     * fügt einen Block zum Vektor hinzu
     * @param contact
     * @param block
     */
    void addBlock(ContactData contact, Block block);

    void freeBlock(ContactData contact);

    void blockBlock(ContactData contact);

private:
    //std::map<> vector;
};

