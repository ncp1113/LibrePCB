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
#include "componentsignal.h"
#include "component.h"
#include <librepcb/common/fileio/xmldomelement.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

ComponentSignal::ComponentSignal(const Uuid& uuid, const QString& name) noexcept :
    mUuid(uuid), mName(name), mRole(SignalRole::passive()), mForcedNetName(),
    mIsRequired(false), mIsNegated(false), mIsClock(false)
{
    Q_ASSERT(mUuid.isNull() == false);
}

ComponentSignal::ComponentSignal(const XmlDomElement& domElement) throw (Exception)
{
    // read attributes
    mUuid = domElement.getAttribute<Uuid>("uuid", true);
    mName = domElement.getText<QString>(true);
    mRole = domElement.getAttribute<SignalRole>("role", true);
    mForcedNetName = domElement.getAttribute<QString>("forced_net_name", false);
    mIsRequired = domElement.getAttribute<bool>("required", true);
    mIsNegated = domElement.getAttribute<bool>("negated", true);
    mIsClock = domElement.getAttribute<bool>("clock", true);

    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);
}

ComponentSignal::~ComponentSignal() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

XmlDomElement* ComponentSignal::serializeToXmlDomElement() const throw (Exception)
{
    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);

    QScopedPointer<XmlDomElement> root(new XmlDomElement("signal"));
    root->setAttribute("uuid", mUuid);
    root->setAttribute("role", mRole);
    root->setAttribute("forced_net_name", mForcedNetName);
    root->setAttribute("required", mIsRequired);
    root->setAttribute("negated", mIsNegated);
    root->setAttribute("clock", mIsClock);
    root->setText(mName);
    return root.take();
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool ComponentSignal::checkAttributesValidity() const noexcept
{
    if (mUuid.isNull())     return false;
    if (mName.isEmpty())    return false;
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
