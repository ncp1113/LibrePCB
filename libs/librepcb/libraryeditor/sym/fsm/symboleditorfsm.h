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

#ifndef LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORFSM_H
#define LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORFSM_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "symboleditorstate.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*****************************************************************************************
 *  Class SymbolEditorFsm
 ****************************************************************************************/

/**
 * @brief The SymbolEditorFsm class is the finit state machine (FSM) of the symbol editor
 *
 * @author  ubruhin
 * @date    2016-11-01
 */
class SymbolEditorFsm final : public SymbolEditorState
{
        Q_OBJECT

    private: // Types
        enum class State {IDLE, SELECT, ADD_PINS};


    public:

        // Constructors / Destructor
        SymbolEditorFsm() = delete;
        SymbolEditorFsm(const SymbolEditorFsm& other) = delete;
        explicit SymbolEditorFsm(const Context& context) noexcept;
        virtual ~SymbolEditorFsm() noexcept;

        // Event Handlers
        bool processGraphicsSceneMouseMoved(QGraphicsSceneMouseEvent& e) noexcept override;
        bool processGraphicsSceneLeftMouseButtonPressed(QGraphicsSceneMouseEvent& e) noexcept override;
        bool processGraphicsSceneLeftMouseButtonReleased(QGraphicsSceneMouseEvent& e) noexcept override;
        bool processRotateCw() noexcept override;
        bool processRotateCcw() noexcept override;
        bool processRemove() noexcept override;
        bool processAbortCommand() noexcept override;
        bool processStartSelecting() noexcept override;
        bool processStartAddingSymbolPins() noexcept override;

        // Operator Overloadings
        SymbolEditorState& operator=(const SymbolEditorState& rhs) = delete;


    private: // Methods
        SymbolEditorState* getCurrentState() const noexcept;
        bool setNextState(State state) noexcept;
        bool leaveCurrentState() noexcept;
        bool enterNextState(State state) noexcept;


    private: // Data
        QMap<State, SymbolEditorState*> mStates;
        State mCurrentState;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace editor
} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_EDITOR_SYMBOLEDITORFSM_H
