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
#include "symbolpin.h"
#include <librepcb/common/schematiclayer.h>
#include <librepcb/common/if_schematiclayerprovider.h>
#include <librepcb/common/graphics/ellipsegraphicsitem.h>
#include <librepcb/common/graphics/linegraphicsitem.h>
#include <librepcb/common/graphics/textgraphicsitem.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolPinGraphicsItem::SymbolPinGraphicsItem(const SymbolPin& pin,
        const IF_SchematicLayerProvider& layerProvider, QGraphicsItem* parent) noexcept :
    QGraphicsItemGroup(parent), mPin(pin),
    mCircleGraphicsItem(new EllipseGraphicsItem()),
    mLineGraphicsItem(new LineGraphicsItem()),
    mTextGraphicsItem(new TextGraphicsItem())
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlag(QGraphicsItem::ItemHasNoContents, false);
    //setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    // circle
    mCircleGraphicsItem->setRadius(Length(600000), Length(600000));
    mCircleGraphicsItem->setLineLayer(layerProvider.getSchematicLayer(
                                          SchematicLayer::SymbolOptionalPinCircles));
    mCircleGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    addToGroup(mCircleGraphicsItem.data());

    // line
    mLineGraphicsItem->setLineWidth(Length(158750));
    mLineGraphicsItem->setLayer(layerProvider.getSchematicLayer(
                                    SchematicLayer::SymbolOutlines));
    mLineGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    addToGroup(mLineGraphicsItem.data());

    // text
    mTextGraphicsItem->setHeight(Length::fromMm(qreal(2)));
    mTextGraphicsItem->setAlignment(Alignment(HAlign::left(), VAlign::center()));
    mTextGraphicsItem->setLayer(layerProvider.getSchematicLayer(
                                    SchematicLayer::SymbolPinNames));
    mTextGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    addToGroup(mTextGraphicsItem.data());

    // pin properties
    setPosition(mPin.getPosition());
    setRotation(mPin.getRotation());
    setLength(mPin.getLength());
    setName(mPin.getName());
}

SymbolPinGraphicsItem::~SymbolPinGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void SymbolPinGraphicsItem::setPosition(const Point& pos) noexcept
{
    QGraphicsItemGroup::setPos(pos.toPxQPointF());
}

void SymbolPinGraphicsItem::setRotation(const Angle& rot) noexcept
{
    QGraphicsItemGroup::setRotation(-rot.toDeg());
}

void SymbolPinGraphicsItem::setLength(const Length& length) noexcept
{
    mLineGraphicsItem->setLine(Point(0, 0), Point(length, 0));
    mTextGraphicsItem->setPosition(Point(length + Length(500000), Length(0)));
}

void SymbolPinGraphicsItem::setName(const QString& name) noexcept
{
    setToolTip(name);
    mTextGraphicsItem->setText(name);
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

QVariant SymbolPinGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) noexcept
{
    if (change == ItemSelectedChange) {
        /*mCircleGraphicsItem->setSelected(value.toBool());
        mLineGraphicsItem->setSelected(value.toBool());
        mTextGraphicsItem->setSelected(value.toBool());*/
    }
    return QGraphicsItem::itemChange(change, value);
}

QPainterPath SymbolPinGraphicsItem::shape() const noexcept
{
    return mCircleGraphicsItem->shape();
    /*QPainterPath p;
    p.addRect(-10, -10, 20, 20);
    return p;*/
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
