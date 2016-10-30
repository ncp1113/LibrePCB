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

#ifndef LIBREPCB_LIBRARY_PACKAGEPAD_H
#define LIBREPCB_LIBRARY_PACKAGEPAD_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <librepcb/common/uuid.h>
#include <librepcb/common/fileio/if_xmlserializableobject.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Class PackagePad
 ****************************************************************************************/

/**
 * @brief The PackagePad class represents one logical pad of a package
 *
 * Following information is considered as the "interface" of a pad and must therefore
 * never be changed:
 *  - UUID
 */
class PackagePad final : public IF_XmlSerializableObject
{
        Q_DECLARE_TR_FUNCTIONS(PackagePad)

    public:

        // Constructors / Destructor
        PackagePad() = delete;
        PackagePad(const PackagePad& other) = delete;
        PackagePad(const Uuid& uuid, const QString& name) noexcept;
        explicit PackagePad(const XmlDomElement& domElement) throw (Exception);
        ~PackagePad() noexcept;

        // Getters
        const Uuid& getUuid() const noexcept {return mUuid;}
        QString getName() const noexcept {return mName;}

        // Setters
        void setName(const QString& name) noexcept;

        // General Methods

        /// @copydoc IF_XmlSerializableObject#serializeToXmlDomElement()
        XmlDomElement* serializeToXmlDomElement() const throw (Exception) override;

        // Operator Overloadings
        PackagePad& operator=(const PackagePad& rhs) = delete;


    private: // Methods

        /// @copydoc IF_XmlSerializableObject#checkAttributesValidity()
        bool checkAttributesValidity() const noexcept override;


    private: // Data
        Uuid mUuid;
        QString mName;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_PACKAGEPAD_H
