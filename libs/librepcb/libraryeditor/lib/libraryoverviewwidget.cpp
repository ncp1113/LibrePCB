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
#include "libraryoverviewwidget.h"
#include "ui_libraryoverviewwidget.h"
#include <librepcb/library/elements.h>
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

LibraryOverviewWidget::LibraryOverviewWidget(workspace::Workspace& ws, LibraryEditor& editor,
        QSharedPointer<Library> lib, QWidget* parent) noexcept:
    EditorWidgetBase(ws, editor, lib->getFilePath(), parent), mLibrary(lib),
    mUi(new Ui::LibraryOverviewWidget)
{
    mUi->setupUi(this);

    if (!mLibrary->getIcon().isNull()) {
        mUi->lblIcon->setPixmap(mLibrary->getIcon().scaledToHeight(mUi->lblIcon->height(),
                                                                   Qt::SmoothTransformation));
    } else {
        mUi->lblIcon->setText(tr("Click here to choose an icon (PNG, 256x256px)."));
    }

    mUi->lblUuid->setText(mLibrary->getUuid().toStr());
    mUi->edtName->setText(mLibrary->getName(mLocaleOrder));
    mUi->edtDescription->setText(mLibrary->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mLibrary->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mLibrary->getAuthor());
    mUi->edtVersion->setText(mLibrary->getVersion().toStr());
    mUi->edtUrl->setText(mLibrary->getUrl().toString());
    mUi->cbxDeprecated->setChecked(mLibrary->isDeprecated());

    updateElementLists();

    connect(mUi->lstCmpCat, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstCmpCatDoubleClicked);
    connect(mUi->lstPkgCat, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstPkgCatDoubleClicked);
    connect(mUi->lstSym, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstSymDoubleClicked);
    connect(mUi->lstPkg, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstPkgDoubleClicked);
    connect(mUi->lstCmp, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstCmpDoubleClicked);
    connect(mUi->lstDev, &QListWidget::doubleClicked, this,
            &LibraryOverviewWidget::lstDevDoubleClicked);
}

LibraryOverviewWidget::~LibraryOverviewWidget()
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool LibraryOverviewWidget::save() noexcept
{
    try {
        QString name = mUi->edtName->text().trimmed();
        if (name.isEmpty()) {
            throw RuntimeError(__FILE__, __LINE__, QString(), tr("The name must not be empty."));
        }
        Version version(mUi->edtVersion->text().trimmed());
        if (!version.isValid()) {
            throw RuntimeError(__FILE__, __LINE__, QString(), tr("The version number is invalid."));
        }

        mLibrary->setName("en_US", name);
        mLibrary->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mLibrary->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mLibrary->setAuthor(mUi->edtAuthor->text().trimmed());
        mLibrary->setVersion(version);
        mLibrary->setDeprecated(mUi->cbxDeprecated->isChecked());
        mLibrary->save();
        return EditorWidgetBase::save();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Save failed"), e.getUserMsg());
        return false;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void LibraryOverviewWidget::updateElementLists() noexcept
{
    updateElementList<ComponentCategory>(*mUi->lstCmpCat, ":/img/places/folder.png");
    updateElementList<PackageCategory>(  *mUi->lstPkgCat, ":/img/places/folder_green.png");
    updateElementList<Symbol>(           *mUi->lstSym,    ":/img/library/symbol.png");
    updateElementList<Package>(          *mUi->lstPkg,    ":/img/library/package.png");
    updateElementList<Component>(        *mUi->lstCmp,    ":/img/library/component.png");
    updateElementList<Device>(           *mUi->lstDev,    ":/img/library/device.png");
}

template <typename ElementType>
void LibraryOverviewWidget::updateElementList(QListWidget& listWidget, const QString& icon) noexcept
{
    listWidget.clear();
    QList<FilePath> list = mLibrary->searchForElements<ElementType>();
    foreach (const FilePath& fp, list) {
        QListWidgetItem* item = new QListWidgetItem(&listWidget);
        try {
            ElementType elem(fp, true);
            item->setText(elem.getName(mLocaleOrder));
            item->setIcon(QIcon(icon));
            item->setData(Qt::UserRole, fp.toStr());
        } catch (const Exception& e) {
            item->setText(e.getUserMsg());
            item->setIcon(QIcon(":/img/status/dialog-error.png"));
            item->setBackground(Qt::red);
            item->setForeground(Qt::white);
        }
    }
}

/*****************************************************************************************
 *  Event Handlers
 ****************************************************************************************/

void LibraryOverviewWidget::lstCmpCatDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstCmpCat->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editComponentCategoryTriggered(fp);
    }
}

void LibraryOverviewWidget::lstPkgCatDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstPkgCat->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editPackageCategoryTriggered(fp);
    }
}

void LibraryOverviewWidget::lstSymDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstSym->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editSymbolTriggered(fp);
    }
}

void LibraryOverviewWidget::lstPkgDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstPkg->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editPackageTriggered(fp);
    }
}

void LibraryOverviewWidget::lstCmpDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstCmp->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editComponentTriggered(fp);
    }
}

void LibraryOverviewWidget::lstDevDoubleClicked(const QModelIndex& index) noexcept
{
    QListWidgetItem* item = mUi->lstDev->item(index.row());
    FilePath fp = item ? FilePath(item->data(Qt::UserRole).toString()) : FilePath();
    if (fp.isValid()) {
        emit editDeviceTriggered(fp);
    }
}


/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library_editor
} // namespace librepcb
