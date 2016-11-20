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

#ifndef LIBREPCB_IF_LAYEROBSERVER_H
#define LIBREPCB_IF_LAYEROBSERVER_H

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
 *  Interface IF_LayerObserver
 ****************************************************************************************/

/**
 * @brief The IF_LayerObserver class
 *
 * @author ubruhin
 * @date 2016-11-06
 */
class IF_LayerObserver
{
    public:

        virtual void layerColorChanged(const Layer& layer, const QColor& newColor) noexcept = 0;
        virtual void layerHighlightColorChanged(const Layer& layer, const QColor& newColor) noexcept = 0;
        virtual void layerFillStyleChanged(const Layer& layer, const Qt::BrushStyle& newStyle) noexcept = 0;
        virtual void layerVisibilityChanged(const Layer& layer, bool newVisible) noexcept = 0;


    protected:
        IF_LayerObserver() noexcept {}
        explicit IF_LayerObserver(const IF_LayerObserver& other) = delete;
        virtual ~IF_LayerObserver() noexcept {}
        IF_LayerObserver& operator=(const IF_LayerObserver& rhs) = delete;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_IF_LAYEROBSERVER_H
