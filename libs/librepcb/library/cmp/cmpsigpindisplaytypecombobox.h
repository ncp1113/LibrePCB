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

#ifndef LIBREPCB_LIBRARY_CMPSIGPINDISPLAYTYPECOMBOBOX_H
#define LIBREPCB_LIBRARY_CMPSIGPINDISPLAYTYPECOMBOBOX_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include "cmpsigpindisplaytype.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Class CmpSigPinDisplayTypeComboBox
 ****************************************************************************************/

/**
 * @brief The CmpSigPinDisplayTypeComboBox class
 *
 * @author ubruhin
 * @date 2016-10-20
 */
class CmpSigPinDisplayTypeComboBox final : public QWidget
{
        Q_OBJECT

    public:
        // Constructors / Destructor
        explicit CmpSigPinDisplayTypeComboBox(QWidget* parent = nullptr) noexcept;
        CmpSigPinDisplayTypeComboBox(const CmpSigPinDisplayTypeComboBox& other) = delete;
        ~CmpSigPinDisplayTypeComboBox() noexcept;


        // Getters
        CmpSigPinDisplayType getCurrentItem() const noexcept;


        // Setters
        void setCurrentItem(const CmpSigPinDisplayType& type) noexcept;


        // Operator Overloadings
        CmpSigPinDisplayTypeComboBox& operator=(const CmpSigPinDisplayTypeComboBox& rhs) = delete;


    private: // Data
        QComboBox* mComboBox;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_CMPSIGPINDISPLAYTYPECOMBOBOX_H
