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

#include <thread>
#include <moba-common/log.h>

#include "msgloop.h"
#include "moba/registry.h"
#include "layoutparser.h"

MessageLoop::MessageLoop(EndpointPtr endpoint) : endpoint{endpoint} {
}

void MessageLoop::run() {
    while(!closing) {
        try {
            endpoint->connect();
            endpoint->sendMsg(LayoutGetLayoutReadOnlyReq{});

            Registry registry;
            registry.registerHandler<GetLayout>([this](const GetLayout &d){parseLayout(d);});
            registry.registerHandler<InterfaceContactTriggered>([this](const InterfaceContactTriggered &d){contactTriggered(d);});

            while(true) {
                registry.handleMsg(endpoint->waitForNewMsg());
            }
        } catch(const std::exception &e) {
            LOG(moba::common::LogLevel::ERROR) << "exception occured! <" << e.what() << ">" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
}

void MessageLoop::parseLayout(const MessageLoop::GetLayout &d) {
    LayoutParser parser;
    parser.parse(d.symbols);
}

void MessageLoop::contactTriggered(const InterfaceContactTriggered &d) {

}
