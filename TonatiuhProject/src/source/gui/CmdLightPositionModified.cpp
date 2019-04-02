/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "gf.h"

#include "CmdLightPositionModified.h"
#include "TLightKit.h"

/**
 * Creates a new lightKit position command that represents a \a light move with \a parent parent.
 *
 * The new position is defined by \a azimuth and \a zenith in radians.
 */
CmdLightPositionModified::CmdLightPositionModified( TLightKit* light, double azimuth, double zenith, QUndoCommand* parent )
:QUndoCommand( "Sun position changed", parent ), lightKit( light ), m_newAzimuth( azimuth ), m_newZenith( zenith )
{
	if( light == 0 ) gf::SevereError( "CmdLinghtPositionModified called with NULL TLightKit" );

	m_oldAzimuth = light->azimuth.getValue();
	m_oldZenith = light->zenith.getValue();
}

/*!
 * Destroys the CmdLightPositionModified object.
 */
CmdLightPositionModified::~CmdLightPositionModified()
{
}

/*!
 * Reverts to the previous light positions. After undo() is called, the light position will be the same as before redo() was called.
 *
 * \sa redo().
 */
void CmdLightPositionModified::undo()
{
	lightKit->ChangePosition( m_oldAzimuth, m_oldZenith );
}

/*!
 * Applies a change to the scene. After redo() the light position will be the position defined by the constructor parameters.
 * \sa undo().
 */
void CmdLightPositionModified::redo()
{
	lightKit->ChangePosition( m_newAzimuth, m_newZenith );
}
