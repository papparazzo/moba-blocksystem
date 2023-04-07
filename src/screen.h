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

class Screen {
public:
    Screen(const BlockNodeMapPtr &blocks);
    
    virtual ~Screen();

    void init();
    void buildChain(NodePtr node);
    void draw();
    
private:
    std::map<int, bool> nodesFound;
    BlockNodeMapPtr     blocks;
    bool                digit{true};
    
    struct Chain {
        bool isOpen;
        std::list<NodePtr> list;    
    };
    
    std::vector<Chain> chainList;
    
};


