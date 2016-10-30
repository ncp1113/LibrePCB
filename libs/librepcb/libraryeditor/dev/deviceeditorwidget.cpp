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
#include "deviceeditorwidget.h"
#include "ui_deviceeditorwidget.h"
#include <librepcb/library/dev/device.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

DeviceEditorWidget::DeviceEditorWidget(workspace::Workspace& ws,
        LibraryEditor& editor, const FilePath& fp, QWidget* parent) throw (Exception) :
    EditorWidgetBase(ws, editor, fp, parent), mUi(new Ui::DeviceEditorWidget)
{
    mUi->setupUi(this);
    setWindowIcon(QIcon(":/img/library/device.png"));

    // insert category list editor widget
    mCategoriesEditorWidget.reset(new ComponentCategoryListEditorWidget(mWorkspace, this));
    int row;
    QFormLayout::ItemRole role;
    mUi->formLayout->getWidgetPosition(mUi->lblCategories, &row, &role);
    mUi->formLayout->setWidget(row, QFormLayout::FieldRole, mCategoriesEditorWidget.data());

    mDevice.reset(new Device(fp, false)); // can throw
    setWindowTitle(mDevice->getName(mLocaleOrder));
    mUi->lblUuid->setText(QString("<a href=\"%1\">%2</a>").arg(
        mDevice->getFilePath().toQUrl().toString(), mDevice->getUuid().toStr()));
    mUi->lblUuid->setToolTip(mDevice->getFilePath().toNative());
    mUi->edtName->setText(mDevice->getName(mLocaleOrder));
    mUi->edtDescription->setText(mDevice->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mDevice->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mDevice->getAuthor());
    mUi->edtVersion->setText(mDevice->getVersion().toStr());
    mCategoriesEditorWidget->setUuids(mDevice->getCategories());
    mUi->cbxDeprecated->setChecked(mDevice->isDeprecated());
}

DeviceEditorWidget::~DeviceEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool DeviceEditorWidget::save() noexcept
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

        mDevice->setName("en_US", name);
        mDevice->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mDevice->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mDevice->setAuthor(mUi->edtAuthor->text().trimmed());
        mDevice->setVersion(version);
        mDevice->setCategories(mCategoriesEditorWidget->getUuids());
        mDevice->setDeprecated(mUi->cbxDeprecated->isChecked());
        mDevice->save();
        return EditorWidgetBase::save();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Save failed"), e.getUserMsg());
        return false;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
