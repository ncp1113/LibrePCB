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

#ifndef LIBREPCB_LIBRARY_EDITOR_PACKAGEEDITORWIDGET_H
#define LIBREPCB_LIBRARY_EDITOR_PACKAGEEDITORWIDGET_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/exceptions.h>
#include <librepcb/common/fileio/filepath.h>
#include "../common/editorwidgetbase.h"
#include "../common/categorylisteditorwidget.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class GridProperties;
class GraphicsScene;

namespace library {

class Package;

namespace editor {

namespace Ui {
class PackageEditorWidget;
}

/*****************************************************************************************
 *  Class PackageEditorWidget
 ****************************************************************************************/

/**
 * @brief The PackageEditorWidget class
 *
 * @author ubruhin
 * @date 2016-10-16
 */
class PackageEditorWidget final : public EditorWidgetBase
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        PackageEditorWidget() = delete;
        PackageEditorWidget(const PackageEditorWidget& other) = delete;
        PackageEditorWidget(workspace::Workspace& ws, LibraryEditor& editor,
                            const FilePath& fp, QWidget* parent = nullptr) throw (Exception);
        ~PackageEditorWidget() noexcept;

        // Operator Overloadings
        PackageEditorWidget& operator=(const PackageEditorWidget& rhs) = delete;


    public slots:

        bool save() noexcept override;


    private: // Data

        QScopedPointer<Ui::PackageEditorWidget> mUi;
        QScopedPointer<PackageCategoryListEditorWidget> mCategoriesEditorWidget;
        QScopedPointer<GridProperties> mGridProperties;
        QScopedPointer<GraphicsScene> mGraphicsScene;
        QSharedPointer<Package> mPackage;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_PACKAGEEDITORWIDGET_H
