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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/nodes/SoTransform.h>

#include "gc.h"
#include "TrackerZAxis.h"
#include "Transform.h"
#include "Vector3D.h"

SO_NODEENGINE_SOURCE( TrackerZAxis );

void TrackerZAxis::initClass()
{
	SO_NODEENGINE_INIT_CLASS( TrackerZAxis, TTracker, "TTracker" );
}

TrackerZAxis::TrackerZAxis()
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerZAxis );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( gc::Pi ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 0.0 ) );

	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);
}

TrackerZAxis::~TrackerZAxis()
{
}

std::string TrackerZAxis::GetIcon()
{
	return ( ":/icons/TrackerZAxis.png" );
}

void TrackerZAxis::Evaluate( Vector3D sunVectorW, Transform parentWT0 )
{
	Vector3D s = parentWT0( sunVectorW );
	Vector3D p( 0.0f, 0.0f, 1.0f);

	Vector3D n;
	Vector3D t;
	if( fabs( DotProduct( s, p ) ) < 1.0 )
	{
		t = Normalize( CrossProduct( s, p ) );
		n = Normalize( CrossProduct( p, t ) );
	}
	else
	{
		t = Vector3D( 1.0f, 0.0f, 0.0f );
		n = Vector3D( 0.0f, 1.0f, 0.0f );
	}

	SbMatrix transformMatrix( t[0], t[1], t[2], 0.0,
								n[0], n[1], n[2], 0.0,
								p[0], p[1], p[2], 0.0,
								0.0, 0.0, 0.0, 1.0 );

	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);
}

void TrackerZAxis::evaluate()
{

}
