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

#include "monitor.h"

#include <iomanip>
#include <chrono>
#include <ctime>

#include <string>

#include <sstream>
#include <moba-common/loggerprefix.h>

void Monitor::setBlocks(const BlockNodeMapPtr &blocks) {
    for(auto const& [_, v]: *blocks) {
        nodesFound[v->getId()] = false;
    }

    this->blocks = blocks;
    init();
}

void Monitor::init() {
    /*
    if(!blocks) { FIXME: throw exceptin at this point
        throw
    }
     */

    digit = !digit;
    chainList.clear();

    for(auto const& [_, v]: *blocks) {
        auto id = v->getId();

        if(nodesFound[id] == digit) {
            continue;
        }
        buildChain(v);
    }
}

void Monitor::buildChain(NodePtr origin) {

    Chain chain;

    NodePtr next = origin->getJunctionNode(Direction::RIGHT);

    nodesFound[origin->getId()] = digit;
    chain.list.push_back(origin);
    addChainLink(origin, next, chain);

    chainList.push_back(std::move(chain));
}

void Monitor::addChainLink(NodePtr origin, NodePtr next, Chain &chain) {
    NodePtr start = origin;

    while(true) {
        if(!next) {
            if(chain.isOpen) {
                return;
            }
            // Hier noch in andere Richtung suchen
            chain.isOpen = true;
            next = start->getJunctionNode(Direction::LEFT);

            addChainLink(start, next, chain);
            return;
        }

        if(chain.isOpen) {
            chain.list.push_front(next);
        } else {
            chain.list.push_back(next);
        }
        nodesFound[next->getId()] = digit;

        auto tmp = next;
        next = next->getJunctionNode(origin);
        origin = tmp;

        if(next == start) {
            return;
        }
    }
}

void Monitor::draw() {
    int y = 2;
    int x = 1;

    for(auto const& v: chainList) {
        std::string s = std::to_string(y - 1) + std::string((v.isOpen ? ") [open]" : ") [closed]"));
        //screen.mvprintw(y * 3, 2, (v.isOpen ? "%2i) [open]" : "%2i) [closed]"), y - 1);
        screen.mvprintw(y * 3, 2, s);

        for(auto const& a: v.list) {
            auto block = std::dynamic_pointer_cast<BlockExt>(a);
            if(x != 1) {
                screen.mvprintw(y * 3, x * 12, "->");
            }
            if(block) {
                screen.mvprintw(y * 3, x * 12 + 2, std::string(" |B:") + std::to_string(a->getId()) + "| ");
            } else {
                screen.mvprintw(y * 3, x * 12 + 2, std::string(" |S:") + std::to_string(a->getId()) + "| ");
            }

            if(block && block->isBlocked()) {
                auto train = block->getTrain();

                if(train->direction.drivingDirection == moba::DrivingDirection::BACKWARD) {
                    screen.mvprintw(y * 3 + 1, x * 12 + 2, std::string(" |#") + std::to_string( train->address) + "| >>");
                } else {
                    screen.mvprintw(y * 3 + 1, x * 12, std::string("<< |#") + std::to_string( train->address) + "|");
                }
            }
            x++;
        }
        y++;
        x = 1;
    }
    screen.refresh();
}





















namespace {
    // TODO: Put this into moba-lib-common
    std::string getTimeStamp() {
        const auto now = std::chrono::system_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        const auto timer = std::chrono::system_clock::to_time_t(now);

        std::tm bt{};

        localtime_r(&timer, &bt);

        std::stringstream ss;
        ss <<
            std::put_time(&bt, "%Y-%m-%d %H:%M:%S.") << std::right <<
            std::setw(3) << std::setfill('0') << ms.count();
        return ss.str();
    }
}

void Monitor::appendAction(const moba::LogLevel level, const std::string &action) {

    std::stringstream ss;
    ss << level << action;

    actions.push_back(ss.str());
    screen.printBuffer(30, 2, actions);
}

void Monitor::printException(const std::string &where, const std::string &what) {
    screen.printException(where, what);
}

void Monitor::feedbackContactTriggered(const std::uint16_t module, const std::uint16_t contact, const std::uint16_t time, bool active) {
    std::stringstream ss;
    ss <<
        "Feedback module [" <<
        std::setw(4) << std::setfill('0') << module << ":" <<
        std::setw(4) << std::setfill('0') << contact << "] time " <<
        std::setw(5) << std::setfill(' ') << time << " ms " <<
        (active ? "[ on]" : "[off]");

  //  appendCanBusAction(ss.str());
}

/*
void Monitor::appendCanBusAction(const std::string &action) {
    std::string msg = getTimeStamp() + " " + action;
    canBusActions.push_back(msg.c_str());
    screen.printBuffer(5, 2, canBusActions);
}
*/

void Monitor::printStatus(SystemHardwareStateChanged::HardwareState status) {
    switch(status) {
        case SystemHardwareStateChanged::HardwareState::ERROR:
            return screen.printStatus("Keine Verbindung zur Hardware", true);

        case SystemHardwareStateChanged::HardwareState::AUTOMATIC:
            return screen.printStatus("automatisch", false);

        case SystemHardwareStateChanged::HardwareState::EMERGENCY_STOP:
            return screen.printStatus("Nohalt ausgelöst", true);

        case SystemHardwareStateChanged::HardwareState::MANUEL:
            return screen.printStatus("manuell", false);

        case SystemHardwareStateChanged::HardwareState::STANDBY:
            return screen.printStatus("Energiesparmodus", false);
    }
}
