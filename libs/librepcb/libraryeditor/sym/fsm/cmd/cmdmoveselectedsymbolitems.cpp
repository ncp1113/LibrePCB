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

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdMoveSelectedSymbolItems::CmdMoveSelectedSymbolItems(Symbol& symbol, const Point& startPos) noexcept :
    UndoCommandGroup(tr("Move Symbol Elements")),
    mSymbol(symbol), mStartPos(startPos), mDeltaPos(0, 0)
{
    // get all selected items
    /*QList<SI_Base*> items = mSchematic.getSelectedItems(false, true, false, true, false, false,
                                                        false, false, false, false, false);

    foreach (SI_Base* item, items) {
        switch (item->getType())
        {
            case SI_Base::Type_t::Symbol: {
                SI_Symbol* symbol = dynamic_cast<SI_Symbol*>(item); Q_ASSERT(symbol);
                CmdSymbolInstanceEdit* cmd = new CmdSymbolInstanceEdit(*symbol);
                mSymbolEditCmds.append(cmd);
                break;
            }
            case SI_Base::Type_t::NetPoint: {
                SI_NetPoint* point = dynamic_cast<SI_NetPoint*>(item); Q_ASSERT(point);
                CmdSchematicNetPointEdit* cmd = new CmdSchematicNetPointEdit(*point);
                mNetPointEditCmds.append(cmd);
                break;
            }
            case SI_Base::Type_t::NetLabel: {
                SI_NetLabel* label = dynamic_cast<SI_NetLabel*>(item); Q_ASSERT(label);
                CmdSchematicNetLabelEdit* cmd = new CmdSchematicNetLabelEdit(*label);
                mNetLabelEditCmds.append(cmd);
                break;
            }
            default: {
                qCritical() << "Unknown schematic item type:" << static_cast<int>(item->getType());
                break;
            }
        }
    }*/
}

CmdMoveSelectedSymbolItems::~CmdMoveSelectedSymbolItems() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void CmdMoveSelectedSymbolItems::setCurrentPosition(const Point& pos) noexcept
{
    /*Point delta = pos - mStartPos;
    delta.mapToGrid(mSchematic.getGridProperties().getInterval());

    if (delta != mDeltaPos) {
        // move selected elements
        foreach (CmdSymbolInstanceEdit* cmd, mSymbolEditCmds) {
            cmd->setDeltaToStartPos(delta, true);
        }
        foreach (CmdSchematicNetPointEdit* cmd, mNetPointEditCmds) {
            cmd->setDeltaToStartPos(delta, true);
        }
        foreach (CmdSchematicNetLabelEdit* cmd, mNetLabelEditCmds) {
            cmd->setDeltaToStartPos(delta, true);
        }
        mDeltaPos = delta;
    }*/
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdMoveSelectedSymbolItems::performExecute() throw (Exception)
{
    /*if (mDeltaPos.isOrigin()) {
        // no movement required --> discard all move commands
        qDeleteAll(mSymbolEditCmds);    mSymbolEditCmds.clear();
        qDeleteAll(mNetPointEditCmds);  mNetPointEditCmds.clear();
        qDeleteAll(mNetLabelEditCmds);  mNetLabelEditCmds.clear();
        return false;
    }

    foreach (CmdSymbolInstanceEdit* cmd, mSymbolEditCmds) {
        appendChild(cmd); // can throw
    }
    foreach (CmdSchematicNetPointEdit* cmd, mNetPointEditCmds) {
        appendChild(cmd); // can throw
    }
    foreach (CmdSchematicNetLabelEdit* cmd, mNetLabelEditCmds) {
        appendChild(cmd); // can throw
    }*/

    // execute all child commands
    return UndoCommandGroup::performExecute(); // can throw
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
