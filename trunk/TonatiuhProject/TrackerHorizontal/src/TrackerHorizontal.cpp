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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "tgc.h"
#include "Trace.h"
#include "TrackerHorizontal.h"

SO_KIT_SOURCE( TrackerHorizontal );

void TrackerHorizontal::initClass()
{
	Trace trace( "TrackerHorizontal::initClass", false );

	SO_KIT_INIT_CLASS( TrackerHorizontal, TTracker, "SeparatorKit" );

}

TrackerHorizontal::TrackerHorizontal()
{
	Trace trace( "TrackerHorizontal::TrackerHorizontal", false );

	SO_KIT_CONSTRUCTOR( TrackerHorizontal );
	//SO_KIT_ADD_FIELD( sunRotation, (0.0, 0.0, 1.0, 0.0) );
	SO_KIT_INIT_INSTANCE();
	m_rotSensor = new SoFieldSensor( fielSensorCB, this );
	m_rotSensor->attach( &sunRotation );

}

TrackerHorizontal::~TrackerHorizontal()
{
	Trace trace( "TrackerHorizontal::~TrackerHorizontal", false );
}
QString TrackerHorizontal::getIcon()
{
	Trace trace( "TrackerHorizontal::getIcon", false );

	return QString(":/icons/TrackerHorizontal.png");
}

void TrackerHorizontal::fielSensorCB(void* data, SoSensor* /* sensor */ )
{
	Trace trace( "TrackerHorizontal::fielSensorCB", false );
	TrackerHorizontal* tracker = (TrackerHorizontal* ) data;
	if( tracker ) tracker->setRotation();
}

void TrackerHorizontal::setRotation()
{
	Trace trace( "TrackerHorizontal::setRotation", false );

	SbMatrix matrix;
	matrix.setRotate(sunRotation.getValue() );

	SbVec3f sunVector;
	matrix.multDirMatrix(  SbVec3f( 0.0, 1.0, 0.0 ), sunVector );

	double angle;

	SbVec3f vXY (sunVector[0], sunVector[1], 0 );
	vXY.normalize();

	angle = acos( vXY[1] );
	if( vXY[0] > 0 ) angle *= -1;

	SoTransform* trackerTransform = static_cast< SoTransform* > ( transform.getValue() );
	trackerTransform->rotation.setValue( SbVec3f( 0.0, 0.0, 1.0 ), angle );

}
