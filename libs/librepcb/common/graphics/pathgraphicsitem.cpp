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
#include <QtWidgets>
#include "pathgraphicsitem.h"
#include "../toolbox.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

PathGraphicsItem::PathGraphicsItem(QGraphicsItem* parent) noexcept :
    QGraphicsItem(parent), mLineLayer(nullptr), mFillLayer(nullptr)
{
    mPen.setWidthF(0);
    mPenHighlighted.setWidthF(0);
    updateColors();
    updateBoundingRectAndShape();
    updateVisibility();
}

PathGraphicsItem::~PathGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void PathGraphicsItem::setPosition(const Point& pos) noexcept
{
    QGraphicsItem::setPos(pos.toPxQPointF());
}

void PathGraphicsItem::setRotation(const Angle& rot) noexcept
{
    QGraphicsItem::setRotation(-rot.toDeg());
}

void PathGraphicsItem::setPath(const QPainterPath& path) noexcept
{
    mPainterPath = path;
    updateBoundingRectAndShape();
}

void PathGraphicsItem::setLineWidth(const Length& width) noexcept
{
    mPen.setWidthF(width.toPx());
    mPenHighlighted.setWidthF(width.toPx());
    updateBoundingRectAndShape();
}

void PathGraphicsItem::setLineLayer(const Layer* layer) noexcept
{
    if (mLineLayer) {
        mLineLayer->unregisterObserver(*this);
    }
    mLineLayer = layer;
    if (mLineLayer) {
        mLineLayer->registerObserver(*this);
    }
    updateColors();
    updateVisibility();
}

void PathGraphicsItem::setFillLayer(const Layer* layer) noexcept
{
    if (mFillLayer) {
        mFillLayer->unregisterObserver(*this);
    }
    mFillLayer = layer;
    if (mFillLayer) {
        mFillLayer->registerObserver(*this);
    }
    updateColors();
    updateVisibility();
}

/*****************************************************************************************
 *  Inherited from IF_LayerObserver
 ****************************************************************************************/

void PathGraphicsItem::layerColorChanged(const Layer& layer, const QColor& newColor) noexcept
{
    if (mLineLayer == &layer) {
        mPen.setColor(newColor);
    }
    if (mFillLayer == &layer) {
        mBrush.setColor(newColor);
    }
    update();
}

void PathGraphicsItem::layerHighlightColorChanged(const Layer& layer, const QColor& newColor) noexcept
{
    if (mLineLayer == &layer) {
        mPenHighlighted.setColor(newColor);
    }
    if (mFillLayer == &layer) {
        mBrushHighlighted.setColor(newColor);
    }
    update();
}

void PathGraphicsItem::layerFillStyleChanged(const Layer& layer, const Qt::BrushStyle& newStyle) noexcept
{
    if ((mFillLayer == &layer) && layer.isVisible()) {
        mBrush.setStyle(newStyle);
        mBrushHighlighted.setStyle(newStyle);
    }
    update();
}

void PathGraphicsItem::layerVisibilityChanged(const Layer& layer, bool newVisible) noexcept
{
    if (mLineLayer == &layer) {
        mPen.setStyle(newVisible ? Qt::SolidLine : Qt::NoPen);
        mPenHighlighted.setStyle(newVisible ? Qt::SolidLine : Qt::NoPen);
    }
    if (mFillLayer == &layer) {
        mBrush.setStyle(newVisible ? layer.getFillStyle() : Qt::NoBrush);
        mBrushHighlighted.setStyle(newVisible ? layer.getFillStyle() : Qt::NoBrush);
    }
    updateVisibility();
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

void PathGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept
{
    Q_UNUSED(widget);
    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->setPen(mPenHighlighted);
        painter->setBrush(mBrushHighlighted);
    } else {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
    }
    painter->drawPath(mPainterPath);
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void PathGraphicsItem::updateColors() noexcept
{
    if (mLineLayer) {
        mPen.setStyle(Qt::SolidLine);
        mPenHighlighted.setStyle(Qt::SolidLine);
        mPen.setColor(mLineLayer->getColor(false));
        mPenHighlighted.setColor(mLineLayer->getColor(true));
    } else {
        mPen.setStyle(Qt::NoPen);
        mPenHighlighted.setStyle(Qt::NoPen);
    }

    if (mFillLayer) {
        mBrush.setStyle(Qt::SolidPattern);
        mBrushHighlighted.setStyle(Qt::SolidPattern);
        mBrush.setColor(mFillLayer->getColor(false));
        mBrushHighlighted.setColor(mFillLayer->getColor(true));
    } else {
        mBrush.setStyle(Qt::NoBrush);
        mBrushHighlighted.setStyle(Qt::NoBrush);
    }
    update();
}

void PathGraphicsItem::updateBoundingRectAndShape() noexcept
{
    prepareGeometryChange();
    mShape = Toolbox::shapeFromPath(mPainterPath, mPen);
    mBoundingRect = mShape.controlPointRect();
    update();
}

void PathGraphicsItem::updateVisibility() noexcept
{
    setVisible((mPen.style() != Qt::NoPen) || (mBrush.style() != Qt::NoBrush));
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
