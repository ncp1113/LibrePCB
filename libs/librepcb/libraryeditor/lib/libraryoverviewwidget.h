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

#ifndef LIBREPCB_LIBRARY_EDITOR_LIBRARYOVERVIEWWIDGET_H
#define LIBREPCB_LIBRARY_EDITOR_LIBRARYOVERVIEWWIDGET_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/exceptions.h>
#include "../common/editorwidgetbase.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {

class Library;

namespace editor {

namespace Ui {
class LibraryOverviewWidget;
}

/*****************************************************************************************
 *  Class LibraryOverviewWidget
 ****************************************************************************************/

/**
 * @brief The LibraryOverviewWidget class
 *
 * @author ubruhin
 * @date 2016-10-08
 */
class LibraryOverviewWidget final : public EditorWidgetBase
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        LibraryOverviewWidget() = delete;
        LibraryOverviewWidget(const LibraryOverviewWidget& other) = delete;
        LibraryOverviewWidget(workspace::Workspace& ws, LibraryEditor& editor,
                              QSharedPointer<Library> lib, QWidget* parent = nullptr) noexcept;
        ~LibraryOverviewWidget() noexcept;

        // Operator Overloadings
        LibraryOverviewWidget& operator=(const LibraryOverviewWidget& rhs) = delete;


    public slots:

        bool save() noexcept override;


    signals:

        void editComponentCategoryTriggered(const FilePath& fp);
        void editPackageCategoryTriggered(const FilePath& fp);
        void editSymbolTriggered(const FilePath& fp);
        void editPackageTriggered(const FilePath& fp);
        void editComponentTriggered(const FilePath& fp);
        void editDeviceTriggered(const FilePath& fp);


    private: // Methods

        void updateElementLists() noexcept;
        template <typename ElementType>
        void updateElementList(QListWidget& listWidget, const QString& icon) noexcept;

        // Event Handlers
        void lstCmpCatDoubleClicked(const QModelIndex& index) noexcept;
        void lstPkgCatDoubleClicked(const QModelIndex& index) noexcept;
        void lstSymDoubleClicked(const QModelIndex& index) noexcept;
        void lstPkgDoubleClicked(const QModelIndex& index) noexcept;
        void lstCmpDoubleClicked(const QModelIndex& index) noexcept;
        void lstDevDoubleClicked(const QModelIndex& index) noexcept;


    private: // Data

        QSharedPointer<Library> mLibrary;
        QScopedPointer<Ui::LibraryOverviewWidget> mUi;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_LIBRARYOVERVIEWWIDGET_H
