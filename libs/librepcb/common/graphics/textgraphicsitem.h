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

#ifndef LIBREPCB_TEXTGRAPHICSITEM_H
#define LIBREPCB_TEXTGRAPHICSITEM_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include "../layer.h"
#include "../alignment.h"
#include "../units/all_length_units.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class TextGraphicsItem
 ****************************************************************************************/

/**
 * @brief The TextGraphicsItem class is the graphical representation of a text
 *
 * @todo Would QStaticText improve the performance?
 *
 * @author ubruhin
 * @date 2016-11-06
 */
class TextGraphicsItem final : public QGraphicsItem, public IF_LayerObserver
{
    public:

        // Types
        enum class Font {SansSerif, Monospace};

        // Constructors / Destructor
        //TextGraphicsItem() = delete;
        TextGraphicsItem(const TextGraphicsItem& other) = delete;
        explicit TextGraphicsItem(QGraphicsItem* parent = nullptr) noexcept;
        ~TextGraphicsItem() noexcept;

        // Setters
        void setPosition(const Point& pos) noexcept;
        void setRotation(const Angle& rot) noexcept;
        void setText(const QString& text) noexcept;
        void setHeight(const Length& height) noexcept;
        void setAlignment(const Alignment& align) noexcept;
        void setFont(Font font) noexcept;
        void setLayer(const Layer* layer) noexcept;

        // Inherited from IF_LayerObserver
        void layerColorChanged(const Layer& layer, const QColor& newColor) noexcept override;
        void layerHighlightColorChanged(const Layer& layer, const QColor& newColor) noexcept override;
        void layerFillStyleChanged(const Layer& layer, const Qt::BrushStyle& newStyle) noexcept override;
        void layerVisibilityChanged(const Layer& layer, bool newVisible) noexcept override;

        // Inherited from QGraphicsItem
        QRectF boundingRect() const noexcept override {return mBoundingRect;}
        QPainterPath shape() const noexcept override {return mShape;}
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) noexcept override;

        // Operator Overloadings
        TextGraphicsItem& operator=(const TextGraphicsItem& rhs) = delete;


    private: // Methods
        void updateBoundingRectAndShape() noexcept;


    private: // Data
        const Layer* mLayer;
        QString mText;
        Alignment mAlignment;
        QFont mFont;
        QPen mPen;
        QPen mPenHighlighted;
        int mTextFlags;
        QRectF mBoundingRect;
        QPainterPath mShape;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_TEXTGRAPHICSITEM_H
