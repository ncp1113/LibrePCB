/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "symboleditorfsm.h"
#include "symboleditorstate_select.h"
#include "symboleditorstate_addpins.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolEditorFsm::SymbolEditorFsm(const Context& context) noexcept :
    SymbolEditorState(context), mCurrentState(State::IDLE)
{
    mStates.insert(State::SELECT,   new SymbolEditorState_Select(context));
    mStates.insert(State::ADD_PINS, new SymbolEditorState_AddPins(context));
    enterNextState(State::SELECT);
}

SymbolEditorFsm::~SymbolEditorFsm() noexcept
{
    leaveCurrentState();
    qDeleteAll(mStates);    mStates.clear();
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

bool SymbolEditorFsm::processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processGraphicsSceneMouseMoved(e);
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processGraphicsSceneLeftMouseButtonPressed(e);
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processGraphicsSceneLeftMouseButtonReleased(e);
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processRotateCw() noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processRotateCw();
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processRotateCcw() noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processRotateCcw();
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processRemove() noexcept
{
    if (getCurrentState()) {
        return getCurrentState()->processRemove();
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processAbortCommand() noexcept
{
    if (getCurrentState() && (!getCurrentState()->processAbortCommand())) {
        return setNextState(State::SELECT);
    } else {
        return false;
    }
}

bool SymbolEditorFsm::processStartSelecting() noexcept
{
    if (getCurrentState() && getCurrentState()->processStartSelecting()) {
        return true;
    } else {
        return setNextState(State::SELECT);;
    }
}

bool SymbolEditorFsm::processStartAddingSymbolPins() noexcept
{
    if (getCurrentState() && getCurrentState()->processStartAddingSymbolPins()) {
        return true;
    } else {
        return setNextState(State::ADD_PINS);;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

SymbolEditorState* SymbolEditorFsm::getCurrentState() const noexcept
{
    return mStates.value(mCurrentState, nullptr);
}

bool SymbolEditorFsm::setNextState(State state) noexcept
{
    if (state == mCurrentState) {
        return true;
    }
    if (!leaveCurrentState()) {
        return false;
    }
    return enterNextState(state);
}

bool SymbolEditorFsm::leaveCurrentState() noexcept
{
    if ((getCurrentState()) && (!getCurrentState()->exit())) {
        return false;
    }
    mCurrentState = State::IDLE;
    return true;
}

bool SymbolEditorFsm::enterNextState(State state) noexcept
{
    Q_ASSERT(mCurrentState == State::IDLE);
    SymbolEditorState* nextState = mStates.value(state, nullptr);
    if ((nextState) && (!nextState->entry())) {
        return false;
    }
    mCurrentState = state;
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

