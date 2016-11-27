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
#include "cmdrotateselectedsymbolitems.h"
#include <librepcb/common/gridproperties.h>
#include <librepcb/library/sym/symbolpin.h>
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

CmdRotateSelectedSymbolItems::CmdRotateSelectedSymbolItems(const SymbolEditorState::Context& context,
                                                       const Angle& angle) noexcept :
    UndoCommandGroup(tr("Rotate Symbol Elements")), mContext(context), mAngle(angle)
{
}

CmdRotateSelectedSymbolItems::~CmdRotateSelectedSymbolItems() noexcept
{
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdRotateSelectedSymbolItems::performExecute() throw (Exception)
{
    // get all selected items
    QList<QSharedPointer<SymbolPinGraphicsItem>> pins = mContext.symbolGraphicsItem.getSelectedPins();

    // no items selected --> nothing to do here
    if (pins.isEmpty()) {
        return false;
    }

    // find the center of all elements
    Point center = Point(0, 0);
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& pin, pins) {Q_ASSERT(pin);
        center += pin->getPin().getPosition();
    }
    center /= pins.count();
    center.mapToGrid(mContext.gridProperties.getInterval());

    // rotate all selected elements
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& pin, pins) {Q_ASSERT(pin);
        CmdSymbolPinEdit* cmd = new CmdSymbolPinEdit(pin->getPin());
        cmd->rotate(mAngle, center, false);
        appendChild(cmd);
    }

    // execute all child commands
    return UndoCommandGroup::performExecute(); // can throw
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
