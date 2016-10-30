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
#include "componentcategoryeditorwidget.h"
#include "ui_componentcategoryeditorwidget.h"
#include <librepcb/library/cat/componentcategory.h>
#include <librepcb/workspace/workspace.h>
#include "../common/categorychooserdialog.h"
#include "../common/categorytreelabeltextbuilder.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

ComponentCategoryEditorWidget::ComponentCategoryEditorWidget(workspace::Workspace& ws,
        LibraryEditor& editor, const FilePath& fp, QWidget* parent) throw (Exception) :
    EditorWidgetBase(ws, editor, fp, parent), mUi(new Ui::ComponentCategoryEditorWidget)
{
    mUi->setupUi(this);
    setWindowIcon(QIcon(":/img/places/folder.png"));
    connect(mUi->btnChooseParentCategory, &QToolButton::clicked,
            this, &ComponentCategoryEditorWidget::btnChooseParentCategoryClicked);
    connect(mUi->edtName, &QLineEdit::textChanged,
            this, &ComponentCategoryEditorWidget::edtnameTextChanged);
    connect(mUi->edtParent, &QLineEdit::textChanged,
            this, &ComponentCategoryEditorWidget::edtParentTextChanged);

    mCategory.reset(new ComponentCategory(fp, false)); // can throw
    setWindowTitle(mCategory->getName(mLocaleOrder));
    mUi->lblUuid->setText(QString("<a href=\"%1\">%2</a>").arg(
        mCategory->getFilePath().toQUrl().toString(), mCategory->getUuid().toStr()));
    mUi->lblUuid->setToolTip(mCategory->getFilePath().toNative());
    mUi->edtName->setText(mCategory->getName(mLocaleOrder));
    mUi->edtDescription->setText(mCategory->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mCategory->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mCategory->getAuthor());
    mUi->edtVersion->setText(mCategory->getVersion().toStr());
    mUi->edtParent->setText(mCategory->getParentUuid().toStr());
    mUi->cbxDeprecated->setChecked(mCategory->isDeprecated());
}

ComponentCategoryEditorWidget::~ComponentCategoryEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool ComponentCategoryEditorWidget::save() noexcept
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
        QString parentUuidStr = mUi->edtParent->text().trimmed();
        Uuid parentUuid(parentUuidStr);
        if (parentUuid.isNull() && !parentUuidStr.isEmpty()) {
            throw RuntimeError(__FILE__, __LINE__, QString(), tr("The parent UUID is invalid."));
        }

        mCategory->setName("en_US", name);
        mCategory->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mCategory->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mCategory->setAuthor(mUi->edtAuthor->text().trimmed());
        mCategory->setVersion(version);
        mCategory->setParentUuid(parentUuid);
        mCategory->setDeprecated(mUi->cbxDeprecated->isChecked());
        mCategory->save();
        return EditorWidgetBase::save();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Save failed"), e.getUserMsg());
        return false;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void ComponentCategoryEditorWidget::btnChooseParentCategoryClicked() noexcept
{
    ComponentCategoryChooserDialog dialog(mWorkspace);
    if (dialog.exec()) {
        mUi->edtParent->setText(dialog.getSelectedCategoryUuid().toStr());
    }
}

void ComponentCategoryEditorWidget::edtnameTextChanged(const QString& text) noexcept
{
    // force updating parent categories
    Q_UNUSED(text);
    edtParentTextChanged(mUi->edtParent->text());
}

void ComponentCategoryEditorWidget::edtParentTextChanged(const QString& text) noexcept
{
    const workspace::WorkspaceLibraryDb& db = mWorkspace.getLibraryDb();
    ComponentCategoryTreeLabelTextBuilder textBuilder(db, mLocaleOrder, *mUi->lblParentCategories);
    textBuilder.setEndlessRecursionUuid(mCategory->getUuid());
    textBuilder.setHighlightLastLine(true);

    QString trimmed = text.trimmed();
    Uuid parentUuid(trimmed);
    if ((trimmed.length() > 0) && parentUuid.isNull()) {
        textBuilder.setErrorText(tr("Invalid UUID!"));
    } else {
        textBuilder.updateText(parentUuid, mUi->edtName->text());
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
