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
#include "componenteditorwidget.h"
#include "ui_componenteditorwidget.h"
#include <librepcb/common/widgets/signalrolecombobox.h>
#include <librepcb/library/cmp/component.h>
#include "componentsymbolvarianteditorwidget.h"
#include "libraryeditor.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

ComponentEditorWidget::ComponentEditorWidget(workspace::Workspace& ws,
        LibraryEditor& editor, const FilePath& fp, QWidget* parent) throw (Exception) :
    EditorWidgetBase(ws, editor, fp, parent), mUi(new Ui::ComponentEditorWidget)
{
    mUi->setupUi(this);
    setWindowIcon(QIcon(":/img/library/component.png"));
    connect(mUi->tblSignals, &QTableWidget::itemChanged,
            this, &ComponentEditorWidget::tblSignalsItemChanged);

    // insert category list editor widget
    mCategoriesEditorWidget.reset(new ComponentCategoryListEditorWidget(mWorkspace, this));
    int row;
    QFormLayout::ItemRole role;
    mUi->formLayout->getWidgetPosition(mUi->lblCategories, &row, &role);
    mUi->formLayout->setWidget(row, QFormLayout::FieldRole, mCategoriesEditorWidget.data());

    mComponent.reset(new Component(fp, false)); // can throw
    setWindowTitle(mComponent->getName(mLocaleOrder));
    mUi->lblUuid->setText(QString("<a href=\"%1\">%2</a>").arg(
        mComponent->getFilePath().toQUrl().toString(), mComponent->getUuid().toStr()));
    mUi->lblUuid->setToolTip(mComponent->getFilePath().toNative());
    mUi->edtName->setText(mComponent->getName(mLocaleOrder));
    mUi->edtDescription->setText(mComponent->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mComponent->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mComponent->getAuthor());
    mUi->edtVersion->setText(mComponent->getVersion().toStr());
    mCategoriesEditorWidget->setUuids(mComponent->getCategories());
    mUi->cbxDeprecated->setChecked(mComponent->isDeprecated());
    mUi->cbxSchematicOnly->setChecked(mComponent->isSchematicOnly());
    mUi->edtPrefix->setText(mComponent->getDefaultPrefix());
    mUi->edtDefaultValue->setText(mComponent->getDefaultValue(mLocaleOrder));

    // list signals
    mUi->tblSignals->setRowCount(mComponent->getSignalCount());
    for (int i = 0; i < mComponent->getSignalCount(); ++i) {
        ComponentSignal* signal = mComponent->getSignal(i); Q_ASSERT(signal);

        QTableWidgetItem* item = new QTableWidgetItem(signal->getUuid().toStr());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        mUi->tblSignals->setItem(i, 0, item);

        item = new QTableWidgetItem(signal->getName());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        mUi->tblSignals->setItem(i, 1, item);

        SignalRoleComboBox* signalRoleComboBox = new SignalRoleComboBox(this);
        signalRoleComboBox->setCurrentItem(signal->getRole());
        mUi->tblSignals->setCellWidget(i, 2, signalRoleComboBox);

        item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(signal->isRequired() ? Qt::Checked : Qt::Unchecked);
        item->setTextAlignment(Qt::AlignCenter);
        mUi->tblSignals->setItem(i, 3, item);

        item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(signal->isNegated() ? Qt::Checked : Qt::Unchecked);
        item->setTextAlignment(Qt::AlignCenter);
        mUi->tblSignals->setItem(i, 4, item);

        item = new QTableWidgetItem();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(signal->isClock() ? Qt::Checked : Qt::Unchecked);
        item->setTextAlignment(Qt::AlignCenter);
        mUi->tblSignals->setItem(i, 5, item);

        item = new QTableWidgetItem(signal->getForcedNetName());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        mUi->tblSignals->setItem(i, 6, item);
    }
    mUi->tblSignals->sortByColumn(1, Qt::AscendingOrder);
    mUi->tblSignals->resizeColumnsToContents();
    //mUi->tblSignals->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // list symbol variants
    foreach (ComponentSymbolVariant* symbVar, mComponent->getSymbolVariants()) { Q_ASSERT(symbVar);
        ComponentSymbolVariantEditorWidget* widget =
                new ComponentSymbolVariantEditorWidget(mWorkspace, mLibraryEditor,
                                                       *mComponent, *symbVar);
        widget->setIsDefault(symbVar == mComponent->getDefaultSymbolVariant());
        mUi->tabSymbolVariants->addTab(widget, widget->windowTitle());
    }
}

ComponentEditorWidget::~ComponentEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool ComponentEditorWidget::save() noexcept
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

        mComponent->setName("en_US", name);
        mComponent->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mComponent->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mComponent->setAuthor(mUi->edtAuthor->text().trimmed());
        mComponent->setVersion(version);
        mComponent->setCategories(mCategoriesEditorWidget->getUuids());
        mComponent->setDeprecated(mUi->cbxDeprecated->isChecked());
        mComponent->addPrefix(QString(""), mUi->edtPrefix->text());
        mComponent->addDefaultValue("en_US", mUi->edtDefaultValue->text());

        // apply signals
        for (int i = 0; i < mUi->tblSignals->rowCount(); ++i) {
            // get cell items
            QTableWidgetItem* uuidItem = mUi->tblSignals->item(i, 0); Q_ASSERT(uuidItem);
            QTableWidgetItem* nameItem = mUi->tblSignals->item(i, 1); Q_ASSERT(nameItem);
            SignalRoleComboBox* roleComboBox = dynamic_cast<SignalRoleComboBox*>(
                mUi->tblSignals->cellWidget(i, 2)); Q_ASSERT(roleComboBox);
            QTableWidgetItem* requiredItem = mUi->tblSignals->item(i, 3); Q_ASSERT(requiredItem);
            QTableWidgetItem* negatedItem = mUi->tblSignals->item(i, 4); Q_ASSERT(negatedItem);
            QTableWidgetItem* clockItem = mUi->tblSignals->item(i, 5); Q_ASSERT(clockItem);
            QTableWidgetItem* forcedNetNameItem = mUi->tblSignals->item(i, 6); Q_ASSERT(forcedNetNameItem);
            // get values
            Uuid uuid(uuidItem->text());
            QString name(nameItem->text().trimmed());
            SignalRole role(roleComboBox->getCurrentItem());
            bool required(requiredItem->checkState() == Qt::Checked);
            bool negated(negatedItem->checkState() == Qt::Checked);
            bool clock(clockItem->checkState() == Qt::Checked);
            QString forcedNetName(forcedNetNameItem->text().trimmed());
            // check values validity
            library::ComponentSignal* signal = mComponent->getSignalByUuid(uuid);
            if (!signal) {
                throw LogicError(__FILE__, __LINE__);
            }
            if (name.isEmpty()) {
                throw RuntimeError(__FILE__, __LINE__, QString(),
                                   tr("The signal names must not be empty."));
            }
            // apply values
            signal->setName(name);
            signal->setRole(role);
            signal->setIsRequired(required);
            signal->setIsNegated(negated);
            signal->setIsClock(clock);
            signal->setForcedNetName(forcedNetName);
        }
        mComponent->save();
        return EditorWidgetBase::save();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Save failed"), e.getUserMsg());
        return false;
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void ComponentEditorWidget::tblSignalsItemChanged(QTableWidgetItem* item) noexcept
{
    if (item && ((item->column() == 1) || (item->column() == 6))) {
        item->setText(item->text().toUpper());
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
