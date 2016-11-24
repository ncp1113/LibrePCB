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

#ifndef LIBREPCB_LIBRARY_SYMBOLPINGRAPHICSITEM_H
#define LIBREPCB_LIBRARY_SYMBOLPINGRAPHICSITEM_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/units/all_length_units.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class IF_SchematicLayerProvider;
class EllipseGraphicsItem;
class LineGraphicsItem;
class TextGraphicsItem;

namespace library {

class SymbolPin;

/*****************************************************************************************
 *  Class SymbolPinGraphicsItem
 ****************************************************************************************/

/**
 * @brief The SymbolPinGraphicsItem class
 *
 * @author ubruhin
 * @date 2016-11-05
 */
class SymbolPinGraphicsItem final : public QGraphicsItemGroup
{
    public:

        // Constructors / Destructor
        SymbolPinGraphicsItem() = delete;
        SymbolPinGraphicsItem(const SymbolPinGraphicsItem& other) = delete;
        SymbolPinGraphicsItem(const SymbolPin& pin,
                              const IF_SchematicLayerProvider& layerProvider,
                              QGraphicsItem* parent = nullptr) noexcept;
        ~SymbolPinGraphicsItem() noexcept;

        // Setters
        void setPosition(const Point& pos) noexcept;
        void setRotation(const Angle& rot) noexcept;
        void setLength(const Length& length) noexcept;
        void setName(const QString& name) noexcept;

        // Inherited from QGraphicsItem
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) noexcept override;
        QPainterPath shape() const noexcept override;

        // Operator Overloadings
        SymbolPinGraphicsItem& operator=(const SymbolPinGraphicsItem& rhs) = delete;


    private: // Methods
        void updateShape() noexcept;


    private: // Data
        const SymbolPin& mPin;
        QScopedPointer<EllipseGraphicsItem> mCircleGraphicsItem;
        QScopedPointer<LineGraphicsItem> mLineGraphicsItem;
        QScopedPointer<TextGraphicsItem> mTextGraphicsItem;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_SYMBOLPINGRAPHICSITEM_H
