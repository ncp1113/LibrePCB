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
#include "libraryeditor.h"
#include "ui_libraryeditor.h"
#include <librepcb/common/schematiclayer.h>
#include <librepcb/library/library.h>
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/settings/workspacesettings.h>
#include "lib/libraryoverviewwidget.h"
#include "cmpcat/componentcategoryeditorwidget.h"
#include "pkgcat/packagecategoryeditorwidget.h"
#include "sym/symboleditorwidget.h"
#include "pkg/packageeditorwidget.h"
#include "cmp/componenteditorwidget.h"
#include "dev/deviceeditorwidget.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

LibraryEditor::LibraryEditor(workspace::Workspace& ws, QSharedPointer<Library> lib) throw (Exception) :
    QMainWindow(nullptr), mWorkspace(ws), mLibrary(lib), mUi(new Ui::LibraryEditor)
{
    mUi->setupUi(this);
    mUi->actionSave->setEnabled(!mLibrary->isOpenedReadOnly());
    connect(mUi->tabWidget, &QTabWidget::currentChanged,
            this, &LibraryEditor::currentTabChanged);
    connect(mUi->tabWidget, &QTabWidget::tabCloseRequested,
            this, &LibraryEditor::tabCloseRequested);

    // set window title
    const QStringList localeOrder = mWorkspace.getSettings().getLibLocaleOrder().getLocaleOrder();
    QString libName = mLibrary->getName(localeOrder);
    if (mLibrary->isOpenedReadOnly()) libName.append(tr(" [Read-Only]"));
    setWindowTitle(QString(tr("%1 - LibrePCB Library Editor")).arg(libName));

    // add all required layers
    addSchematicLayer(SchematicLayer::LayerID::Grid);
    addSchematicLayer(SchematicLayer::LayerID::OriginCrosses);
    addSchematicLayer(SchematicLayer::LayerID::SymbolOutlines);
    addSchematicLayer(SchematicLayer::LayerID::SymbolGrabAreas);
    addSchematicLayer(SchematicLayer::LayerID::SymbolOptionalPinCircles);
    addSchematicLayer(SchematicLayer::LayerID::SymbolRequiredPinCircles);
    addSchematicLayer(SchematicLayer::LayerID::SymbolPinNames);
    addSchematicLayer(SchematicLayer::LayerID::ComponentNames);
    addSchematicLayer(SchematicLayer::LayerID::ComponentValues);
    addSchematicLayer(SchematicLayer::LayerID::NetLabels);
    addSchematicLayer(SchematicLayer::LayerID::Nets);
    addSchematicLayer(SchematicLayer::LayerID::Busses);
#ifdef QT_DEBUG
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_GraphicsItemsBoundingRect);
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_GraphicsItemsTextsBoundingRect);
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_SymbolPinNetSignalNames);
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_NetLinesNetSignalNames);
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_InvisibleNetPoints);
    addSchematicLayer(SchematicLayer::LayerID::DEBUG_ComponentSymbolsCount);
#endif

    // add overview tab
    QScopedPointer<LibraryOverviewWidget> overviewWidget(new LibraryOverviewWidget(ws, *this, lib));
    connect(overviewWidget.data(), &LibraryOverviewWidget::editComponentCategoryTriggered,
            this, &LibraryEditor::editComponentCategoryTriggered);
    connect(overviewWidget.data(), &LibraryOverviewWidget::editPackageCategoryTriggered,
            this, &LibraryEditor::editPackageCategoryTriggered);
    connect(overviewWidget.data(), &LibraryOverviewWidget::editSymbolTriggered,
            this, &LibraryEditor::editSymbolTriggered);
    connect(overviewWidget.data(), &LibraryOverviewWidget::editPackageTriggered,
            this, &LibraryEditor::editPackageTriggered);
    connect(overviewWidget.data(), &LibraryOverviewWidget::editComponentTriggered,
            this, &LibraryEditor::editComponentTriggered);
    connect(overviewWidget.data(), &LibraryOverviewWidget::editDeviceTriggered,
            this, &LibraryEditor::editDeviceTriggered);
    mUi->tabWidget->addTab(overviewWidget.take(), lib->getIcon(), tr("Library"));

    // remove close button on first tab (which is the library overview)
    QTabBar* tabBar = mUi->tabWidget->tabBar(); Q_ASSERT(tabBar);
    tabBar->setTabButton(0, QTabBar::RightSide, nullptr);

    // Restore Window Geometry
    QSettings clientSettings;
    restoreGeometry(clientSettings.value("library_editor/window_geometry").toByteArray());
    restoreState(clientSettings.value("library_editor/window_state").toByteArray());
}

LibraryEditor::~LibraryEditor() noexcept
{
}

/*****************************************************************************************
 *  Public Methods
 ****************************************************************************************/

bool LibraryEditor::closeAndDestroy(bool askForSave) noexcept
{
    // Save Window Geometry
    QSettings clientSettings;
    clientSettings.setValue("library_editor/window_geometry", saveGeometry());
    clientSettings.setValue("library_editor/window_state", saveState());

    // TODO
    deleteLater();
    return true;

    /*if (((!mProject.isRestored()) && (mUndoStack->isClean())) || (mProject.isReadOnly()) || (!askForSave))
    {
        // no unsaved changes or opened in read-only mode or don't save --> close project
        deleteLater();  // this project object will be deleted later in the event loop
        return true;
    }

    QString msg1 = tr("You have unsaved changes in the project.\n"
                      "Do you want to save them before closing the project?");
    QString msg2 = tr("Attention: The project was restored from a backup, so if you "
                      "don't save the project now the current state of the project (and "
                      "the backup) will be lost forever!");

    QMessageBox::StandardButton choice = QMessageBox::question(msgBoxParent,
         tr("Save Project?"), (mProject.isRestored() ? msg1 % QStringLiteral("\n\n") % msg2 : msg1),
         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

    switch (choice)
    {
        case QMessageBox::Yes: // save and close project
            if (saveProject())
            {
                deleteLater(); // this project object will be deleted later in the event loop
                return true;
            }
            else
                return false;

        case QMessageBox::No: // close project without saving
            deleteLater(); // this project object will be deleted later in the event loop
            return true;

        default: // cancel, don't close the project
            return false;
    }*/
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void LibraryEditor::editComponentCategoryTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<ComponentCategoryEditorWidget>(fp);
}

void LibraryEditor::editPackageCategoryTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<PackageCategoryEditorWidget>(fp);
}

void LibraryEditor::editSymbolTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<SymbolEditorWidget>(fp);
}

void LibraryEditor::editPackageTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<PackageEditorWidget>(fp);
}

void LibraryEditor::editComponentTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<ComponentEditorWidget>(fp);
}

void LibraryEditor::editDeviceTriggered(const FilePath& fp) noexcept
{
    editLibraryElementTriggered<DeviceEditorWidget>(fp);
}

template <typename EditWidgetType>
void LibraryEditor::editLibraryElementTriggered(const FilePath& fp) noexcept
{
    try {
        for (int i = 0; i < mUi->tabWidget->count(); i++) {
            EditorWidgetBase* widget = dynamic_cast<EditorWidgetBase*>(mUi->tabWidget->widget(i));
            if (widget && (widget->getFilePath() == fp)) {
                mUi->tabWidget->setCurrentIndex(i);
                return;
            }
        }

        EditWidgetType* widget = new EditWidgetType(mWorkspace, *this, fp);
        int index = mUi->tabWidget->addTab(widget, widget->windowIcon(), widget->windowTitle());
        mUi->tabWidget->setCurrentIndex(index);
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Failed to open category"), e.getUserMsg());
    }
}

void LibraryEditor::currentTabChanged(int index) noexcept
{
    foreach (const auto& connection, mCurrentTabActionConnections) {
        disconnect(connection);
    }

    EditorWidgetBase* widget = dynamic_cast<EditorWidgetBase*>(mUi->tabWidget->widget(index));
    if (widget) {
        mCurrentTabActionConnections.append(
                    connect(mUi->actionSave, &QAction::triggered,
                            widget, &EditorWidgetBase::save));
        mCurrentTabActionConnections.append(
                    connect(mUi->actionUndo, &QAction::triggered,
                            widget, &EditorWidgetBase::undo));
        mCurrentTabActionConnections.append(
                    connect(mUi->actionRedo, &QAction::triggered,
                            widget, &EditorWidgetBase::redo));
        mCurrentTabActionConnections.append(
                    connect(widget, &EditorWidgetBase::undoTextChanged,
                            mUi->actionUndo, &QAction::setText));
        mCurrentTabActionConnections.append(
                    connect(widget, &EditorWidgetBase::redoTextChanged,
                            mUi->actionRedo, &QAction::setText));
        mCurrentTabActionConnections.append(
                    connect(widget, &EditorWidgetBase::canUndoChanged,
                            mUi->actionUndo, &QAction::setEnabled));
        mCurrentTabActionConnections.append(
                    connect(widget, &EditorWidgetBase::canRedoChanged,
                            mUi->actionRedo, &QAction::setEnabled));
        mUi->actionUndo->setText(widget->getUndoText());
        mUi->actionRedo->setText(widget->getRedoText());
        mUi->actionUndo->setEnabled(widget->canUndo());
        mUi->actionRedo->setEnabled(widget->canRedo());
    }
}

void LibraryEditor::tabCloseRequested(int index) noexcept
{
    EditorWidgetBase* widget = dynamic_cast<EditorWidgetBase*>(
                                   mUi->tabWidget->widget(index));
    // library overview tab cannot be closed!
    if (dynamic_cast<LibraryOverviewWidget*>(widget) == nullptr) {
        if (widget->hasChanges()) {
            QString msg = tr("You have unsaved changes in the library element.\n"
                             "Do you want to save them before closing it?");
            QMessageBox::StandardButton choice = QMessageBox::question(this,
                 tr("Unsaved changes"), msg, QMessageBox::Yes | QMessageBox::No |
                 QMessageBox::Cancel, QMessageBox::Yes);
            switch (choice)
            {
                case QMessageBox::Yes:
                    if (widget->save()) {
                        delete widget;
                    }
                    break;
                case QMessageBox::No:
                    delete widget;
                    break;
                default:
                    break;
            }
        } else {
            delete widget;
        }
    }
}

void LibraryEditor::closeEvent(QCloseEvent* event) noexcept
{
    if (closeAndDestroy(true)) {
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
}

void LibraryEditor::addSchematicLayer(int id) noexcept
{
    mSchematicLayers.insert(id, new SchematicLayer(id));
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
