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

#ifndef LIBREPCB_XMLDOMDOCUMENT_H
#define LIBREPCB_XMLDOMDOCUMENT_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QDomDocument>
#include "../exceptions.h"
#include "filepath.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class XmlDomElement;

/*****************************************************************************************
 *  Class XmlDomDocument
 ****************************************************************************************/

/**
 * @brief The XmlDomDocument class represents a XML DOM document with the whole DOM tree
 *
 * @todo Use libxml2 instead of Qt's DOM classes
 * @todo Use XSD schema files to validate the opened XML file (with libxml2)
 * @todo Save the DOM document as canonical XML to file (with libxml2 c14n)
 *
 * @author ubruhin
 * @date 2015-02-01
 */
class XmlDomDocument final
{
        Q_DECLARE_TR_FUNCTIONS(XmlDomDocument)

    public:

        // Constructors / Destructor
        XmlDomDocument() = delete;
        XmlDomDocument(const XmlDomDocument& other) = delete;

        /**
         * @brief Constructor to create a new DOM document with a root element
         *
         * @param root              The root element which will be added to the document.
         *                          The document will take the ownership over the root
         *                          element object!
         */
        explicit XmlDomDocument(XmlDomElement& root) noexcept;

        /**
         * @brief Constructor to create the whole DOM tree from the content of a XML file
         *
         * @param xmlFileContent    The content of the XML file to load
         * @param filepath          The filepath of the XML file (needed for the exceptions)
         *
         * @throw Exception         If parsing the XML file has failed.
         */
        explicit XmlDomDocument(const QByteArray& xmlFileContent, const FilePath& filepath) throw (Exception);

        /**
         * @brief Destructor (destroys the whole DOM tree)
         */
        ~XmlDomDocument() noexcept;


        // Getters

        /**
         * @brief Get the filepath which was passed to the constructor
         *
         * @return The filepath
         */
        const FilePath& getFilePath() const noexcept {return mFilePath;}

        /**
         * @brief Get the root XML DOM element
         *
         * @return The root element of the document
         */
        XmlDomElement& getRoot() const noexcept {Q_ASSERT(mRootElement); return *mRootElement;}

        /**
         * @brief Get the root XML DOM element and check it's tag name
         *
         * @param expectedName  The expected name of the root element. If it differs,
         *                      an exception will be thrown.
         *
         * @return The root element of the document
         */
        XmlDomElement& getRoot(const QString& expectedName) const throw (Exception);


        // General Methods

        /**
         * @brief Export the whole DOM tree as a QByteArray to write back to the XML file
         *
         * @return The XML DOM tree which can be written into an XML file
         */
        QByteArray toByteArray() const noexcept;


        // Operator Overloadings
        XmlDomDocument& operator=(const XmlDomDocument& rhs) = delete;


    private:

        // General
        FilePath mFilePath;                         ///< the filepath from the constructor
        QScopedPointer<XmlDomElement> mRootElement; ///< the root DOM element
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_XMLDOMDOCUMENT_H
