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

#include <map>
#include <exception>

#include "node.h"
#include "moba/position.h"
#include "moba/container.h"
#include "moba/symbol.h"
#include "moba/direction.h"
#include "moba/shared.h"
#include "common.h"
#include <functional>

class LayoutParserException: public std::exception {
    public:
        explicit LayoutParserException(const std::string &err) throw() : what_{err} {
        }

        LayoutParserException() throw() : what_{"Unknown error"} {
        }

        virtual ~LayoutParserException() throw() {
        }

        virtual const char *what() const throw() {
            return this->what_.c_str();
        }

    private:
        std::string what_;
};

class LayoutParser {

    using NodeCallback = std::function<void(const NodePtr&)>;

    // In
    LayoutContainerPtr layout;
    BlockContactDataMapPtr blockContacts;
    SwitchStandMapPtr switchstates;
    TrainListPtr trainList;

    // Out
    SwitchNodeMapPtr switcheNodeMap;
    BlockNodeMapPtr blockNodeMap;

    struct NodeJunctions {
        std::map<Direction, NodeCallback> junctions;
        NodePtr node;
    };

    std::map<Position, NodeJunctions> nodes;

    void fetchBlockNodes(Direction dir, Position pos);
    BlockPtr createBlock(int id, BlockContactDataPtr contact);

public:
    LayoutParser() {
        switcheNodeMap = std::make_shared<std::map<int, std::shared_ptr<Node>>>();
        blockNodeMap = std::make_shared<std::map<ContactData, std::shared_ptr<Block>>>();
    }

    virtual ~LayoutParser(){
    }

    void parse(LayoutContainerPtr layout, BlockContactDataMapPtr blockContacts, SwitchStandMapPtr switchstates, TrainListPtr trainList);

    SwitchNodeMapPtr getSwitchMap() const {
        return switcheNodeMap;
    }

    BlockNodeMapPtr getBlockMap() const {
        return blockNodeMap;
    }
};

