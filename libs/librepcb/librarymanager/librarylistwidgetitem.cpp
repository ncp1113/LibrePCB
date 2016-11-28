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
#include "librarylistwidgetitem.h"
#include "ui_librarylistwidgetitem.h"
#include <librepcb/library/library.h>
#include <librepcb/libraryeditor/libraryeditor.h>
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/settings/workspacesettings.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace manager {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

LibraryListWidgetItem::LibraryListWidgetItem(workspace::Workspace& ws,
                                             QSharedPointer<Library> lib) noexcept :
    QWidget(nullptr), mUi(new Ui::LibraryListWidgetItem), mWorkspace(ws), mLib(lib)
{
    mUi->setupUi(this);

    if (lib) {
        const QStringList& localeOrder = ws.getSettings().getLibLocaleOrder().getLocaleOrder();
        if (!lib->getIcon().isNull()) {
            mUi->lblIcon->setPixmap(lib->getIcon());
        }
        if (isRemoteLibrary()) {
            mUi->lblLibraryType->setText(tr("(remote)"));
            mUi->lblLibraryType->setStyleSheet("QLabel { color: red; }");
        } else {
            mUi->lblLibraryType->setText(tr("(local)"));
            mUi->lblLibraryType->setStyleSheet("QLabel { color: blue; }");
        }
        mUi->lblLibraryName->setText(lib->getName(localeOrder));
        mUi->lblLibraryDescription->setText(lib->getDescription(localeOrder));
        mUi->lblLibraryUrl->setText(lib->getFilePath().toRelative(ws.getLibrariesPath()));
    } else {
        QPixmap image(":/img/actions/add.png");
        mUi->lblIcon->setPixmap(image.scaled(mUi->lblIcon->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
        mUi->lblLibraryType->setVisible(false);
        mUi->lblLibraryName->setText(tr("Add a new library"));
        mUi->lblLibraryDescription->setText(tr("Click here to add a new library."));
        mUi->lblLibraryUrl->setText("");
    }

    if (mUi->lblLibraryDescription->text().isEmpty()) {
        mUi->lblLibraryDescription->setVisible(false);
    }
}

LibraryListWidgetItem::~LibraryListWidgetItem() noexcept
{
}

/*****************************************************************************************
 *  Getters
 ****************************************************************************************/

QString LibraryListWidgetItem::getName() const noexcept
{
    return mUi->lblLibraryName->text();
}

bool LibraryListWidgetItem::isRemoteLibrary() const noexcept
{
    return mLib ? mLib->isOpenedReadOnly() : false;
}

/*****************************************************************************************
 *  Inherited from QWidget
 ****************************************************************************************/

void LibraryListWidgetItem::mouseDoubleClickEvent(QMouseEvent* e) noexcept
{
    if (mLib) {
        try {
            using library::editor::LibraryEditor;
            QScopedPointer<LibraryEditor> editor(new LibraryEditor(mWorkspace, mLib));
            editor->show();
            editor.take(); // release ownership because the editor deletes itself after closing
        } catch (const Exception& e) {
            QMessageBox::critical(this, tr("Error"), e.getUserMsg());
        }
        e->accept();
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace manager
} // namespace library
} // namespace librepcb
