/*
*  Project:    moba-connector
 *
 *  Copyright (C) 2022 Stefan Paproth <pappi-@gmx.de>
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
 *  along with this program. If not, see <https://www.gnu.org/licenses/agpl.txt>.
 *
 */

#pragma once

#include <boost/circular_buffer.hpp>
#include <moba-common/loggerprefix.h>
#include <moba-common/screen.h>

#include "moba/systemmessages.h"

#include "common.h"
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <string>

class Monitor final {
public:
    explicit Monitor(const bool debug): screen{"moba-blocksystem", debug} {}
    ~Monitor() = default;

    void setBlocks(const BlockNodeMapPtr &blocks);

    void init();
    void draw();
    void printException(const std::string &where, const std::string &what);
    void printStatus(const std::string &txt);

private:
    std::map<int, bool> nodesFound;
    BlockNodeMapPtr     blocks;
    bool                digit{false};

    struct Chain {
        /**
         * Gibt an, ob die Kette offen ist (z.B. falsch gestellte Weiche, Prellbock...) bei einer geschlossenen Kette
         * kann der Zug einmal im Kreis fahren...
         */
        bool isOpen{false};
        std::list<NodePtr> list;
    };

    std::vector<Chain> chainList;

    void buildChain(NodePtr origin);
    void addChainLink(NodePtr origin, NodePtr next, Chain &chain);


public:


    void appendAction(moba::LogLevel level, const std::string &action);



    void feedbackContactTriggered(std::uint16_t module, std::uint16_t contact, std::uint16_t time, bool active);

    void printStatus(SystemHardwareStateChanged::HardwareState status);

private:
    boost::circular_buffer<std::string> actions{20};
    boost::circular_buffer<std::string> canBusActions{20};
    moba::Screen screen;

};

using MonitorPtr = std::shared_ptr<Monitor>;
