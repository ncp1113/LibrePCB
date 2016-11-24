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
#include <librepcb/library/sym/symbolgraphicsitem.h>

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
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

bool SymbolEditorState_Select::processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept
{
    switch (mState) {
        case SubState::SELECTING: {
            Point startPos = Point::fromPx(e.buttonDownScenePos(Qt::LeftButton));
            Point currentPos = Point::fromPx(e.scenePos());
            setSelectionRect(startPos, currentPos);
            return true;
        }
        default: {
            return false;
        }
    }
}

bool SymbolEditorState_Select::processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept
{
    switch (mState) {
        case SubState::IDLE: {
            clearSelectionRect(true);
            Point pos = Point::fromPx(e.scenePos());
            QGraphicsItem* item = mContext.graphicsScene.itemAt(pos.toPxQPointF(), QTransform());
            if (item && (item != &mContext.symbolGraphicsItem)) {
                // select + move one single item
                item->setSelected(true);
            } else {
                // start selection mode
                mState = SubState::SELECTING;
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
        default: {
            return false;
        }
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void SymbolEditorState_Select::setSelectionRect(const Point& p1, const Point& p2) noexcept
{
    mContext.graphicsScene.setSelectionRect(p1, p2);
    QPainterPath path;
    path.addRect(QRectF(p1.toPxQPointF(), p2.toPxQPointF()));
    mContext.graphicsScene.setSelectionArea(path);
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

