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

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "componentpinsignalmapitem.h"
#include <librepcb/common/fileio/xmldomelement.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

ComponentPinSignalMapItem::ComponentPinSignalMapItem(const Uuid& pin, const Uuid& signal,
                                                     const CmpSigPinDisplayType& displayType) noexcept :
    mPinUuid(pin), mSignalUuid(signal), mDisplayType(displayType)
{
}

ComponentPinSignalMapItem::ComponentPinSignalMapItem(const XmlDomElement& domElement) throw (Exception)
{
    // read attributes
    mPinUuid = domElement.getAttribute<Uuid>("pin", true);
    mDisplayType = CmpSigPinDisplayType::fromString(
                       domElement.getAttribute<QString>("display", true));
    mSignalUuid = domElement.getText<Uuid>(false);

    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);
}

ComponentPinSignalMapItem::~ComponentPinSignalMapItem() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

XmlDomElement* ComponentPinSignalMapItem::serializeToXmlDomElement() const throw (Exception)
{
    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);

    QScopedPointer<XmlDomElement> root(new XmlDomElement("map"));
    root->setAttribute("pin", mPinUuid);
    root->setAttribute<QString>("display", mDisplayType.toString());
    root->setText(mSignalUuid);
    return root.take();
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool ComponentPinSignalMapItem::checkAttributesValidity() const noexcept
{
    if (mPinUuid.isNull())  return false;
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
