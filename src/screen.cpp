/*
 *  Project:    moba-blocksystem
 *
 *  Copyright (C) 2021 Stefan Paproth <pappi-@gmx.de>
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

Screen::~Screen(){
    endwin();
}

void Screen::drawBlock(int i, const ContactData &contact, std::shared_ptr<Block> block) {
    auto in = block->getNextBlockInDirection();
    auto out = block->getNextBlockOutDirection();
    auto train = block->getTrain();

    mvprintw(i, 0, "#%d Block %d   R %d/%d", i, block->getId(), contact.modulAddr, contact.contactNb);
    if(out) {
        mvprintw(i, 27, "[%d] >>>>", out->getId());
    } else {
        mvprintw(i, 27, "[---] >>>>");
    }

    if(train) {
        mvprintw(i, 37, ">>>> [%d] >>>>", train->localId);
    } else {
        mvprintw(i, 37, ">>>> [-----] >>>>");
    }

    if(in) {
        mvprintw(i, 54, ">>>> [%d]", in->getId());
    } else {
        mvprintw(i, 54, ">>>> [---]");
    }

    refresh();
}

void Screen::printLine(const std::string &notice) {
    mvprintw(11, 0, "                                                                      ");
    mvprintw(11, 0, "%s", notice.c_str());
}

/*
    attrset(COLOR_PAIR(0));

    attrset(COLOR_PAIR(2)); // green
    attrset(COLOR_PAIR(1)); // red
 */
