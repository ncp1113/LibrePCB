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

#ifndef LIBREPCB_LIBRARY_EDITOR_EDITORWIDGETBASE_H
#define LIBREPCB_LIBRARY_EDITOR_EDITORWIDGETBASE_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/exceptions.h>
#include <librepcb/common/undostack.h>
#include <librepcb/common/fileio/filepath.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

namespace workspace {
class Workspace;
}

namespace library {
namespace editor {

class LibraryEditor;

/*****************************************************************************************
 *  Class EditorWidgetBase
 ****************************************************************************************/

/**
 * @brief The EditorWidgetBase class
 *
 * @author ubruhin
 * @date 2016-10-17
 */
class EditorWidgetBase : public QWidget
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        EditorWidgetBase() = delete;
        EditorWidgetBase(const EditorWidgetBase& other) = delete;
        explicit EditorWidgetBase(workspace::Workspace& ws, LibraryEditor& editor,
                                  const FilePath& fp, QWidget* parent = nullptr) throw (Exception);
        virtual ~EditorWidgetBase() noexcept;

        // Getters
        const FilePath& getFilePath() const noexcept {return mFilePath;}

        /**
         * @copydoc librepcb::UndoStack::getUndoText()
         */
        QString getUndoText() const noexcept;

        /**
         * @copydoc librepcb::UndoStack::getRedoText()
         */
        QString getRedoText() const noexcept;

        /**
         * @copydoc librepcb::UndoStack::canUndo()
         */
        bool canUndo() const noexcept;

        /**
         * @copydoc librepcb::UndoStack::canRedo()
         */
        bool canRedo() const noexcept;

        bool hasChanges() const noexcept;

        // Operator Overloadings
        EditorWidgetBase& operator=(const EditorWidgetBase& rhs) = delete;


    public slots:

        virtual bool save() noexcept;
        virtual void undo() noexcept;
        virtual void redo() noexcept;


    signals:

        void undoTextChanged(const QString& text);
        void redoTextChanged(const QString& text);
        void canUndoChanged(bool canUndo);
        void canRedoChanged(bool canRedo);


    protected: // Data

        workspace::Workspace& mWorkspace;
        LibraryEditor& mLibraryEditor;
        QScopedPointer<UndoStack> mUndoStack;
        FilePath mFilePath;
        const QStringList& mLocaleOrder;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_EDITORWIDGETBASE_H
