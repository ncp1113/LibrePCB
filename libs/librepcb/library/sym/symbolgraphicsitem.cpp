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

SymbolGraphicsItem::SymbolGraphicsItem(Symbol& symbol,
                                       const IF_SchematicLayerProvider& layerProvider) noexcept :
    QGraphicsItem(nullptr), mSymbol(symbol), mLayerProvider(layerProvider)
{
    foreach (const Uuid& uuid, mSymbol.getPinUuids()) {
        SymbolPin* pin = mSymbol.getPinByUuid(uuid); Q_ASSERT(pin);
        addPin(*pin);
    }

    /*for (int i = 0; i < mSymbol.getPolygonCount(); ++i) {
        const Polygon* polygon = mSymbol.getPolygon(i); Q_ASSERT(polygon);
        QSharedPointer<PolygonGraphicsItem> item(
                    new SymbolPinGraphicsItem(*pin, mLayerProvider, this));
        mPinGraphicsItems.insert(pin->getUuid(), item);
    }*/

    // register to the symbol to get attribute updates
    mSymbol.registerGraphicsItem(*this);
}

SymbolGraphicsItem::~SymbolGraphicsItem() noexcept
{
    mSymbol.unregisterGraphicsItem(*this);
}

/*****************************************************************************************
 *  Getters
 ****************************************************************************************/

SymbolPinGraphicsItem* SymbolGraphicsItem::getPinGraphicsItem(const Uuid& pin) noexcept
{
    return mPinGraphicsItems.value(pin).data();
}

int SymbolGraphicsItem::getItemsAtPosition(const Point& pos,
    QList<QSharedPointer<SymbolPinGraphicsItem>>& pins) noexcept
{
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& item, mPinGraphicsItems) {
        QPointF mappedPos = mapToItem(item.data(), pos.toPxQPointF());
        if (item->shape().contains(mappedPos)) {
            pins.append(item);
        }
    }
    return pins.count();
}

QList<QSharedPointer<SymbolPinGraphicsItem>> SymbolGraphicsItem::getSelectedPins() noexcept
{
    QList<QSharedPointer<SymbolPinGraphicsItem>> pins;
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& item, mPinGraphicsItems) {
        if (item->isSelected()) {
            pins.append(item);
        }
    }
    return pins;
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

void SymbolGraphicsItem::addPin(SymbolPin& pin) noexcept
{
    Q_ASSERT(!mPinGraphicsItems.contains(pin.getUuid()));
    QSharedPointer<SymbolPinGraphicsItem> item(
                new SymbolPinGraphicsItem(pin, mLayerProvider, this));
    mPinGraphicsItems.insert(pin.getUuid(), item);
}

void SymbolGraphicsItem::removePin(SymbolPin& pin) noexcept
{
    Q_ASSERT(mPinGraphicsItems.contains(pin.getUuid()));
    mPinGraphicsItems.remove(pin.getUuid()); // this deletes the graphics item
}

void SymbolGraphicsItem::setSelectionRect(const QRectF rect) noexcept
{
    QPainterPath path;
    path.addRect(rect);
    foreach (const QSharedPointer<SymbolPinGraphicsItem>& item, mPinGraphicsItems) {
        QPainterPath mappedPath = mapToItem(item.data(), path);
        item->setSelected(item->shape().intersects(mappedPath));
    }
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
