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
#include "symboleditorstate_addpins.h"
#include <librepcb/common/gridproperties.h>
#include <librepcb/library/sym/symbol.h>
#include <librepcb/library/sym/symbolgraphicsitem.h>
#include <librepcb/library/sym/symbolpin.h>
#include <librepcb/library/sym/symbolpingraphicsitem.h>
#include <librepcb/library/sym/cmd/cmdsymbolpinadd.h>
#include <librepcb/library/sym/cmd/cmdsymbolpinedit.h>
#include "../symboleditorwidget.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolEditorState_AddPins::SymbolEditorState_AddPins(const Context& context) noexcept :
    SymbolEditorState(context), mCurrentPin(nullptr), mCurrentGraphicsItem(nullptr)
{
}

SymbolEditorState_AddPins::~SymbolEditorState_AddPins() noexcept
{
    Q_ASSERT(mEditCmd.isNull());
    Q_ASSERT(mCurrentPin == nullptr);
    Q_ASSERT(mCurrentGraphicsItem == nullptr);
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

bool SymbolEditorState_AddPins::entry() noexcept
{
    // TODO: clear selection
    return addNextPin(Point(0, 0), Angle::deg0());
}

bool SymbolEditorState_AddPins::exit() noexcept
{
    try {
        mCurrentGraphicsItem = nullptr;
        mCurrentPin = nullptr;
        mEditCmd.reset();
        mContext.undoStack.abortCmdGroup();
        return true;
    } catch (const Exception& e) {
        QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
        return false;
    }
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

bool SymbolEditorState_AddPins::processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept
{
    Point currentPos = Point::fromPx(e.scenePos(), mContext.gridProperties.getInterval());
    mEditCmd->setPosition(currentPos, true);
    return true;
}

bool SymbolEditorState_AddPins::processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept
{
    Point currentPos = Point::fromPx(e.scenePos(), mContext.gridProperties.getInterval());
    mEditCmd->setPosition(currentPos, true);
    Angle currentRot = mCurrentPin->getRotation();
    try {
        mCurrentGraphicsItem->setSelected(false);
        mCurrentGraphicsItem = nullptr;
        mCurrentPin = nullptr;
        mContext.undoStack.appendToCmdGroup(mEditCmd.take());
        mContext.undoStack.commitCmdGroup();
        return addNextPin(currentPos, currentRot);
    } catch (const Exception& e) {
        QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
        return false;
    }
}

bool SymbolEditorState_AddPins::processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept
{
    return false;
}

bool SymbolEditorState_AddPins::processRotateCw() noexcept
{
    mEditCmd->rotate(-Angle::deg90(), mCurrentPin->getPosition(), true);
    return true;
}

bool SymbolEditorState_AddPins::processRotateCcw() noexcept
{
    mEditCmd->rotate(Angle::deg90(), mCurrentPin->getPosition(), true);
    return true;
}

bool SymbolEditorState_AddPins::processRemove() noexcept
{
    return false;
}

bool SymbolEditorState_AddPins::processAbortCommand() noexcept
{
    return false;
}

bool SymbolEditorState_AddPins::processStartSelecting() noexcept
{
    return false;
}

bool SymbolEditorState_AddPins::processStartAddingSymbolPins() noexcept
{
    return false;
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool SymbolEditorState_AddPins::addNextPin(const Point& pos, const Angle& rot) noexcept
{
    try {
        mContext.undoStack.beginCmdGroup(tr("Add symbol pin"));
        QScopedPointer<CmdSymbolPinAdd> cmdAdd(
                    new CmdSymbolPinAdd(mContext.symbol, pos, rot));
        mCurrentPin = &cmdAdd->getPin();
        mContext.undoStack.appendToCmdGroup(cmdAdd.take());
        mEditCmd.reset(new CmdSymbolPinEdit(*mCurrentPin));
        mCurrentGraphicsItem = mContext.symbolGraphicsItem.getPinGraphicsItem(mCurrentPin->getUuid());
        Q_ASSERT(mCurrentGraphicsItem);
        mCurrentGraphicsItem->setSelected(true);
        return true;
    } catch (const Exception& e) {
        QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
        mCurrentGraphicsItem = nullptr;
        mCurrentPin = nullptr;
        mEditCmd.reset();
        return false;
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

