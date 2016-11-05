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
#include "ellipsegraphicsitem.h"
#include "../geometry/ellipse.h"
#include "../toolbox.h"
#include "../if_layerprovider.h"
#include "../schematiclayer.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

EllipseGraphicsItem::EllipseGraphicsItem(const Ellipse& ellipse,
                                         const IF_LayerProvider& layerProvider) noexcept :
    QGraphicsItem(), mEllipse(ellipse), mLayerProvider(layerProvider)//, mLayer(nullptr)
{
}

EllipseGraphicsItem::~EllipseGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void EllipseGraphicsItem::updateCacheAndRepaint() noexcept
{
    const Layer* layer = mLayerProvider.getLayer(mEllipse.getLayerId());
    if (layer && layer->isVisible()) {
        setVisible(true);

        // line width
        mPen.setWidthF(mEllipse.getLineWidth().toPx());
        mPenHighlighted.setWidthF(mEllipse.getLineWidth().toPx());

        // is filled / is grab area
        if (mEllipse.isFilled()) {
            mBrush.setColor(layer->getColor(false));
            mBrushHighlighted.setColor(layer->getColor(true));
            mBrush.setStyle(layer->getFillStyle());
            mBrushHighlighted.setStyle(layer->getFillStyle());
        } else if (mEllipse.isGrabArea()) {
            //const Layer*
            mBrush.setColor(layer->getColor(false));
            mBrushHighlighted.setColor(layer->getColor(true));
            mBrush.setStyle(layer->getFillStyle());
            mBrushHighlighted.setStyle(layer->getFillStyle());
        } else {

        }

        // size
        mEllipseRect = Toolbox::boundingRectFromRadius(mEllipse.getRadiusX().toPx(),
                                                       mEllipse.getRadiusY().toPx());
        mBoundingRect = mEllipseRect.adjusted(-mPen.widthF(), -mPen.widthF(), mPen.widthF(), mPen.widthF());

        // position + rotation
        setPos(mEllipse.getCenter().toPxQPointF());
        setRotation(-mEllipse.getRotation().toDeg());

        QPainterPath path;
        path.addEllipse(mEllipseRect);
        mShape = Toolbox::shapeFromPath(path, mPen);

        // line color
        mPen.setColor(layer->getColor(false));
        mPenHighlighted.setColor(layer->getColor(true));



        // schedule repaint
        update();
    } else {
        setVisible(false);
    }
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

void EllipseGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept
{
    Q_UNUSED(widget);
    if (option->state & QStyle::State_Selected) {
        painter->setPen(mPenHighlighted);
        painter->setBrush(mBrushHighlighted);
    } else {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
    }
    painter->drawEllipse(mEllipseRect);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

