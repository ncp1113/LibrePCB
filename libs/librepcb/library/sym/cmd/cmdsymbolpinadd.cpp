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
#include "cmdsymbolpinadd.h"
#include "../symbol.h"
#include "../symbolpin.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdSymbolPinAdd::CmdSymbolPinAdd(Symbol& symbol) noexcept :
    UndoCommand(tr("Add pin")), mSymbol(symbol),
    mPin(new SymbolPin(Uuid::createRandom(), "Pin", Point(0, 0), Length(2540000), Angle::deg0()))
{
}

CmdSymbolPinAdd::CmdSymbolPinAdd(Symbol& symbol, SymbolPin& pin) noexcept :
    UndoCommand(tr("Add pin")), mSymbol(symbol), mPin(&pin)
{
}

CmdSymbolPinAdd::~CmdSymbolPinAdd() noexcept
{
    if (!isCurrentlyExecuted()) {
        delete mPin;
    }
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdSymbolPinAdd::performExecute() throw (Exception)
{
    performRedo(); // can throw
    return true;
}

void CmdSymbolPinAdd::performUndo() throw (Exception)
{
    mSymbol.removePin(*mPin);
}

void CmdSymbolPinAdd::performRedo() throw (Exception)
{
    mSymbol.addPin(*mPin);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
