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
#include <cassert>

#include "node.h"
#include "moba/driving_direction.h"
#include "moba/direction.h"
#include "moba/train.h"

struct Block;

using BlockPtr = std::shared_ptr<Block>;

struct Block: public Node, std::enable_shared_from_this<Node> {

    Block(unsigned int id): Node{id} {
    }

    Block(unsigned int id, TrainPtr train): Node{id}, train{train} {
    }

    virtual ~Block() {
    }

    void setJunctionNode(Direction dir, NodePtr node) {
        switch(dir) {
            case Direction::TOP:
            case Direction::TOP_RIGHT:
            case Direction::RIGHT:
            case Direction::BOTTOM_RIGHT:
                out = node;
                return;

            case Direction::BOTTOM:
            case Direction::BOTTOM_LEFT:
            case Direction::LEFT:
            case Direction::TOP_LEFT:
                in = node;
                return;
        }
        throw NodeException{"invalid direction given!"};
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(train) {
            return NodePtr{};
        }

        if(node == in) {
            return out;
        }
        if(node == out) {
            return in;
        }
        throw NodeException{"invalid node given!"};
    }

    BlockPtr getNextBlockInDirection() {
        return getNextBlock(in);
    }

    BlockPtr getNextBlockOutDirection() {
        return getNextBlock(out);
    }

    bool isIn(NodePtr b) {
        return b == in;
    }

    bool isBlocked() const {
        return (bool)train;
    }

    TrainPtr getTrain() const {
        return train;
    }

    void setTrain(TrainPtr train) {
        this->train = train;
    }

    void pushTrain() {
        if(!isBlocked()) {
            throw NodeException{"block not blocked!"};
        }

        BlockPtr nextBlock;

        if(train->direction.value == DrivingDirection::BACKWARD) {
            nextBlock = getNextBlock(in);
        }
        nextBlock = getNextBlock(out);

        if(!nextBlock) {
            return;
        }

        nextBlock->train = train;
        train = TrainPtr();
    }

protected:
    NodePtr in;
    NodePtr out;

    TrainPtr train;

    BlockPtr getNextBlock(NodePtr b) {
        if(!b) {
            return BlockPtr{};
        }

        auto a = shared_from_this();

        while(auto c = b->getJunctionNode(a)) {
            auto derived = std::dynamic_pointer_cast<Block>(b);
            if(derived) {
                if(!derived->isIn(a)) {
                    this->train->switchDirection();
                }
                return derived;
            }
            a = b;
            b = c;
        }
        return BlockPtr{};
    }
};

