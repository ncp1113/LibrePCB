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

#ifndef LIBREPCB_LIBRARY_EDITOR_COMPONENTSYMBOLVARIANTEDITORWIDGET_H
#define LIBREPCB_LIBRARY_EDITOR_COMPONENTSYMBOLVARIANTEDITORWIDGET_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QWidget>
#include <QListWidgetItem>
#include <librepcb/common/exceptions.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class IF_SchematicLayerProvider;
class GraphicsScene;

namespace workspace {
class Workspace;
}

namespace library {

class Component;
class ComponentSymbolVariant;
class Symbol;
class SymbolPreviewGraphicsItem;

namespace editor {

namespace Ui {
class ComponentSymbolVariantEditorWidget;
}

/*****************************************************************************************
 *  Class ComponentSymbolVariantEditorWidget
 ****************************************************************************************/

/**
 * @brief The ComponentSymbolVariantEditorWidget class
 *
 * @author ubruhin
 * @date 2016-10-23
 */
class ComponentSymbolVariantEditorWidget final : public QWidget
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        ComponentSymbolVariantEditorWidget() = delete;
        ComponentSymbolVariantEditorWidget(const ComponentSymbolVariantEditorWidget& other) = delete;
        ComponentSymbolVariantEditorWidget(workspace::Workspace& ws,
                                           const IF_SchematicLayerProvider& layerProvider,
                                           Component& cmp, ComponentSymbolVariant& symbVar,
                                           QWidget* parent = nullptr) throw (Exception);
        ~ComponentSymbolVariantEditorWidget() noexcept;

        // Setters
        void setIsDefault(bool isDefault) noexcept;

        // Operator Overloadings
        ComponentSymbolVariantEditorWidget& operator=(const ComponentSymbolVariantEditorWidget& rhs) = delete;


    signals:

        void setAsDefaultRequested();



    private: // Methods

        void lstSymbolItemsCurrentItemChanged(QListWidgetItem* current,
                                              QListWidgetItem* previous) noexcept;
        void cbxSymbVarItemRequiredCheckedChanged(bool checked) noexcept;
        void edtSymbVarItemSuffixTextChanged(const QString& text) noexcept;
        void showEvent(QShowEvent* event) noexcept override;


    private: // Data

        workspace::Workspace& mWorkspace;
        const IF_SchematicLayerProvider& mLayerProvider;
        Component& mComponent;
        ComponentSymbolVariant& mSymbVar;
        QScopedPointer<Ui::ComponentSymbolVariantEditorWidget> mUi;
        QScopedPointer<GraphicsScene> mPreviewScene;
        const QStringList& mLocaleOrder;

        // preview
        QScopedPointer<Symbol> mPreviewSymbol;
        QScopedPointer<SymbolPreviewGraphicsItem> mSymbolPreviewGraphicsItem;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_COMPONENTSYMBOLVARIANTEDITORWIDGET_H
