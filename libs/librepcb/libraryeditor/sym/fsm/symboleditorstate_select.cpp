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
#include "symboleditorstate_select.h"
#include <librepcb/common/graphics/graphicsscene.h>
#include <librepcb/common/undostack.h>
#include <librepcb/library/sym/symbolgraphicsitem.h>
#include <librepcb/library/sym/symbolpingraphicsitem.h>
#include "cmd/cmdmoveselectedsymbolitems.h"
#include "cmd/cmdrotateselectedsymbolitems.h"
#include "cmd/cmdremoveselectedsymbolitems.h"
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

SymbolEditorState_Select::SymbolEditorState_Select(const Context& context) noexcept :
    SymbolEditorState(context), mState(SubState::IDLE)
{
}

SymbolEditorState_Select::~SymbolEditorState_Select() noexcept
{
    Q_ASSERT(mCmdMoveSelectedItems.isNull());
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

bool SymbolEditorState_Select::processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept
{
    Point startPos = Point::fromPx(e.buttonDownScenePos(Qt::LeftButton));
    Point currentPos = Point::fromPx(e.scenePos());

    switch (mState) {
        case SubState::SELECTING: {
            setSelectionRect(startPos, currentPos);
            return true;
        }
        case SubState::MOVING: {
            if (!mCmdMoveSelectedItems) {
                mCmdMoveSelectedItems.reset(new CmdMoveSelectedSymbolItems(mContext, startPos));
            }
            mCmdMoveSelectedItems->setCurrentPosition(currentPos);
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept
{
    Point pos = Point::fromPx(e.scenePos());

    switch (mState) {
        case SubState::IDLE: {
            // get items under cursor
            QList<QSharedPointer<SymbolPinGraphicsItem>> pins;
            int count = mContext.symbolGraphicsItem.getItemsAtPosition(pos, pins);
            if (count == 0) {
                // start selecting
                clearSelectionRect(true);
                mState = SubState::SELECTING;
            } else {
                // check if the top most item under the cursor is already selected
                bool itemAlreadySelected = pins.first()->isSelected();
                // remove whole selection if CTRL is not pressed
                if ((!itemAlreadySelected) && (!e.modifiers().testFlag(Qt::ControlModifier))) {
                    clearSelectionRect(true);
                }
                // select top most item under the cursor
                pins.first()->setSelected(true);
                // start moving
                Q_ASSERT(!mCmdMoveSelectedItems);
                mState = SubState::MOVING;
            }
            return true;
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept
{
    switch (mState) {
        case SubState::SELECTING: {
            clearSelectionRect(false);
            mState = SubState::IDLE;
            return true;
        }
        case SubState::MOVING: {
            if (mCmdMoveSelectedItems) {
                try {
                    mContext.undoStack.execCmd(mCmdMoveSelectedItems.take());
                } catch (const Exception& e) {
                    QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
                }
            }
            mState = SubState::IDLE;
            return true;
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processRotateCw() noexcept
{
    switch (mState) {
        case SubState::IDLE: {
            return rotateSelectedItems(-Angle::deg90());
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processRotateCcw() noexcept
{
    switch (mState) {
        case SubState::IDLE: {
            return rotateSelectedItems(Angle::deg90());
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processRemove() noexcept
{
    switch (mState) {
        case SubState::IDLE: {
            return removeSelectedItems();
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processAbortCommand() noexcept
{
    return false;
}

bool SymbolEditorState_Select::processStartSelecting() noexcept
{
    return false;
}

bool SymbolEditorState_Select::processStartAddingSymbolPins() noexcept
{
    return false;
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool SymbolEditorState_Select::rotateSelectedItems(const Angle& angle) noexcept
{
    try {
        mContext.undoStack.execCmd(new CmdRotateSelectedSymbolItems(mContext, angle));
    } catch (const Exception& e) {
        QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
    }
    return true; // TODO: return false if no items were selected
}

bool SymbolEditorState_Select::removeSelectedItems() noexcept
{
    try {
        mContext.undoStack.execCmd(new CmdRemoveSelectedSymbolItems(mContext));
    } catch (const Exception& e) {
        QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getUserMsg());
    }
    return true; // TODO: return false if no items were selected
}

void SymbolEditorState_Select::setSelectionRect(const Point& p1, const Point& p2) noexcept
{
    mContext.graphicsScene.setSelectionRect(p1, p2);
    mContext.symbolGraphicsItem.setSelectionRect(QRectF(p1.toPxQPointF(), p2.toPxQPointF()));
}

void SymbolEditorState_Select::clearSelectionRect(bool updateItemsSelectionState) noexcept
{
    mContext.graphicsScene.setSelectionRect(Point(), Point());
    if (updateItemsSelectionState) {
        mContext.graphicsScene.setSelectionArea(QPainterPath());
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

