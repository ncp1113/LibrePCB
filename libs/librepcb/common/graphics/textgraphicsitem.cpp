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
#include "textgraphicsitem.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

TextGraphicsItem::TextGraphicsItem(QGraphicsItem* parent) noexcept :
    QGraphicsItem(parent), mLayer(nullptr), mAlignment(HAlign::left(), VAlign::bottom()),
    mTextFlags(0)
{
    mFont.setStyleStrategy(QFont::StyleStrategy(QFont::OpenGLCompatible | QFont::PreferQuality));
    mFont.setStyleHint(QFont::SansSerif);
    mFont.setFamily("Nimbus Sans L");
    mFont.setPixelSize(1);

    updateBoundingRectAndShape();
    setVisible(false);
}

TextGraphicsItem::~TextGraphicsItem() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void TextGraphicsItem::setPosition(const Point& pos) noexcept
{
    QGraphicsItem::setPos(pos.toPxQPointF());
}

void TextGraphicsItem::setRotation(const Angle& rot) noexcept
{
    QGraphicsItem::setRotation(-rot.toDeg());
}

void TextGraphicsItem::setText(const QString& text) noexcept
{
    mText = text;
    updateBoundingRectAndShape();
}

void TextGraphicsItem::setHeight(const Length& height) noexcept
{
    mFont.setPixelSize(height.toPx());
    updateBoundingRectAndShape();
}

void TextGraphicsItem::setAlignment(const Alignment& align) noexcept
{
    mAlignment = align;
    updateBoundingRectAndShape();
}

void TextGraphicsItem::setFont(Font font) noexcept
{
    switch (font) {
        case Font::SansSerif: {
            mFont.setStyleHint(QFont::SansSerif);
            mFont.setFamily("Nimbus Sans L");
            break;
        }
        case Font::Monospace: {
            mFont.setStyleHint(QFont::TypeWriter);
            mFont.setFamily("Monospace");
            break;
        }
        default: {
            Q_ASSERT(false);
            qCritical() << "Unknown font:" << static_cast<int>(font);
            break;
        }
    }
    updateBoundingRectAndShape();
}

void TextGraphicsItem::setLayer(const Layer* layer) noexcept
{
    if (mLayer) {
        mLayer->unregisterObserver(*this);
    }
    mLayer = layer;
    if (mLayer) {
        mLayer->registerObserver(*this);
        mPen.setColor(mLayer->getColor(false));
        mPenHighlighted.setColor(mLayer->getColor(true));
        setVisible(mLayer->isVisible());
    } else {
        setVisible(false);
    }
}

/*****************************************************************************************
 *  Inherited from IF_LayerObserver
 ****************************************************************************************/

void TextGraphicsItem::layerColorChanged(const Layer& layer, const QColor& newColor) noexcept
{
    Q_UNUSED(layer);
    Q_ASSERT(&layer == mLayer);
    mPen.setColor(newColor);
    update();
}

void TextGraphicsItem::layerHighlightColorChanged(const Layer& layer, const QColor& newColor) noexcept
{
    Q_UNUSED(layer);
    Q_ASSERT(&layer == mLayer);
    mPenHighlighted.setColor(newColor);
    update();
}

void TextGraphicsItem::layerFillStyleChanged(const Layer& layer, const Qt::BrushStyle& newStyle) noexcept
{
    Q_UNUSED(layer);
    Q_UNUSED(newStyle);
}

void TextGraphicsItem::layerVisibilityChanged(const Layer& layer, bool newVisible) noexcept
{
    Q_UNUSED(layer);
    Q_ASSERT(&layer == mLayer);
    setVisible(newVisible);
}

/*****************************************************************************************
 *  Inherited from QGraphicsItem
 ****************************************************************************************/

void TextGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) noexcept
{
    Q_UNUSED(widget);
    painter->setFont(mFont);
    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->setPen(mPenHighlighted);
    } else {
        painter->setPen(mPen);
    }

    if (mapToScene(0, 1).y() < mapToScene(0, 0).y()) {
        // The text needs to be rotated 180°!
        // TODO: Is there a better solution to determine the overall rotation of the item?
        //painter->save();
        painter->rotate(180);
        painter->translate(-mBoundingRect.topLeft() - mBoundingRect.bottomRight());
        painter->drawText(QRectF(), mTextFlags, mText);
        //painter->restore();
    } else {
        painter->drawText(QRectF(), mTextFlags, mText);
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void TextGraphicsItem::updateBoundingRectAndShape() noexcept
{
    prepareGeometryChange();
    mTextFlags = Qt::TextDontClip | mAlignment.toQtAlign();
    QFontMetricsF fm(mFont);
    mBoundingRect = fm.boundingRect(QRectF(), mTextFlags, mText);
    mShape = QPainterPath();
    mShape.addRect(mBoundingRect);
    update();
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
