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

#include "node.h"
#include "common.h"
#include "train.h"
#include "moba/interfacehandler.h"

class Block;

using BlockPtr = std::shared_ptr<Block>;

class Block : public Node {

    public:
        Block(NodePtr in, Contact contact, TrainPtr train, BlockDirection direction);

        Block(NodePtr in, Contact contact);

        virtual ~Block();

    private:
        NodePtr in;
        NodePtr out;

        Contact contact;
        TrainPtr train;

        BlockDirection direction;
        Direction currDirection;

        void setOutNode(NodePtr node);

        NodePtr getJunctionNode(NodePtr node) const;

        BlockPtr getNextBlock() const;

        bool pushTrain();

        void moveTrain();
};

