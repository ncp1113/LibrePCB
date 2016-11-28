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

#ifndef LIBREPCB_LIBRARY_CMDSYMBOLPINADD_H
#define LIBREPCB_LIBRARY_CMDSYMBOLPINADD_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <librepcb/common/undocommand.h>
#include <librepcb/common/units/all_length_units.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace library {

class Symbol;
class SymbolPin;

/*****************************************************************************************
 *  Class CmdSymbolPinAdd
 ****************************************************************************************/

/**
 * @brief The CmdSymbolPinAdd class
 */
class CmdSymbolPinAdd final : public UndoCommand
{
    public:

        // Constructors / Destructor
        CmdSymbolPinAdd() = delete;
        CmdSymbolPinAdd(const CmdSymbolPinAdd& other) = delete;
        CmdSymbolPinAdd(Symbol& symbol, const Point& pos = Point(0, 0),
                        const Angle& rot = Angle::deg0()) noexcept;
        CmdSymbolPinAdd(Symbol& symbol, SymbolPin& pin) noexcept;
        ~CmdSymbolPinAdd() noexcept;

        // Getters
        SymbolPin& getPin() const noexcept {return *mPin;}

        // Operator Overloadings
        CmdSymbolPinAdd& operator=(const CmdSymbolPinAdd& rhs) = delete;


    private:

        // Private Methods

        /// @copydoc UndoCommand::performExecute()
        bool performExecute() throw (Exception) override;

        /// @copydoc UndoCommand::performUndo()
        void performUndo() throw (Exception) override;

        /// @copydoc UndoCommand::performRedo()
        void performRedo() throw (Exception) override;


        // Private Member Variables

        // Attributes from the constructor
        Symbol& mSymbol;

        // General Attributes
        SymbolPin* mPin;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb

#endif // LIBREPCB_LIBRARY_CMDSYMBOLPINADD_H
