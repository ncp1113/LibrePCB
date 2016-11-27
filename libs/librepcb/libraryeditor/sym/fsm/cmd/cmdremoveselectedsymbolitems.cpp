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
#include "cmdremoveselectedsymbolitems.h"
#include <librepcb/library/sym/symbolgraphicsitem.h>
#include <librepcb/library/sym/symbolpingraphicsitem.h>
#include <librepcb/library/sym/cmd/cmdsymbolpinremove.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdRemoveSelectedSymbolItems::CmdRemoveSelectedSymbolItems(const SymbolEditorState::Context& context) noexcept :
    UndoCommandGroup(tr("Remove Symbol Elements")), mContext(context)
{
}

CmdRemoveSelectedSymbolItems::~CmdRemoveSelectedSymbolItems() noexcept
{
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdRemoveSelectedSymbolItems::performExecute() throw (Exception)
{
    // get all selected items
    QList<QSharedPointer<SymbolPinGraphicsItem>> pins = mContext.symbolGraphicsItem.getSelectedPins();

    // no items selected --> nothing to do here
    if (pins.isEmpty()) {
        return false;
    }

    // remove all selected elements
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& pin, pins) {Q_ASSERT(pin);
        appendChild(new CmdSymbolPinRemove(mContext.symbol, pin->getPin()));
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
