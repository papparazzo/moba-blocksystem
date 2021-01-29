/*
 *  Project:    moba-svg
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
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
#include <vector>
#include <map>

#include "moba/train.h"
#include "moba/symbol.h"
#include "moba/container.h"
#include "moba/position.h"
#include "moba/shared.h"
#include "node.h"
#include "node_block.h"

// In
struct LayoutSymbol {
    LayoutSymbol(int id, Symbol symbol): id{id}, symbol{symbol} {
    }
	int id;
	Symbol symbol;
};

using SymbolPtr = std::shared_ptr<LayoutSymbol>;
using LayoutContainerPtr = std::shared_ptr<Container<SymbolPtr>>;
using BlockContactDataPtr = std::shared_ptr<BlockContactData>;
using BlockContactDataMapPtr = std::shared_ptr<std::map<Position, BlockContactDataPtr>>;
using SwitchStandMapPtr = std::shared_ptr<std::map<Position, SwitchStandData>>;

// Out
using SwitchNodeMapPtr = std::shared_ptr<std::map<int, NodePtr>>;
using BlockNodeMapPtr = std::shared_ptr<std::map<ContactData, BlockPtr>>;