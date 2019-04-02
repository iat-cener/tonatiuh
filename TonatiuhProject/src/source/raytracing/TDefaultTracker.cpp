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

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <QString>

#include "gc.h"

#include "TDefaultTracker.h"

SO_NODEENGINE_SOURCE( TDefaultTracker );

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 *
 */
void TDefaultTracker::initClass()
{

	SO_NODEENGINE_INIT_CLASS( TDefaultTracker, TTracker, "TTracker" );
}

/**
 * TDefaultTracker constructor
 */
TDefaultTracker::TDefaultTracker()
{
	SO_NODEENGINE_CONSTRUCTOR(TDefaultTracker);

	//SO_NODE_ADD_FIELD( m_azimuth, ( gc::Pi ) );
	//SO_NODE_ADD_FIELD( m_zenith, ( 0.0 ) );

	//ConstructEngineOutput();
	// Define input fields and their default values
	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);
}

/**
 * TDefaultTracker destructor
 */
TDefaultTracker::~TDefaultTracker()
{

}

/**
 * Returns the icon that represents the TDefaultTracker.
 */
QString TDefaultTracker::getIcon()
{
	return QLatin1String(":/icons/HorizontalTracker.png");
}

/**
 * Computes the rotation ouput.
 */
void TDefaultTracker::evaluate()
{
	//if (!IsConnected()) return;

	SetEngineOutputIdentity();

}
