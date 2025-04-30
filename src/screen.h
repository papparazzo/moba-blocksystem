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

#include "common.h"
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <string>

class Screen final {
public:
    Screen();

    ~Screen();

    void setBlocks(const BlockNodeMapPtr &blocks);
    void init();
    void draw();
    void printException(const std::string &txt);
    void printStatus(const std::string &txt);
    
private:
    std::map<int, bool> nodesFound;
    BlockNodeMapPtr     blocks;
    bool                digit{false};
    
    struct Chain {
        bool isOpen{false};
        std::list<NodePtr> list;    
    };
    
    std::vector<Chain> chainList;

    void buildChain(NodePtr origin);
    void addChainLink(NodePtr origin, NodePtr next, Chain &chain);    
};
