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

#include "screen.h"
#include <curses.h>
#include <iostream>
#include <unordered_set>

Screen::Screen() {
    initscr();
    curs_set(0);
    noecho();

    if(!has_colors()) {
        return;
    }

    start_color();
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
}

Screen::~Screen() {
    endwin();
}

void Screen::setBlocks(const BlockNodeMapPtr &blocks) {
    for(auto const& [_, v]: *blocks) {
        nodesFound[v->getId()] = false;
    }

    this->blocks = blocks;
    init();
}

void Screen::init() {
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

void Screen::buildChain(NodePtr origin) {

    Chain chain;

    NodePtr next = origin->getJunctionNode(Direction::RIGHT);
    
    nodesFound[origin->getId()] = digit;
    chain.list.push_back(origin);
    addChainLink(origin, next, chain);
       
    chainList.push_back(std::move(chain));
}

void Screen::addChainLink(NodePtr origin, NodePtr next, Chain &chain) {
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

void Screen::draw() {
    int y = 2;
    int x = 1;
    clear();
    for(auto const& v: chainList) {
        mvprintw(y * 3, 0, (v.isOpen ? "%2i) [open]" : "%2i) [closed]"), y);
        
        for(auto const& a: v.list) {
            auto block = std::dynamic_pointer_cast<BlockExt>(a);
            if(x != 1) {
                mvprintw(y * 3, x * 12 - 2, "->");
            } 
            if(block) {
                mvprintw(y * 3, x * 12, " |B:%4d| ", a->getId());
            } else {
                mvprintw(y * 3, x * 12, " |S:%4d| ", a->getId());
            }

            if(block && block->isBlocked()) {
                auto train = block->getTrain();
                if(train->direction.drivingDirection == moba::DrivingDirection::BACKWARD) {
                    mvprintw(y * 3 + 1, x * 12, " |#%5d| >>", train->address);
                } else {
                    mvprintw(y * 3 + 1, x * 12 - 2, "<< |#%5d|", train->address);
                }
            }
            x++;
        }
        y++;
        x = 1;
        refresh();
    }
}

void Screen::printException(const std::string& txt) {
    attrset(COLOR_PAIR(1)); // red
    mvprintw(0, 0, ">> exception occured! <%s>", txt.c_str());
    attrset(COLOR_PAIR(0));
    refresh();
}

/*
    attrset(COLOR_PAIR(2)); // green
 */
