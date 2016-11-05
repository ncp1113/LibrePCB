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

#ifndef LIBREPCB_LIBRARY_SYMBOLGRAPHICSITEM_H
#define LIBREPCB_LIBRARY_SYMBOLGRAPHICSITEM_H

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

class Uuid;
class IF_SchematicLayerProvider;

namespace library {

class Symbol;
class SymbolPinGraphicsItem;
class SymbolPinPreviewGraphicsItem;

/*****************************************************************************************
 *  Class SymbolGraphicsItem
 ****************************************************************************************/

/**
 * @brief The SymbolGraphicsItem class represents a symbol as a graphical item
 *
 * @author  ubruhin
 * @date    2016-11-02
 */
class SymbolGraphicsItem final : public QGraphicsItemGroup
{
    public:

        // Constructors / Destructor
        SymbolGraphicsItem() = delete;
        SymbolGraphicsItem(const SymbolGraphicsItem& other) = delete;
        explicit SymbolGraphicsItem(const Symbol& symbol,
                                    const IF_SchematicLayerProvider& layerProvider) noexcept;
        ~SymbolGraphicsItem() noexcept;

        // General Methods
        //void setSelectionRect(const Point& p1, const Point& p2) noexcept;

        // Operator Overloadings
        SymbolGraphicsItem& operator=(const SymbolGraphicsItem& rhs) = delete;


    private: // Data
        const Symbol& mSymbol;
        //QHash<Uuid, QSharedPointer<SymbolPinGraphicsItem>> mPinGraphicsItems;
        QHash<Uuid, QSharedPointer<SymbolPinPreviewGraphicsItem>> mPinGraphicsItems;
        QList<QSharedPointer<QGraphicsPathItem>> mPolygonGraphicsItems;
        QList<QSharedPointer<QGraphicsSimpleTextItem>> mTextGraphicsItems;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_SYMBOLGRAPHICSITEM_H
