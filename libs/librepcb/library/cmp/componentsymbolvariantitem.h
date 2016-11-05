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

#ifndef LIBREPCB_LIBRARY_COMPONENTSYMBOLVARIANTITEM_H
#define LIBREPCB_LIBRARY_COMPONENTSYMBOLVARIANTITEM_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <librepcb/common/uuid.h>
#include <librepcb/common/fileio/if_xmlserializableobject.h>
#include "componentpinsignalmapitem.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Class ComponentSymbolVariantItem
 ****************************************************************************************/

/**
 * @brief The ComponentSymbolVariantItem class represents one symbol of a component symbol
 *        variant
 *
 * Following information is considered as the "interface" of a symbol variant item and
 * must therefore never be changed:
 *  - UUID
 *  - Symbol UUID
 *  - Pin-signal-mapping
 */
class ComponentSymbolVariantItem final : public IF_XmlSerializableObject
{
        Q_DECLARE_TR_FUNCTIONS(ComponentSymbolVariantItem)

    public:

        // Constructors / Destructor
        ComponentSymbolVariantItem() = delete;
        ComponentSymbolVariantItem(const ComponentSymbolVariantItem& other) = delete;
        ComponentSymbolVariantItem(const Uuid& uuid, const Uuid& symbolUuid,
                                   bool isRequired, const QString& suffix) noexcept;
        explicit ComponentSymbolVariantItem(const XmlDomElement& domElement) throw (Exception);
        ~ComponentSymbolVariantItem() noexcept;

        // Getters: Attributes
        const Uuid& getUuid() const noexcept {return mUuid;}
        const Uuid& getSymbolUuid() const noexcept {return mSymbolUuid;}
        bool isRequired() const noexcept {return mIsRequired;}
        const QString& getSuffix() const noexcept {return mSuffix;}

        // Setters: Attributes
        void setIsRequired(bool required) noexcept {mIsRequired = required;}
        void setSuffix(const QString& suffix) noexcept {mSuffix = suffix;}

        // Pin-Signal-Map Methods
        const QMap<Uuid, ComponentPinSignalMapItem*>& getPinSignalMappings() noexcept {return mPinSignalMap;}
        QList<Uuid> getPinUuids() const noexcept {return mPinSignalMap.keys();}
        ComponentPinSignalMapItem* getPinSignalMapItemOfPin(const Uuid& pinUuid) noexcept {return mPinSignalMap.value(pinUuid);}
        const ComponentPinSignalMapItem* getPinSignalMapItemOfPin(const Uuid& pinUuid) const noexcept {return mPinSignalMap.value(pinUuid);}
        void addPinSignalMapItem(ComponentPinSignalMapItem& item) noexcept;
        void removePinSignalMapItem(ComponentPinSignalMapItem& item) noexcept;

        /// @copydoc #IF_XmlSerializableObject#serializeToXmlDomElement()
        XmlDomElement* serializeToXmlDomElement() const throw (Exception) override;

        // Operator Overloadings
        ComponentSymbolVariantItem& operator=(const ComponentSymbolVariantItem& rhs) = delete;


    private: // Methods

        /// @copydoc #IF_XmlSerializableObject#checkAttributesValidity()
        bool checkAttributesValidity() const noexcept override;


    private: // Data
        Uuid mUuid;
        Uuid mSymbolUuid;
        bool mIsRequired;
        QString mSuffix;
        QMap<Uuid, ComponentPinSignalMapItem*> mPinSignalMap; ///< Key: Pin UUID (all pins required!)
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_COMPONENTSYMBOLVARIANTITEM_H
