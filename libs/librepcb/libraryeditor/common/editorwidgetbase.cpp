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
#include "editorwidgetbase.h"
#include <librepcb/common/undostack.h>
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/settings/workspacesettings.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

EditorWidgetBase::EditorWidgetBase(workspace::Workspace& ws, LibraryEditor& editor,
                                   const FilePath& fp, QWidget* parent) throw (Exception):
    QWidget(parent), mWorkspace(ws), mLibraryEditor(editor), mFilePath(fp),
    mLocaleOrder(mWorkspace.getSettings().getLibLocaleOrder().getLocaleOrder())
{
    mUndoStack.reset(new UndoStack());
    connect(mUndoStack.data(), &UndoStack::undoTextChanged,
            this, &EditorWidgetBase::undoTextChanged);
    connect(mUndoStack.data(), &UndoStack::redoTextChanged,
            this, &EditorWidgetBase::redoTextChanged);
    connect(mUndoStack.data(), &UndoStack::canUndoChanged,
            this, &EditorWidgetBase::canUndoChanged);
    connect(mUndoStack.data(), &UndoStack::canRedoChanged,
            this, &EditorWidgetBase::canRedoChanged);
}

EditorWidgetBase::~EditorWidgetBase() noexcept
{
}

/*****************************************************************************************
 *  Getters
 ****************************************************************************************/

QString EditorWidgetBase::getUndoText() const noexcept
{
    return mUndoStack->getUndoText();
}

QString EditorWidgetBase::getRedoText() const noexcept
{
    return mUndoStack->getRedoText();
}

bool EditorWidgetBase::canUndo() const noexcept
{
    return mUndoStack->canUndo();
}

bool EditorWidgetBase::canRedo() const noexcept
{
    return mUndoStack->canRedo();
}

bool EditorWidgetBase::hasChanges() const noexcept
{
    return !mUndoStack->isClean();
}

/*****************************************************************************************
 *  Public Methods
 ****************************************************************************************/

bool EditorWidgetBase::save() noexcept
{
    mUndoStack->setClean();
    return true;
}

void EditorWidgetBase::undo() noexcept
{
    try {
        mUndoStack->undo();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Undo failed"), e.getUserMsg());
    }
}

void EditorWidgetBase::redo() noexcept
{
    try {
        mUndoStack->redo();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Redo failed"), e.getUserMsg());
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
