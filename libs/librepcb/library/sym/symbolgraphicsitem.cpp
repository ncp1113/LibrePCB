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
#include "symbolgraphicsitem.h"
#include "symbol.h"
#include "symbolpingraphicsitem.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolGraphicsItem::SymbolGraphicsItem(const Symbol& symbol,
                                       const IF_SchematicLayerProvider& layerProvider) noexcept :
    QGraphicsItem(nullptr), mSymbol(symbol), mLayerProvider(layerProvider)
{
    foreach (const Uuid& uuid, mSymbol.getPinUuids()) {
        const SymbolPin* pin = mSymbol.getPinByUuid(uuid); Q_ASSERT(pin);
        QSharedPointer<SymbolPinGraphicsItem> item(
                    new SymbolPinGraphicsItem(*pin, mLayerProvider, this));
        mPinGraphicsItems.insert(pin->getUuid(), item);
    }

    /*for (int i = 0; i < mSymbol.getPolygonCount(); ++i) {
        const Polygon* polygon = mSymbol.getPolygon(i); Q_ASSERT(polygon);
        QSharedPointer<PolygonGraphicsItem> item(
                    new SymbolPinGraphicsItem(*pin, mLayerProvider, this));
        mPinGraphicsItems.insert(pin->getUuid(), item);
    }*/
}

SymbolGraphicsItem::~SymbolGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void SymbolGraphicsItem::setPosition(const Point& pos) noexcept
{
    QGraphicsItem::setPos(pos.toPxQPointF());
}

void SymbolGraphicsItem::setRotation(const Angle& rot) noexcept
{
    QGraphicsItem::setRotation(-rot.toDeg());
}

void SymbolGraphicsItem::setSelectionRect(const QRectF rect) noexcept
{
    /*foreach (const QSharedPointer<SymbolPinGraphicsItem>& item, mPinGraphicsItems) {
        QPolygonF mappedRect = mapToItem(item.data(), rect);
        item->setSelected(item->shape().intersects(mappedRect));
    }*/
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

void SymbolGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept
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
