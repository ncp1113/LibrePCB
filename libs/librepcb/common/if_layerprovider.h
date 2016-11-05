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

#ifndef LIBREPCB_IF_LAYERPROVIDER_H
#define LIBREPCB_IF_LAYERPROVIDER_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class Layer;

/*****************************************************************************************
 *  Interface IF_LayerProvider
 ****************************************************************************************/

/**
 * @brief The IF_LayerProvider class defines an interface for classes which provide layers
 *
 * @author ubruhin
 * @date 2016-11-04
 */
class IF_LayerProvider
{
    public:

        /**
         * @brief Get the layer with a specific ID
         *
         * @param id                The ID of the requested layer
         *
         * @retval Layer*           Pointer to the layer with the specified ID
         * @retval nullptr          If no layer with the specified ID is available
         */
        virtual Layer* getLayer(int id) const noexcept = 0;


    protected:

        // Constructors / Destructor
        IF_LayerProvider() noexcept {}
        IF_LayerProvider(const IF_LayerProvider& other) noexcept {Q_UNUSED(other);}
        virtual ~IF_LayerProvider() noexcept {}

        // Operator Overloadings
        IF_LayerProvider& operator=(const IF_LayerProvider& rhs) noexcept {Q_UNUSED(rhs); return *this;}
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_IF_LAYERPROVIDER_H

