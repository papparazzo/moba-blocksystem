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

#include <iostream>
#include <functional>
#include <memory>
#include <thread>

#include <moba-common/helper.h>
#include <moba-common/log.h>

#include "config.h"

#include "moba/socket.h"
#include "moba/endpoint.h"
#include "moba/layouthandler.h"
#include "msgloop.h"

#include <unistd.h>

namespace {
    moba::common::AppData appData = {
        PACKAGE_NAME,
        moba::common::Version(PACKAGE_VERSION),
        __DATE__,
        __TIME__,
        "::1",
        7000
    };
}

int main(int argc, char *argv[]) {
    moba::common::setCoreFileSizeToULimit();

    auto groups = Groups::CLIENT | Groups::LAYOUT | Groups::INTERFACE;
    auto socket = std::make_shared<Socket>(appData.host, appData.port);
    auto endpoint = std::make_shared<Endpoint>(socket, appData.appName, appData.version, groups);

    while(true) {
        try {
            endpoint->connect();
            endpoint->sendMsg(LayoutGetLayoutReq{1});



            exit(EXIT_SUCCESS);
        } catch(std::exception &e) {
            LOG(moba::common::LogLevel::NOTICE) << e.what() << std::endl;
            sleep(4);
        }
    }
}
