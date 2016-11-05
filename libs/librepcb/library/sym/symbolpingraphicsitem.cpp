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
#include "symbolpingraphicsitem.h"
#include <librepcb/common/schematiclayer.h>
#include <librepcb/common/if_schematiclayerprovider.h>
#include <librepcb/common/toolbox.h>
#include "symbolpin.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolPinGraphicsItem::SymbolPinGraphicsItem(const SymbolPin& pin,
        const IF_SchematicLayerProvider& layerProvider) noexcept :
    QGraphicsItemGroup(), mPin(pin),
    mCircleLayer(layerProvider.getSchematicLayer(SchematicLayer::SymbolPinCircles)),
    mLineLayer(layerProvider.getSchematicLayer(SchematicLayer::SymbolOutlines))//,
    //mCircleItem(this), mLineItem(this)
{
    // set position and rotation
    setPos(mPin.getPosition().toPxQPointF());
    setRotation(-mPin.getRotation().toDeg());

    // add circle
    /*mCircleItem.setZValue(1);
    mCircleItem.setToolTip(mPin.getName());
    mCircleItem.setRect(Toolbox::boundingRectFromRadius(Length(600000).toPx()));
    mCircleItem.setVisible(mCircleLayer && mCircleLayer->isVisible());
    mCircleItem.setPen(QPen(mCircleLayer ? mCircleLayer->getColor(false) : Qt::NoBrush, 0));
    mCircleItem.setBrush(Qt::NoBrush);

    // add line
    mLineItem.setZValue(0);
    mLineItem.setLine(0, 0, mPin.getLength().toPx(), 0);
    mCircleItem.setVisible(mLineLayer && mLineLayer->isVisible());
    mLineItem.setPen(QPen(mLineLayer ? mLineLayer->getColor(false) : Qt::NoBrush,
                          Length(158750).toPx(), Qt::SolidLine, Qt::RoundCap));*/
}

SymbolPinGraphicsItem::~SymbolPinGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

