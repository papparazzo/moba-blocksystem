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

#include "node_block.h"

Block::Block(
    NodePtr in, ContactData contact, TrainPtr train, BlockDirection direction
) : in{in}, contact{contact}, train{train}, direction{direction} {
}

Block::Block(NodePtr in, ContactData contact) : in{in}, contact{contact} {
}

Block::~Block() {
}

NodePtr Block::getJunctionNode(NodePtr node) const {
    if(node == in) {
        return out;
    }
    if(node == out) {
        return in;
    }
    throw NodeException{"invalid node given!"};
}

BlockPtr Block::getNextBlock() const {
    /*
    NodePtr node;
    do {
        node = in->getJunctionNode(this);
        if(node instanceof Block) {
            return (Block)node;
        }

    } while(node == std::nullptr_t);
     * */
    return BlockPtr{};
}

bool Block::pushTrain() {
    return false;
}

void Block::moveTrain() {

}

void Block::setOutNode(NodePtr node) {
    out = node;
}