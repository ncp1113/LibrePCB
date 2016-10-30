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
#include "packageeditorwidget.h"
#include "ui_packageeditorwidget.h"
#include <librepcb/common/gridproperties.h>
#include <librepcb/common/graphics/graphicsscene.h>
#include <librepcb/library/pkg/package.h>
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

PackageEditorWidget::PackageEditorWidget(workspace::Workspace& ws, LibraryEditor& editor,
        const FilePath& fp, QWidget* parent) throw (Exception) :
    EditorWidgetBase(ws, editor, fp, parent), mUi(new Ui::PackageEditorWidget),
    mGridProperties(new GridProperties()), mGraphicsScene(new GraphicsScene())
{
    mUi->setupUi(this);
    mUi->graphicsView->setUseOpenGl(mWorkspace.getSettings().getAppearance().getUseOpenGl());
    mUi->graphicsView->setGridProperties(*mGridProperties);
    mUi->graphicsView->setScene(mGraphicsScene.data());
    mUi->graphicsView->setBackgroundBrush(Qt::black);
    mUi->graphicsView->setForegroundBrush(Qt::white);
    setWindowIcon(QIcon(":/img/library/package.png"));

    // insert category list editor widget
    mCategoriesEditorWidget.reset(new PackageCategoryListEditorWidget(mWorkspace, this));
    int row;
    QFormLayout::ItemRole role;
    mUi->formLayout->getWidgetPosition(mUi->lblCategories, &row, &role);
    mUi->formLayout->setWidget(row, QFormLayout::FieldRole, mCategoriesEditorWidget.data());

    mPackage.reset(new Package(fp, false)); // can throw
    setWindowTitle(mPackage->getName(mLocaleOrder));
    mUi->lblUuid->setText(QString("<a href=\"%1\">%2</a>").arg(
        mPackage->getFilePath().toQUrl().toString(), mPackage->getUuid().toStr()));
    mUi->lblUuid->setToolTip(mPackage->getFilePath().toNative());
    mUi->edtName->setText(mPackage->getName(mLocaleOrder));
    mUi->edtDescription->setText(mPackage->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mPackage->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mPackage->getAuthor());
    mUi->edtVersion->setText(mPackage->getVersion().toStr());
    mCategoriesEditorWidget->setUuids(mPackage->getCategories());
    mUi->cbxDeprecated->setChecked(mPackage->isDeprecated());
}

PackageEditorWidget::~PackageEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool PackageEditorWidget::save() noexcept
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

        mPackage->setName("en_US", name);
        mPackage->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mPackage->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mPackage->setAuthor(mUi->edtAuthor->text().trimmed());
        mPackage->setVersion(version);
        mPackage->setCategories(mCategoriesEditorWidget->getUuids());
        mPackage->setDeprecated(mUi->cbxDeprecated->isChecked());
        mPackage->save();
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
