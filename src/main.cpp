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

#include <memory>

#include <moba-common/helper.h>

#include "config.h"

#include "moba/socket.h"
#include "moba/endpoint.h"
#include "moba/layoutmessages.h"
#include "msgloop.h"

#include <unistd.h>

namespace {
    moba::AppData appData = {
        PACKAGE_NAME,
        moba::Version{PACKAGE_VERSION},
        __DATE__,
        __TIME__,
        "::1",
        7000
    };
}

int main(int argc, char *argv[]) {
    auto socket = std::make_shared<Socket>(appData.host, appData.port);
    auto endpoint = EndpointPtr{new Endpoint{socket, appData.appName, appData.version, {Message::CLIENT, Message::LAYOUT, Message::INTERFACE}}};

    MessageLoop loop(endpoint);
    loop.run();
}
