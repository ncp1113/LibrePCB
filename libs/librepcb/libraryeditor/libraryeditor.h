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

#ifndef LIBREPCB_LIBRARY_LIBRARYEDITOR_H
#define LIBREPCB_LIBRARY_LIBRARYEDITOR_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/exceptions.h>
#include <librepcb/common/if_schematiclayerprovider.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

namespace workspace {
class Workspace;
}

namespace library {
namespace editor {

namespace Ui {
class LibraryEditor;
}

/*****************************************************************************************
 *  Class LibraryEditor
 ****************************************************************************************/

/**
 * @brief The LibraryEditor class
 *
 * @todo this is only a stub class...
 */
class LibraryEditor final : public QMainWindow, public IF_SchematicLayerProvider
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        explicit LibraryEditor(workspace::Workspace& workspace) throw (Exception);
        ~LibraryEditor() noexcept;

        // Getters

        /**
         * @copydoc librepcb::IF_SchematicLayerProvider::getSchematicLayer()
         */
        SchematicLayer* getSchematicLayer(int id) const noexcept override
        {Q_UNUSED(id); return nullptr;} // TODO


    private:

        // make some methods inaccessible...
        LibraryEditor(const LibraryEditor& other);
        LibraryEditor& operator=(const LibraryEditor& rhs);

        // Attributes
        workspace::Workspace& mWorkspace;
        Ui::LibraryEditor* mUi;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_LIBRARYEDITOR_H
