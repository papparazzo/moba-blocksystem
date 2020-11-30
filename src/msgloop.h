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

#include <boost/noncopyable.hpp>

#include "moba/layouthandler.h"
#include "moba/interfacemessage.h"
#include "moba/endpoint.h"
#include <memory>
#include "common.h"

class MessageLoop : private boost::noncopyable {

    struct GetLayout : public LayoutMessage {
        static constexpr std::uint32_t MESSAGE_ID = LAYOUT_GET_LAYOUT_RES;

        GetLayout(const rapidjson::Document &d) {
            symbols = std::make_shared<Container<std::shared_ptr<Symbol>>>();
            for(auto &iter : d["symbols"].GetArray()) {
                symbols->addItem(
                    {
                        static_cast<std::size_t>(iter["xPos"].GetInt()),
                        static_cast<std::size_t>(iter["yPos"].GetInt())
                    },
                    std::make_shared<Symbol>(iter["symbol"].GetInt())
                );
            }
        }

        LayoutContainerPtr symbols;
    };

    EndpointPtr endpoint;
    bool closing;

    void parseLayout(const GetLayout &d);
    void contactTriggered(const InterfaceContactTriggered &d);

public:
    MessageLoop(EndpointPtr endpoint);
    void run();
};
