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

#include <memory>

#include "derivedmessages.h"
#include "moba/interfacemessages.h"
#include "moba/endpoint.h"
#include "moba/train.h"
#include "common.h"
#include "screen.h"

class MessageLoop {

    EndpointPtr endpoint;

    // in
    BlockContactDataMapPtr blockContacts;
    SwitchStandMapPtr switchstates;
    TrainListPtr trainList;

    // out
    BlockNodeMapPtr blockMap;
    SwitchNodeMapPtr switchMap;

    bool closing;

    Screen screen;

    void parseLayout(const LayoutGetLayoutsRes_Derived &d);
    void contactTriggered(const InterfaceContactTriggered &d);
    void getFeedbackContactList(const ControlGetContactListRes &d);
    void getSwitchStates(const ControlGetSwitchStandListRes &d);
    void getTrainList(const ControlGetTrainListRes &d);
    void updateScreen();

public:
    MessageLoop(EndpointPtr endpoint, bool enableScreen);

    MessageLoop(const MessageLoop&) = delete;
    MessageLoop& operator=(const MessageLoop&) = delete;

    void run();
};
