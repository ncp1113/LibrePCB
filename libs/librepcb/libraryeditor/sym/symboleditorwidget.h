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

#ifndef LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORWIDGET_H
#define LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORWIDGET_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/exceptions.h>
#include <librepcb/common/fileio/filepath.h>
#include <librepcb/common/graphics/if_graphicsvieweventhandler.h>
#include "../common/editorwidgetbase.h"
#include "../common/categorylisteditorwidget.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class GridProperties;
class GraphicsScene;

namespace library {

class Symbol;
class SymbolGraphicsItem;

namespace editor {

class SymbolEditorFsm;

namespace Ui {
class SymbolEditorWidget;
}

/*****************************************************************************************
 *  Class SymbolEditorWidget
 ****************************************************************************************/

/**
 * @brief The SymbolEditorWidget class
 *
 * @author ubruhin
 * @date 2016-10-16
 */
class SymbolEditorWidget final : public EditorWidgetBase, public IF_GraphicsViewEventHandler
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        SymbolEditorWidget() = delete;
        SymbolEditorWidget(const SymbolEditorWidget& other) = delete;
        SymbolEditorWidget(workspace::Workspace& ws, LibraryEditor& editor,
                           const FilePath& fp, QWidget* parent = nullptr) throw (Exception);
        ~SymbolEditorWidget() noexcept;

        // Operator Overloadings
        SymbolEditorWidget& operator=(const SymbolEditorWidget& rhs) = delete;


    public slots:

        bool save() noexcept override;
        void rotateCw() noexcept override;
        void rotateCcw() noexcept override;
        void remove() noexcept override;
        void abortCommand() noexcept override;
        void startSelecting() noexcept override;
        void startAddingSymbolPins() noexcept override;


    private: // Methods

        /**
         * @copydoc librepcb::IF_GraphicsViewEventHandler::graphicsViewEventHandler()
         */
        bool graphicsViewEventHandler(QEvent* event) noexcept override;


    private: // Data

        QScopedPointer<Ui::SymbolEditorWidget> mUi;
        QScopedPointer<ComponentCategoryListEditorWidget> mCategoriesEditorWidget;
        QScopedPointer<GridProperties> mGridProperties;
        QScopedPointer<GraphicsScene> mGraphicsScene;
        QSharedPointer<Symbol> mSymbol;
        QScopedPointer<SymbolGraphicsItem> mGraphicsItem;
        QSharedPointer<SymbolEditorFsm> mFsm;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORWIDGET_H
