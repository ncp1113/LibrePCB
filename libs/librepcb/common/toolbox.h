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

#ifndef LIBREPCB_TOOLBOX_H
#define LIBREPCB_TOOLBOX_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class CategoryListEditorWidget
 ****************************************************************************************/

/**
 * @brief The Toolbox class provides some static useful general purpose methods
 *
 * @author  ubruhin
 * @date    2016-10-30
 */
class Toolbox final
{
    public:

        // Constructors / Destructor
        Toolbox() = delete;
        Toolbox(const Toolbox& other) = delete;
        ~Toolbox() = delete;

        // Operator Overloadings
        Toolbox& operator=(const Toolbox& rhs) = delete;

        // Static Methods
        template <typename T>
        static QList<T> sortedQSet(const QSet<T>& set) noexcept {
            QList<T> list = set.toList();
            qSort(list);
            return list;
        }

        static QRectF boundingRectFromRadius(qreal radius) noexcept {
            return QRectF(-radius, -radius, 2*radius, 2*radius);
        }

        static QRectF boundingRectFromRadius(qreal rx, qreal ry) noexcept {
            return QRectF(-rx, -ry, 2*rx, 2*ry);
        }

        static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen) noexcept;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_TOOLBOX_H
