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
}

Screen::~Screen(){
    endwin();
}

void Screen::drawBlock(int i, const ContactData &contact, std::shared_ptr<Block> block) {
    auto in = block->getNextBlockInDirection();
    auto out = block->getNextBlockOutDirection();

    mvprintw(i, 0, "Block #%d", i);
    mvprintw(i, 11, "R %d/%d", contact.modulAddr, contact.contactNb);
    if(in) {
        mvprintw(i, 22, "[%d] <<<<", in->getId());
    } else {
        mvprintw(i, 22, "[---] <<<<");
    }
    mvprintw(i, 32, "<<<< [%d] <<<<", block->getId());
    if(out) {
        mvprintw(i, 47, "<<<< [%d]", out->getId());
    } else {
        mvprintw(i, 47, "<<<< [---]");
    }

    refresh();



    auto train = block->getTrain();

    if(train) {
        //train->localId
    }
}
