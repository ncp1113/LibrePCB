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

SymbolPinGraphicsItem::SymbolPinGraphicsItem(SymbolPin& pin,
        const IF_SchematicLayerProvider& layerProvider, QGraphicsItem* parent) noexcept :
    QGraphicsItem(parent), mPin(pin),
    mCircleGraphicsItem(new EllipseGraphicsItem(this)),
    mLineGraphicsItem(new LineGraphicsItem(this)),
    mTextGraphicsItem(new TextGraphicsItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // circle
    mCircleGraphicsItem->setRadius(Length(600000), Length(600000));
    mCircleGraphicsItem->setLineLayer(layerProvider.getSchematicLayer(
                                          SchematicLayer::SymbolOptionalPinCircles));
    mCircleGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // line
    mLineGraphicsItem->setLineWidth(Length(158750));
    mLineGraphicsItem->setLayer(layerProvider.getSchematicLayer(
                                    SchematicLayer::SymbolOutlines));
    mLineGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // text
    mTextGraphicsItem->setHeight(Length::fromMm(qreal(2)));
    mTextGraphicsItem->setAlignment(Alignment(HAlign::left(), VAlign::center()));
    mTextGraphicsItem->setLayer(layerProvider.getSchematicLayer(
                                    SchematicLayer::SymbolPinNames));
    mTextGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // pin properties
    setPosition(mPin.getPosition());
    setRotation(mPin.getRotation());
    setLength(mPin.getLength());
    setName(mPin.getName());

    // register to the pin to get attribute updates
    mPin.registerGraphicsItem(*this);
}

SymbolPinGraphicsItem::~SymbolPinGraphicsItem() noexcept
{
    mPin.unregisterGraphicsItem(*this);
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void SymbolPinGraphicsItem::setPosition(const Point& pos) noexcept
{
    QGraphicsItem::setPos(pos.toPxQPointF());
}

void SymbolPinGraphicsItem::setRotation(const Angle& rot) noexcept
{
    QGraphicsItem::setRotation(-rot.toDeg());
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

void SymbolPinGraphicsItem::setSelected(bool selected) noexcept
{
    mCircleGraphicsItem->setSelected(selected);
    mLineGraphicsItem->setSelected(selected);
    mTextGraphicsItem->setSelected(selected);
    QGraphicsItem::setSelected(selected);
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

QPainterPath SymbolPinGraphicsItem::shape() const noexcept
{
    return mCircleGraphicsItem->shape();
}

void SymbolPinGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
