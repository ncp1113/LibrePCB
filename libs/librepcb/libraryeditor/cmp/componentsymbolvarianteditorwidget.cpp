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
#include "componentsymbolvarianteditorwidget.h"
#include "ui_componentsymbolvarianteditorwidget.h"
#include <librepcb/common/graphics/graphicsscene.h>
#include <librepcb/library/cmp/component.h>
#include <librepcb/library/cmp/componentsymbolvariant.h>
#include <librepcb/library/cmp/cmpsigpindisplaytypecombobox.h>
#include <librepcb/library/sym/symbol.h>
#include <librepcb/library/sym/symbolpreviewgraphicsitem.h>
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/library/workspacelibrarydb.h>
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

ComponentSymbolVariantEditorWidget::ComponentSymbolVariantEditorWidget(
        workspace::Workspace& ws, const IF_SchematicLayerProvider& layerProvider,
        Component& cmp, ComponentSymbolVariant& symbVar, QWidget* parent) throw (Exception) :
    QWidget(parent), mWorkspace(ws), mLayerProvider(layerProvider), mComponent(cmp),
    mSymbVar(symbVar), mUi(new Ui::ComponentSymbolVariantEditorWidget),
    mPreviewScene(new GraphicsScene()),
    mLocaleOrder(mWorkspace.getSettings().getLibLocaleOrder().getLocaleOrder())
{
    mUi->setupUi(this);
    mUi->graphicsView->setUseOpenGl(mWorkspace.getSettings().getAppearance().getUseOpenGl());
    mUi->graphicsView->setScene(mPreviewScene.data());
    connect(mUi->btnSetAsDefault, &QPushButton::clicked,
            this, &ComponentSymbolVariantEditorWidget::setAsDefaultRequested);
    connect(mUi->lstSymbolItems, &QListWidget::currentItemChanged,
            this, &ComponentSymbolVariantEditorWidget::lstSymbolItemsCurrentItemChanged);
    connect(mUi->cbxSymbVarItemRequired, &QCheckBox::toggled,
            this, &ComponentSymbolVariantEditorWidget::cbxSymbVarItemRequiredCheckedChanged);
    connect(mUi->edtSymbVarItemSuffix, &QLineEdit::textChanged,
            this, &ComponentSymbolVariantEditorWidget::edtSymbVarItemSuffixTextChanged);

    mUi->lblUuid->setText(mSymbVar.getUuid().toStr());
    mUi->edtName->setText(mSymbVar.getName(mLocaleOrder));
    mUi->edtDescription->setText(mSymbVar.getDescription(mLocaleOrder));
    mUi->edtNorm->setText(mSymbVar.getNorm());
    setIsDefault(false);

    // list symbol items
    for (int i = 0; i < mSymbVar.getItemCount(); ++i) {
        // get symbol variant item
        ComponentSymbolVariantItem* symbVarItem = mSymbVar.getItem(i); Q_ASSERT(symbVarItem);
        // open symbol
        FilePath symFp = mWorkspace.getLibraryDb().getLatestSymbol(symbVarItem->getSymbolUuid());
        QScopedPointer<Symbol> symbol(new Symbol(symFp, true)); // can throw
        // add list item
        QString text = QString("%1: %2 (Suffix: \"%3\")").arg(i)
                       .arg(symbol->getName(mLocaleOrder)).arg(symbVarItem->getSuffix());
        QListWidgetItem* item = new QListWidgetItem(text, mUi->lstSymbolItems);
        item->setData(Qt::UserRole, symbVarItem->getUuid().toStr());
        // list pin-signal-mappings
        if (symbVarItem->getPinSignalMappings().keys().toSet() != symbol->getPinUuids().toSet()) {
            throw RuntimeError(__FILE__, __LINE__, QString(), QString(tr("The pin-signal-map "
                "of the symbol variant item {} does not match with the pins of symbol {}!"))
                .arg(symbVarItem->getUuid().toStr(), symbol->getUuid().toStr()));
        }
        foreach (const ComponentPinSignalMapItem* mapItem, symbVarItem->getPinSignalMappings()) { Q_ASSERT(mapItem);
            // get signal and pin
            SymbolPin* pin = symbol->getPinByUuid(mapItem->getPinUuid());
            if (!pin) {
                throw RuntimeError(__FILE__, __LINE__, QString(), QString(tr(
                    "Symbol pin {} not found!")).arg(mapItem->getPinUuid().toStr()));
            }
            ComponentSignal* signal = mComponent.getSignalByUuid(mapItem->getSignalUuid());
            // append row
            int count = mUi->tblPinSignalMap->rowCount();
            mUi->tblPinSignalMap->setRowCount(count + 1);
            // symbol
            QTableWidgetItem* item = new QTableWidgetItem(symbol->getName(mLocaleOrder));
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            mUi->tblPinSignalMap->setItem(count, 0, item);
            // symbol suffix
            item = new QTableWidgetItem(symbVarItem->getSuffix());
            item->setData(Qt::UserRole, symbVarItem->getUuid().toStr());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            mUi->tblPinSignalMap->setItem(count, 1, item);
            // pin
            item = new QTableWidgetItem(pin->getName());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            mUi->tblPinSignalMap->setItem(count, 2, item);
            // component signal
            /*QComboBox* signalComboBox = new QComboBox(this);
            foreach (const ComponentSignal* sig, mComponent.getSignals()) { Q_ASSERT(sig);
                signalComboBox->addItem(sig->getName(), sig->getUuid().toStr());
                if (sig == signal) {
                    signalComboBox->setCurrentIndex(signalComboBox->count()-1);
                }
            }
            Q_ASSERT(signalComboBox->currentIndex() >= 0);
            mUi->tblPinSignalMap->setCellWidget(count, 3, signalComboBox);*/
            item = new QTableWidgetItem(signal ? signal->getName() : "");
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            mUi->tblPinSignalMap->setItem(count, 3, item);
            // display type
            CmpSigPinDisplayTypeComboBox* displayTypeComboBox = new CmpSigPinDisplayTypeComboBox(this);
            displayTypeComboBox->setCurrentItem(mapItem->getDisplayType());
            mUi->tblPinSignalMap->setCellWidget(count, 4, displayTypeComboBox);
        }
    }
    mUi->lstSymbolItems->setCurrentRow(0);
    mUi->tblPinSignalMap->sortByColumn(2, Qt::AscendingOrder); // pin name
    mUi->tblPinSignalMap->sortByColumn(1, Qt::AscendingOrder); // symbol suffix
    mUi->tblPinSignalMap->resizeColumnsToContents();
    //mUi->tblPinSignalMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ComponentSymbolVariantEditorWidget::~ComponentSymbolVariantEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void ComponentSymbolVariantEditorWidget::setIsDefault(bool isDefault) noexcept
{
    if (isDefault) {
        mUi->btnSetAsDefault->setText(tr("Is already default"));
        mUi->btnSetAsDefault->setEnabled(false);
        setWindowTitle(QString("%1 [default]").arg(mSymbVar.getName(mLocaleOrder)));
    } else {
        mUi->btnSetAsDefault->setText(tr("Set as default"));
        mUi->btnSetAsDefault->setEnabled(true);
        setWindowTitle(mSymbVar.getName(mLocaleOrder));
    }
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void ComponentSymbolVariantEditorWidget::lstSymbolItemsCurrentItemChanged(
        QListWidgetItem* current, QListWidgetItem* previous) noexcept
{
    Q_UNUSED(previous);

    mPreviewSymbol.reset();
    mSymbolPreviewGraphicsItem.reset();
    mUi->lblSymbVarItemSymbolUuid->setText("");
    mUi->cbxSymbVarItemRequired->setChecked(false);
    mUi->cbxSymbVarItemRequired->setEnabled(false);
    mUi->edtSymbVarItemSuffix->setText("");
    mUi->edtSymbVarItemSuffix->setEnabled(false);

    if (current) {
        try {
            // get symbol variant item
            Uuid itemUuid = Uuid(current->data(Qt::UserRole).toString()); Q_ASSERT(!itemUuid.isNull());
            ComponentSymbolVariantItem* symbVarItem = mSymbVar.getItemByUuid(itemUuid); Q_ASSERT(symbVarItem);
            if (!symbVarItem) throw LogicError(__FILE__, __LINE__);
            // open symbol
            FilePath symFp = mWorkspace.getLibraryDb().getLatestSymbol(symbVarItem->getSymbolUuid());
            mPreviewSymbol.reset(new Symbol(symFp, true)); // can throw
            // show preview
            mSymbolPreviewGraphicsItem.reset(new SymbolPreviewGraphicsItem(mLayerProvider,
                                                                           mLocaleOrder,
                                                                           *mPreviewSymbol));
            mPreviewScene->addItem(*mSymbolPreviewGraphicsItem);
            mUi->graphicsView->zoomAll();
            // update other widgets
            mUi->lblSymbVarItemSymbolUuid->setText(symbVarItem->getSymbolUuid().toStr());
            mUi->cbxSymbVarItemRequired->setChecked(symbVarItem->isRequired());
            mUi->cbxSymbVarItemRequired->setEnabled(true);
            mUi->edtSymbVarItemSuffix->setText(symbVarItem->getSuffix());
            mUi->edtSymbVarItemSuffix->setEnabled(true);
        } catch (const Exception& e) {
            QMessageBox::critical(this, tr("Error"), e.getUserMsg());
        }
    }
}

void ComponentSymbolVariantEditorWidget::cbxSymbVarItemRequiredCheckedChanged(bool checked) noexcept
{
    QListWidgetItem* item = mUi->lstSymbolItems->currentItem();
    if (item) {
        Uuid symbVarItemUuid = Uuid(item->data(Qt::UserRole).toString()); Q_ASSERT(!symbVarItemUuid.isNull());
        ComponentSymbolVariantItem* symbVarItem = mSymbVar.getItemByUuid(symbVarItemUuid); Q_ASSERT(symbVarItem);
        if (symbVarItem) {
            symbVarItem->setIsRequired(checked);
        } else {
            qWarning() << "Invalid item selected!";
        }
    } else {
        qWarning() << "No item selected!";
    }
}

void ComponentSymbolVariantEditorWidget::edtSymbVarItemSuffixTextChanged(const QString& text) noexcept
{
    QListWidgetItem* item = mUi->lstSymbolItems->currentItem();
    if (item) {
        Uuid symbVarItemUuid = Uuid(item->data(Qt::UserRole).toString()); Q_ASSERT(!symbVarItemUuid.isNull());
        ComponentSymbolVariantItem* symbVarItem = mSymbVar.getItemByUuid(symbVarItemUuid); Q_ASSERT(symbVarItem);
        if (symbVarItem) {
            // close your eyes - the following lines are an incredible ugly hack o_o
            QString oldText = QString(" (Suffix: \"%1\")").arg(symbVarItem->getSuffix());
            symbVarItem->setSuffix(text.trimmed());
            QString newText = QString(" (Suffix: \"%1\")").arg(symbVarItem->getSuffix());
            item->setText(item->text().replace(oldText, newText));
            for (int i = 0; i < mUi->tblPinSignalMap->rowCount(); ++i) {
                QTableWidgetItem* suffixItem = mUi->tblPinSignalMap->item(i, 1); Q_ASSERT(suffixItem);
                Uuid itemUuid = Uuid(suffixItem ? suffixItem->data(Qt::UserRole).toString() : "");
                if (itemUuid == symbVarItemUuid) {
                    suffixItem->setText(symbVarItem->getSuffix());
                }
            }
        } else {
            qWarning() << "Invalid item selected!";
        }
    } else {
        qWarning() << "No item selected!";
    }
}

void ComponentSymbolVariantEditorWidget::showEvent(QShowEvent* event) noexcept
{
    Q_UNUSED(event);
    mUi->graphicsView->zoomAll();
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
