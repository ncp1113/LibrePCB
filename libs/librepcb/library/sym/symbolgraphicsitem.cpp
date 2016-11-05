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
#include <librepcb/common/schematiclayer.h>
#include <librepcb/common/if_schematiclayerprovider.h>
#include "symbol.h"
#include "symbolpingraphicsitem.h"
#include "symbolpinpreviewgraphicsitem.h"

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
    mSymbol(symbol)
{
    // add pins
    foreach (const Uuid& uuid, mSymbol.getPinUuids()) {
        const SymbolPin* pin = mSymbol.getPinByUuid(uuid); Q_ASSERT(pin);
        //QSharedPointer<SymbolPinGraphicsItem> item(
        //    new SymbolPinGraphicsItem(*pin, layerProvider));
        QSharedPointer<SymbolPinPreviewGraphicsItem> item(new SymbolPinPreviewGraphicsItem(
            layerProvider, *pin, nullptr, CmpSigPinDisplayType::pinName()));
        item->setParentItem(this);
        item->setPos(pin->getPosition().toPxQPointF());
        item->setRotation(-pin->getRotation().toDeg());
        mPinGraphicsItems.insert(uuid, item);
    }

    // add polygons
    for (int i = 0; i < mSymbol.getPolygonCount(); ++i) {
        const Polygon* polygon = mSymbol.getPolygon(i); Q_ASSERT(polygon);
        QSharedPointer<QGraphicsPathItem> item(
            new QGraphicsPathItem(polygon->toQPainterPathPx(), this));
        SchematicLayer* layer = layerProvider.getSchematicLayer(polygon->getLayerId());
        item->setVisible(layer && layer->isVisible());
        item->setPen(QPen(layer ? layer->getColor(false) : Qt::NoBrush,
                          polygon->getLineWidth().toPx(),
                          Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        if (polygon->isFilled()) {
            layer = layerProvider.getSchematicLayer(polygon->getLayerId());
        } else if (polygon->isGrabArea()) {
            layer = layerProvider.getSchematicLayer(SchematicLayer::LayerID::SymbolGrabAreas);
        } else {
            layer = nullptr;
        }
        item->setBrush(layer ? QBrush(layer->getColor(false)) : Qt::NoBrush);
        mPolygonGraphicsItems.append(item);
    }

    // add texts
    for (int i = 0; i < mSymbol.getTextCount(); ++i) {
        const Text* text = mSymbol.getText(i); Q_ASSERT(text);
        QSharedPointer<QGraphicsSimpleTextItem> item(
            new QGraphicsSimpleTextItem(text->getText(), this));
        SchematicLayer* layer = layerProvider.getSchematicLayer(text->getLayerId());
        item->setVisible(layer && layer->isVisible());
        item->setBrush(layer ? layer->getColor(false) : Qt::NoBrush);
        item->setPos(text->getPosition().toPxQPointF());
        item->setRotation(-text->getRotation().toDeg());
        mTextGraphicsItems.append(item);
    }
}

SymbolGraphicsItem::~SymbolGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

/*void SymbolGraphicsItem::setSelectionRect(const Point& p1, const Point& p2) noexcept
{
    QRectF rectPx = QRectF(p1.toPxQPointF(), p2.toPxQPointF()).normalized();
    foreach (const auto& pinItem, mPinGraphicsItems) {
        //pinItem->setSelected(pinItem->getGrabAreaScenePx().intersects(rectPx));
        pinItem->setSelected(pinItem->shape().intersects(rectPx));
    }
}*/

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

