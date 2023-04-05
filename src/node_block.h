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
        if(node == in) {
            return out;
        }
        if(node == out) {
            return in;
        }
        throw NodeException{"invalid node given!"};
    }

    NodePtr getJunctionNode(Direction dir) const {
        switch(dir) {
            case Direction::TOP:
            case Direction::TOP_RIGHT:
            case Direction::RIGHT:
            case Direction::BOTTOM_RIGHT:
                return out;

            case Direction::BOTTOM:
            case Direction::BOTTOM_LEFT:
            case Direction::LEFT:
            case Direction::TOP_LEFT:
                return in;
        }
        throw NodeException{"invalid direction given!"};
    }
    
    bool isOut(NodePtr b) const {
        return b == out;
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

    BlockPtr pushTrain() {
        if(!isBlocked()) {
            throw NodeException{"block not blocked!"};
        }

        BlockPtr nextBlock;

        if(train->direction.drivingDirection == DrivingDirection::FORWARD) {
            nextBlock = getNextBlock(out);
        } else {
            nextBlock = getNextBlock(in);
        }

        if(nextBlock) {
            nextBlock->setTrain(train);
            train = TrainPtr();
        }
        return nextBlock;
    }

protected:
    NodePtr in;
    NodePtr out;

    TrainPtr train;

    BlockPtr getNextBlock(NodePtr nextNode) {
        if(!nextNode) {
            return BlockPtr{};
        }

        auto curNode = shared_from_this();

        while(auto afterNextNode = nextNode->getJunctionNode(curNode)) {
            auto nextBlock = std::dynamic_pointer_cast<Block>(nextNode);
            if(nextBlock) {
                if(nextBlock->isBlocked() ) {
                    return BlockPtr{};
                }
                
                if(!train) {
                    return nextBlock;
                }
                if(nextBlock->isOut(curNode)) {
                    train->direction.drivingDirection = DrivingDirection::BACKWARD;
                } else {
                    train->direction.drivingDirection = DrivingDirection::FORWARD;
                }
                return nextBlock;
            }
            curNode = nextNode;
            nextNode = afterNextNode;
        }
        return BlockPtr{};
    }
};

