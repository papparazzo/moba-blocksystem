/*
 *  Project:    moba-blocksystem
 *
 *  Copyright (C) 2023 Stefan Paproth <pappi-@gmx.de>
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

#include "moba/node.h"
#include "moba/node_block.h"
#include "moba/direction.h"
#include "moba/train.h"

struct BlockExt;

using BlockExtPtr = std::shared_ptr<BlockExt>;

struct BlockExt: public Block, std::enable_shared_from_this<Node> {

    using Block::Block;
    
    BlockExt(unsigned int id, TrainPtr train): Block{id}, train{train} {
    }

    virtual ~BlockExt() noexcept = default;

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

    BlockExtPtr pushTrain() {
        if(!isBlocked()) {
            throw NodeException{"block not blocked!"};
        }

        BlockExtPtr nextBlock;

        if(train->direction.drivingDirection == moba::DrivingDirection::FORWARD) {
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

    TrainPtr train;

    BlockExtPtr getNextBlock(NodePtr nextNode) {
        if(!nextNode) {
            return BlockExtPtr{};
        }

        auto curNode = shared_from_this();

        while(auto afterNextNode = nextNode->getJunctionNode(curNode)) {
            auto nextBlock = std::dynamic_pointer_cast<BlockExt>(nextNode);
            if(nextBlock) {
                if(nextBlock->isBlocked() ) {
                    return BlockExtPtr{};
                }
                
                if(!train) {
                    return nextBlock;
                }
                if(nextBlock->isOut(curNode)) {
                    train->direction.drivingDirection = moba::DrivingDirection::BACKWARD;
                } else {
                    train->direction.drivingDirection = moba::DrivingDirection::FORWARD;
                }
                return nextBlock;
            }
            curNode = nextNode;
            nextNode = afterNextNode;
        }
        return BlockExtPtr{};
    }
};
