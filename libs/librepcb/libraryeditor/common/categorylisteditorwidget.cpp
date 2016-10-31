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
#include "categorylisteditorwidget.h"
#include "ui_categorylisteditorwidget.h"
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/library/workspacelibrarydb.h>
#include <librepcb/workspace/settings/workspacesettings.h>
#include "categorychooserdialog.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

template <typename ElementType>
CategoryListEditorWidget<ElementType>::CategoryListEditorWidget(
        const workspace::Workspace& ws, QWidget* parent) noexcept :
    QWidget(parent), mWorkspace(ws), mUi(new Ui::CategoryListEditorWidget)
{
    mUi->setupUi(this);
    connect(mUi->btnAdd, &QPushButton::clicked,
            this, &CategoryListEditorWidget<ElementType>::btnAddClicked);
    connect(mUi->btnRemove, &QPushButton::clicked,
            this, &CategoryListEditorWidget<ElementType>::btnRemoveClicked);
}

template <typename ElementType>
CategoryListEditorWidget<ElementType>::~CategoryListEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::setUuids(const QSet<Uuid>& uuids) noexcept
{
    mUuids = uuids;
    mUi->listWidget->clear();
    foreach (const Uuid& category, mUuids) {
        addItem(category);
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::btnAddClicked() noexcept
{
    CategoryChooserDialog<ElementType> dialog(mWorkspace, this);
    if (dialog.exec() == QDialog::Accepted) {
        Uuid uuid = dialog.getSelectedCategoryUuid();
        if (!mUuids.contains(uuid)) {
            mUuids.insert(uuid);
            addItem(uuid);
        }
    }
}

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::btnRemoveClicked() noexcept
{
    QListWidgetItem* item = mUi->listWidget->currentItem();
    if (item) {
        Uuid uuid(item->data(Qt::UserRole).toString());
        mUuids.remove(uuid);
        delete item;
    }
}

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::addItem(const Uuid& category) noexcept
{
    try {
        QStringList lines;
        if (!category.isNull()) {
            QList<Uuid> parents = getCategoryParents(category);
            parents.prepend(category);
            foreach (const Uuid& parent, parents) {
                FilePath filepath = getLatestCategory(parent); // can throw
                lines.prepend(getCategoryName(filepath)); // can throw
            }
        }
        lines.prepend(tr("Root category"));
        addItem(category, lines);
    } catch (const Exception& e) {
        addItem(category, QString("%1: %2").arg(category.toStr(), e.getUserMsg()));
    }
}

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::addItem(const Uuid& category, const QStringList& lines) noexcept
{
    QString text;
    for (int i = 0; i < lines.count(); ++i) {
        QString line = lines.value(i);
        if (i == 0) {
            text.append(line);
        } else {
            text.append(QString("\n%1⤷ %2").arg(QString(" ").repeated(i * 2), line));
        }
    }
    addItem(category, text);
}

template <typename ElementType>
void CategoryListEditorWidget<ElementType>::addItem(const Uuid& category, const QString& text) noexcept
{
    QListWidgetItem* item = new QListWidgetItem(text, mUi->listWidget);
    item->setData(Qt::UserRole, category.toStr());
}

template <>
FilePath CategoryListEditorWidget<ComponentCategory>::getLatestCategory(const Uuid& category) const throw (Exception)
{
    return mWorkspace.getLibraryDb().getLatestComponentCategory(category);
}

template <>
FilePath CategoryListEditorWidget<PackageCategory>::getLatestCategory(const Uuid& category) const throw (Exception)
{
    return mWorkspace.getLibraryDb().getLatestPackageCategory(category);
}

template <>
QList<Uuid> CategoryListEditorWidget<ComponentCategory>::getCategoryParents(const Uuid& category) const throw (Exception)
{
    return mWorkspace.getLibraryDb().getComponentCategoryParents(category);
}

template <>
QList<Uuid> CategoryListEditorWidget<PackageCategory>::getCategoryParents(const Uuid& category) const throw (Exception)
{
    return mWorkspace.getLibraryDb().getComponentCategoryParents(category);
}

template <typename ElementType>
QString CategoryListEditorWidget<ElementType>::getCategoryName(const FilePath& fp) const throw (Exception)
{
    QString name;
    mWorkspace.getLibraryDb().template getElementTranslations<ElementType>(fp,
        mWorkspace.getSettings().getLibLocaleOrder().getLocaleOrder(), &name); // can throw
    return name;
}

/*****************************************************************************************
 *  Explicit template instantiations
 ****************************************************************************************/
template class CategoryListEditorWidget<library::ComponentCategory>;
template class CategoryListEditorWidget<library::PackageCategory>;

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
