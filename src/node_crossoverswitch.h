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

#include "node.h"

class CrossOverSwitch;

using CrossOverSwitchPtr = std::shared_ptr<CrossOverSwitch>;

class CrossOverSwitch : public Node {
public:
    enum SwitchState {
        BEND_1,
        STRAIGHT_1,
        BEND_2,
        STRAIGHT_2,
    };

    CrossOverSwitch(SwitchState state);

    virtual ~CrossOverSwitch();

    virtual NodePtr getJunctionNode(NodePtr node) const;

    void setInNodeDiagonal(NodePtr node);

    void setInNodeVertical(NodePtr node);

    void setOutNodeDiagonal(NodePtr node);

    void setOutNodeVertical(NodePtr node);

    bool turnSwitch(SwitchState state);

    SwitchState turnSwitch();

    NodePtr getJunctionNode(NodePtr node);

    NodePtr getInNode() const;

    NodePtr getOutNode() const;

private:
    NodePtr inDiagonal;
    NodePtr inVertical;
    NodePtr outDiagonal;
    NodePtr outVertical;

    SwitchState currentState;
};

