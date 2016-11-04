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
#include <QtWidgets>
#include "layer.h"
#include "fileio/xmldomelement.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

Layer::Layer(const Layer& other) noexcept :
    QObject(nullptr), mId(other.mId), mName(other.mName), mColor(other.mColor),
    mColorHighlighted(other.mColorHighlighted), mFillStyle(other.mFillStyle),
    mIsVisible(other.mIsVisible)
{
    Q_ASSERT(mId > 0);
}

Layer::Layer(const XmlDomElement& domElement) throw (Exception) :
    QObject(nullptr),
    mId(domElement.getAttribute<uint>("id", true)),
    mName(domElement.getText<QString>(true)),
    mColor(domElement.getAttribute<QColor>("color", true)),
    mColorHighlighted(domElement.getAttribute<QColor>("color_hl", true)),
    mFillStyle(Qt::SolidPattern), // TODO
    mIsVisible(domElement.getAttribute<bool>("visible", true))
{
    Q_ASSERT(mId > 0);
}

Layer::Layer(int id, const QString& name, const QColor& color,
             const QColor& colorHighlighted, Qt::BrushStyle fillStyle, bool isVisible) noexcept :
    QObject(nullptr), mId(id), mName(name), mColor(color),
    mColorHighlighted(colorHighlighted), mFillStyle(fillStyle), mIsVisible(isVisible)
{
    Q_ASSERT(mId > 0);
}

Layer::~Layer() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void Layer::setVisible(bool visible) noexcept
{
    mIsVisible = visible;
    emit attributesChanged();
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

XmlDomElement* Layer::serializeToXmlDomElement() const throw (Exception)
{
    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);

    QScopedPointer<XmlDomElement> root(new XmlDomElement("layer"));
    root->setAttribute("id", mId);
    root->setText(mName);
    root->setAttribute("color", mColor);
    root->setAttribute("color_hl", mColorHighlighted);
    root->setAttribute("visible", mIsVisible);
    return root.take();
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool Layer::checkAttributesValidity() const noexcept
{
    if (mId <= 0)           return false;
    if (mName.isEmpty())    return false;
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
