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
    SymbolEditorState(context)
{
    mCurrentState = new SymbolEditorState_Select(context);
}

SymbolEditorFsm::~SymbolEditorFsm() noexcept
{
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

bool SymbolEditorFsm::processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept
{
    return mCurrentState->processGraphicsSceneMouseMoved(e);
}

bool SymbolEditorFsm::processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept
{
    return mCurrentState->processGraphicsSceneLeftMouseButtonPressed(e);
}

bool SymbolEditorFsm::processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept
{
    return mCurrentState->processGraphicsSceneLeftMouseButtonReleased(e);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

