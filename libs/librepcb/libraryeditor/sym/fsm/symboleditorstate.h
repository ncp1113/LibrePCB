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

#ifndef LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORSTATE_H
#define LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORSTATE_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtWidgets>
#include <librepcb/common/units/all_length_units.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class UndoStack;
class GraphicsScene;

namespace library {

class Symbol;
class SymbolGraphicsItem;

namespace editor {

class SymbolEditorWidget;

/*****************************************************************************************
 *  Class SymbolEditorState
 ****************************************************************************************/

/**
 * @brief The SymbolEditorState class is the base class of all symbol editor FSM states
 *
 * @author  ubruhin
 * @date    2016-11-01
 */
class SymbolEditorState
{
    public:

        // Types
        struct Context {
            SymbolEditorWidget& editorWidget;
            UndoStack& undoStack;
            GraphicsScene& graphicsScene;
            Symbol& symbol;
            SymbolGraphicsItem& symbolGraphicsItem;
        };

        // Constructors / Destructor
        SymbolEditorState() = delete;
        SymbolEditorState(const SymbolEditorState& other) = delete;
        explicit SymbolEditorState(const Context& context) noexcept;
        virtual ~SymbolEditorState() noexcept;

        // General Methods
        virtual bool entry() noexcept {return true;}
        virtual bool exit() noexcept {return true;}

        // Event Handlers
        virtual bool processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept = 0;
        virtual bool processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept = 0;
        virtual bool processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept = 0;

        // Operator Overloadings
        SymbolEditorState& operator=(const SymbolEditorState& rhs) = delete;


    protected: // Data
        Context mContext;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORSTATE_H
