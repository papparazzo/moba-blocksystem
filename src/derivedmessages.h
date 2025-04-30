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

#include "moba/layoutmessages.h"
#include "moba/controlmessages.h"
#include "common.h"

struct LayoutGetLayoutsRes_Derived final : LayoutMessage {
    static constexpr std::uint32_t MESSAGE_ID = LAYOUT_GET_LAYOUT_RES;

    explicit LayoutGetLayoutsRes_Derived(const nlohmann::json &d) {
        symbols = std::make_shared<Container<SymbolPtr>>();
        for(auto &iter: d["symbols"]) {
            symbols->addItem(
                {
                    static_cast<std::size_t>(iter["xPos"].get<int>()),
                    static_cast<std::size_t>(iter["yPos"].get<int>())
                },
                std::make_shared<LayoutSymbol>(iter["id"].get<int>(), iter["symbol"].get<int>())
            );
        }
    }

    LayoutContainerPtr symbols{};
};

struct ControlGetBlockListRes final : ControlMessage {
    static constexpr std::uint32_t MESSAGE_ID = CONTROL_GET_BLOCK_LIST_RES;

    explicit ControlGetBlockListRes(const nlohmann::json &d) {
        blockContacts = std::make_shared<std::map<Position, BlockContactDataPtr>>();

        for(auto &iter : d) {
            (*blockContacts)[{
                static_cast<std::size_t>(iter["xPos"].get<int>()),
                static_cast<std::size_t>(iter["yPos"].get<int>())
            }] = std::make_shared<BlockContactData>(iter);
        }
    }

    BlockContactDataMapPtr blockContacts;
};

struct ControlGetSwitchStandListRes final : ControlMessage {
    static constexpr std::uint32_t MESSAGE_ID = CONTROL_GET_SWITCH_STAND_LIST_RES;
    explicit ControlGetSwitchStandListRes(const nlohmann::json &d) {
        switchstates = std::make_shared<std::map<Position, SwitchStandData>>();

        for(auto &iter : d) {
            (*switchstates)[{
                static_cast<std::size_t>(iter["xPos"].get<int>()),
                static_cast<std::size_t>(iter["yPos"].get<int>())
            }] = SwitchStandData{iter};
        }
    }

    SwitchStandMapPtr switchstates;
};

