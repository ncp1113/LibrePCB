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

#ifndef LIBREPCB_SCHEMATICLAYER_H
#define LIBREPCB_SCHEMATICLAYER_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include "layer.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class SchematicLayer
 ****************************************************************************************/

/**
 * @brief The SchematicLayer class
 */
class SchematicLayer final : public Layer
{
        Q_OBJECT

    public:

        /// All Schematic Layer IDs
        enum LayerID {

            // General
            Grid                                    = 1,
            OriginCrosses                           = 2,

            // Symbols
            SymbolOutlines                          = 10,
            SymbolGrabAreas                         = 11,
            SymbolOptionalPinCircles                = 12,
            SymbolRequiredPinCircles                = 13,
            SymbolPinNames                          = 14,

            // Symbols in a Schematic
            ComponentNames                          = 20,
            ComponentValues                         = 21,
            NetLabels                               = 22,

            // Circuit Stuff in a Schematic
            Nets                                    = 30,
            Busses                                  = 31,

#ifdef QT_DEBUG
            // IDs 900-999: debug layers (for developers)
            DEBUG_GraphicsItemsBoundingRect         = 900,
            DEBUG_GraphicsItemsTextsBoundingRect    = 901,
            DEBUG_SymbolPinNetSignalNames           = 902,
            DEBUG_NetLinesNetSignalNames            = 903,
            DEBUG_InvisibleNetPoints                = 904,
            DEBUG_ComponentSymbolsCount             = 905,
#endif

            // Begin of User defined Layers
            UserDefinedBaseId                       = 1000
        };

        // Constructors / Destructor
        SchematicLayer() = delete;
        explicit SchematicLayer(const SchematicLayer& other) noexcept;
        explicit SchematicLayer(const XmlDomElement& domElement) throw (Exception);
        explicit SchematicLayer(int id) noexcept;
        ~SchematicLayer() noexcept;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_SCHEMATICLAYER_H
