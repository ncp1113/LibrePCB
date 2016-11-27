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
#include "cmdsymbolpinedit.h"
#include "../symbolpin.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

CmdSymbolPinEdit::CmdSymbolPinEdit(SymbolPin& pin) noexcept :
    UndoCommand(tr("Edit pin")), mPin(pin),
    mOldPos(pin.getPosition()), mNewPos(mOldPos),
    mOldRotation(pin.getRotation()), mNewRotation(mOldRotation)
{
}

CmdSymbolPinEdit::~CmdSymbolPinEdit() noexcept
{
    if (!wasEverExecuted()) {
        mPin.setPosition(mOldPos);
        mPin.setRotation(mOldRotation);
    }
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void CmdSymbolPinEdit::setPosition(const Point& pos, bool immediate) noexcept
{
    Q_ASSERT(!wasEverExecuted());
    mNewPos = pos;
    if (immediate) mPin.setPosition(mNewPos);
}

void CmdSymbolPinEdit::setDeltaToStartPos(const Point& deltaPos, bool immediate) noexcept
{
    Q_ASSERT(!wasEverExecuted());
    mNewPos = mOldPos + deltaPos;
    if (immediate) mPin.setPosition(mNewPos);
}

void CmdSymbolPinEdit::setRotation(const Angle& angle, bool immediate) noexcept
{
    Q_ASSERT(!wasEverExecuted());
    mNewRotation = angle;
    if (immediate) mPin.setRotation(mNewRotation);
}

void CmdSymbolPinEdit::rotate(const Angle& angle, const Point& center, bool immediate) noexcept
{
    Q_ASSERT(!wasEverExecuted());
    mNewPos.rotate(angle, center);
    mNewRotation += angle;
    if (immediate) {
        mPin.setPosition(mNewPos);
        mPin.setRotation(mNewRotation);
    }
}

/*****************************************************************************************
 *  Inherited from UndoCommand
 ****************************************************************************************/

bool CmdSymbolPinEdit::performExecute() throw (Exception)
{
    performRedo(); // can throw

    return true; // TODO: determine if the pin was really modified
}

void CmdSymbolPinEdit::performUndo() throw (Exception)
{
    mPin.setPosition(mOldPos);
    mPin.setRotation(mOldRotation);
}

void CmdSymbolPinEdit::performRedo() throw (Exception)
{
    mPin.setPosition(mNewPos);
    mPin.setRotation(mNewRotation);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb
