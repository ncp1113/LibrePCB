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
#include "symboleditorwidget.h"
#include "ui_symboleditorwidget.h"
#include <librepcb/common/gridproperties.h>
#include <librepcb/common/graphics/graphicsscene.h>
#include <librepcb/library/sym/symbol.h>
#include <librepcb/library/sym/symbolgraphicsitem.h>
#include <librepcb/library/cmp/cmpsigpindisplaytype.h>
#include <librepcb/workspace/workspace.h>
#include <librepcb/workspace/settings/workspacesettings.h>
#include "../libraryeditor.h"
#include "fsm/symboleditorfsm.h"

#include <librepcb/common/graphics/ellipsegraphicsitem.h>
#include <librepcb/common/schematiclayer.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SymbolEditorWidget::SymbolEditorWidget(workspace::Workspace& ws, LibraryEditor& editor,
        const FilePath& fp, QWidget* parent) throw (Exception) :
    EditorWidgetBase(ws, editor, fp, parent), mUi(new Ui::SymbolEditorWidget),
    mGridProperties(new GridProperties()), mGraphicsScene(new GraphicsScene())
{
    mUi->setupUi(this);
    mUi->graphicsView->setUseOpenGl(mWorkspace.getSettings().getAppearance().getUseOpenGl());
    mUi->graphicsView->setGridProperties(*mGridProperties);
    mUi->graphicsView->setScene(mGraphicsScene.data());
    setWindowIcon(QIcon(":/img/library/symbol.png"));

    // insert category list editor widget
    mCategoriesEditorWidget.reset(new ComponentCategoryListEditorWidget(mWorkspace, this));
    int row;
    QFormLayout::ItemRole role;
    mUi->formLayout->getWidgetPosition(mUi->lblCategories, &row, &role);
    mUi->formLayout->setWidget(row, QFormLayout::FieldRole, mCategoriesEditorWidget.data());

    // load symbol
    mSymbol.reset(new Symbol(fp, false)); // can throw
    setWindowTitle(mSymbol->getName(mLocaleOrder));
    mUi->lblUuid->setText(QString("<a href=\"%1\">%2</a>").arg(
        mSymbol->getFilePath().toQUrl().toString(), mSymbol->getUuid().toStr()));
    mUi->lblUuid->setToolTip(mSymbol->getFilePath().toNative());
    mUi->edtName->setText(mSymbol->getName(mLocaleOrder));
    mUi->edtDescription->setText(mSymbol->getDescription(mLocaleOrder));
    mUi->edtKeywords->setText(mSymbol->getKeywords(mLocaleOrder));
    mUi->edtAuthor->setText(mSymbol->getAuthor());
    mUi->edtVersion->setText(mSymbol->getVersion().toStr());
    mCategoriesEditorWidget->setUuids(mSymbol->getCategories());
    mUi->cbxDeprecated->setChecked(mSymbol->isDeprecated());

    // load graphics items recursively
    mGraphicsItem.reset(new SymbolGraphicsItem(*mSymbol, editor));
    mGraphicsScene->addItem(*mGraphicsItem);
    mUi->graphicsView->zoomAll();

    // load finite state machine (FSM)
    SymbolEditorState::Context fsmContext {
        *this, *mUndoStack, *mGraphicsScene, *mGridProperties, *mSymbol, *mGraphicsItem
    };
    mFsm.reset(new SymbolEditorFsm(fsmContext));

    // last but not least, connect the graphics scene events with the FSM
    mUi->graphicsView->setEventHandlerObject(this);
}

SymbolEditorWidget::~SymbolEditorWidget() noexcept
{
}

/*****************************************************************************************
 *  Public Slots
 ****************************************************************************************/

bool SymbolEditorWidget::save() noexcept
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

        mSymbol->setName("en_US", name);
        mSymbol->setDescription("en_US", mUi->edtDescription->text().trimmed());
        mSymbol->setKeywords("en_US", mUi->edtKeywords->text().trimmed());
        mSymbol->setAuthor(mUi->edtAuthor->text().trimmed());
        mSymbol->setVersion(version);
        mSymbol->setCategories(mCategoriesEditorWidget->getUuids());
        mSymbol->setDeprecated(mUi->cbxDeprecated->isChecked());
        mSymbol->save();
        return EditorWidgetBase::save();
    } catch (const Exception& e) {
        QMessageBox::critical(this, tr("Save failed"), e.getUserMsg());
        return false;
    }
}

bool SymbolEditorWidget::rotateCw() noexcept
{
    return mFsm->processRotateCw();
}

bool SymbolEditorWidget::rotateCcw() noexcept
{
    return mFsm->processRotateCcw();
}

bool SymbolEditorWidget::remove() noexcept
{
    return mFsm->processRemove();
}

bool SymbolEditorWidget::abortCommand() noexcept
{
    return mFsm->processAbortCommand();
}

bool SymbolEditorWidget::startSelecting() noexcept
{
    return mFsm->processStartSelecting();
}

bool SymbolEditorWidget::startAddingSymbolPins() noexcept
{
    return mFsm->processStartAddingSymbolPins();
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool SymbolEditorWidget::graphicsViewEventHandler(QEvent* event) noexcept
{
    Q_ASSERT(event);
    switch (event->type()) {
        case QEvent::GraphicsSceneMouseMove: {
            auto* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event); Q_ASSERT(e);
            return mFsm->processGraphicsSceneMouseMoved(*e);
        }
        case QEvent::GraphicsSceneMousePress: {
            auto* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event); Q_ASSERT(e);
            switch (e->button()) {
                case Qt::LeftButton: return mFsm->processGraphicsSceneLeftMouseButtonPressed(*e);
                default: return false;
            }
        }
        case QEvent::GraphicsSceneMouseRelease: {
            auto* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event); Q_ASSERT(e);
            switch (e->button()) {
                case Qt::LeftButton: return mFsm->processGraphicsSceneLeftMouseButtonReleased(*e);
                default: return false;
            }
        }
        default: {
            return false;
        }
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb
