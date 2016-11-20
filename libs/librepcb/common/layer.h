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

#ifndef LIBREPCB_LAYER_H
#define LIBREPCB_LAYER_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include "fileio/if_xmlserializableobject.h"
#include "if_layerobserver.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class Layer
 ****************************************************************************************/

/**
 * @brief The Layer class represents a graphical layer used in schematics and boards
 *
 * These layers are used in graphics items (QGraphicsItem) to determine their visibility
 * and colors.
 *
 * @see librepcb::SchematicLayer, librepcb::BoardLayer
 *
 * @todo    Define our own type for the fill stype (#mFillStyle) because we don't support
 *          all values of Qt::BrushStyle.
 * @todo    Support loading/saving the fill style (mFillStyle) from/to XML files.
 * @todo    Remove the signal attributesChanged(). Use IF_LayerObserver instead.
 *
 * @author ubruhin
 * @date 2016-11-04
 */
class Layer : public QObject, public IF_XmlSerializableObject
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        Layer() = delete;
        explicit Layer(const Layer& other) noexcept;
        explicit Layer(const XmlDomElement& domElement) throw (Exception);
        virtual ~Layer() noexcept;

        // Getters
        int getId() const noexcept {return mId;}
        const QString& getName() const noexcept {return mName;}
        const QColor& getColor(bool highlighted = false) const noexcept {
            return highlighted ? mColorHighlighted : mColor;
        }
        Qt::BrushStyle getFillStyle() const noexcept {return mFillStyle;}
        bool isVisible() const noexcept {return mIsVisible;}

        // Setters
        void setVisible(bool visible) noexcept;

        // General Methods
        void registerObserver(IF_LayerObserver& object) const noexcept;
        void unregisterObserver(IF_LayerObserver& object) const noexcept;

        /// @copydoc IF_XmlSerializableObject#serializeToXmlDomElement()
        virtual XmlDomElement* serializeToXmlDomElement() const throw (Exception) override;

        // Operator Overloadings
        Layer& operator=(const Layer& rhs) = delete;


    signals:

        void attributesChanged();


    protected: // Methods
        Layer(int id, const QString& name, const QColor& color,
              const QColor& colorHighlighted, Qt::BrushStyle fillStyle, bool isVisible) noexcept;


    private: // Methods

        /// @copydoc IF_XmlSerializableObject#checkAttributesValidity()
        virtual bool checkAttributesValidity() const noexcept override;


    protected: // Data
        int mId;                    ///< Unique identifier of the layer (must be > 0)
        QString mName;              ///< Layer name (translated into the user's language)
        QColor mColor;              ///< Color of graphics items on that layer
        QColor mColorHighlighted;   ///< Color of hightlighted graphics items on that layer
        Qt::BrushStyle mFillStyle;  ///< Fill style of areas
        bool mIsVisible;            ///< Visibility of graphics items on that layer
        mutable QSet<IF_LayerObserver*> mObservers; ///< A list of all observer objects
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_LAYER_H
