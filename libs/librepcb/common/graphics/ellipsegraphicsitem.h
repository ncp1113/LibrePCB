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

#ifndef LIBREPCB_ELLIPSEGRAPHICSITEM_H
#define LIBREPCB_ELLIPSEGRAPHICSITEM_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include "../units/length.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class Ellipse;
class IF_LayerProvider;

/*****************************************************************************************
 *  Class EllipseGraphicsItem
 ****************************************************************************************/

/**
 * @brief The EllipseGraphicsItem class is the graphical representation of an ellips
 *
 * @todo replace IF_SchematicLayerProvider with a generic IF_LayerProvider!
 *
 * @author  ubruhin
 * @date    2016-11-03
 */

class EllipseGraphicsItem final : public QGraphicsItem
{
    public:

        // Constructors / Destructor
        EllipseGraphicsItem() = delete;
        EllipseGraphicsItem(const EllipseGraphicsItem& other) = delete;
        EllipseGraphicsItem(const Ellipse& ellipse, const IF_LayerProvider& layerProvider) noexcept;
        ~EllipseGraphicsItem() noexcept;

        // General Methods
        void updateCacheAndRepaint() noexcept;

        // Inherited from QGraphicsItem
        QRectF boundingRect() const noexcept override {return mBoundingRect;}
        QPainterPath shape() const noexcept override {return mShape;}
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) noexcept override;

        // Operator Overloadings
        EllipseGraphicsItem& operator=(const EllipseGraphicsItem& rhs) = delete;


    private: // Data
        const Ellipse& mEllipse;
        const IF_LayerProvider& mLayerProvider;
        QPen mPen;
        QPen mPenHighlighted;
        QBrush mBrush;
        QBrush mBrushHighlighted;
        QRectF mEllipseRect;
        QRectF mBoundingRect;
        QPainterPath mShape;
};

} // namespace librepcb

#endif // LIBREPCB_ELLIPSEGRAPHICSITEM_H
