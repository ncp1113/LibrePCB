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
#include "cmdmoveselectedsymbolitems.h"
#include <librepcb/common/gridproperties.h>
#include <librepcb/library/sym/symbolgraphicsitem.h>
#include <librepcb/library/sym/symbolpingraphicsitem.h>
#include <librepcb/library/sym/cmd/cmdsymbolpinedit.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdMoveSelectedSymbolItems::CmdMoveSelectedSymbolItems(const SymbolEditorState::Context& context,
                                                       const Point& startPos) noexcept :
    UndoCommandGroup(tr("Move Symbol Elements")),
    mContext(context), mStartPos(startPos), mDeltaPos(0, 0)
{
    QList<QSharedPointer<SymbolPinGraphicsItem>> pins = context.symbolGraphicsItem.getSelectedPins();
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& pin, pins) {Q_ASSERT(pin);
        mPinEditCmds.append(new CmdSymbolPinEdit(pin->getPin()));
    }
}

CmdMoveSelectedSymbolItems::~CmdMoveSelectedSymbolItems() noexcept
{
    deleteAllCommands();
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void CmdMoveSelectedSymbolItems::setCurrentPosition(const Point& pos) noexcept
{
    Point delta = pos - mStartPos;
    delta.mapToGrid(mContext.gridProperties.getInterval());

    if (delta != mDeltaPos) {
        // move selected elements
        foreach (CmdSymbolPinEdit* cmd, mPinEditCmds) {
            cmd->setDeltaToStartPos(delta, true);
        }
        mDeltaPos = delta;
    }
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdMoveSelectedSymbolItems::performExecute() throw (Exception)
{
    if (mDeltaPos.isOrigin()) {
        // no movement required --> discard all move commands
        deleteAllCommands();
        return false;
    }

    // move all child commands to parent class
    while (mPinEditCmds.count() > 0) {
        appendChild(mPinEditCmds.takeLast());
    }

    // execute all child commands
    return UndoCommandGroup::performExecute(); // can throw
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void CmdMoveSelectedSymbolItems::deleteAllCommands() noexcept
{
    qDeleteAll(mPinEditCmds);           mPinEditCmds.clear();
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
