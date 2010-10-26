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
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "tgc.h"
#include "TrackerHeliostat.h"
#include "Transform.h"
#include "Vector3D.h"

SO_NODEENGINE_SOURCE( TrackerHeliostat );

void TrackerHeliostat::initClass()
{
	SO_NODEENGINE_INIT_CLASS( TrackerHeliostat, TTracker, "TTracker" );

}

TrackerHeliostat::TrackerHeliostat()
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerHeliostat );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );
	SO_NODE_ADD_FIELD( aimingPoint, ( 0.0, 0.0, 0.0 ) );


	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);


}

TrackerHeliostat::~TrackerHeliostat()
{
}

QString TrackerHeliostat::getIcon()
{

	return QString(":/icons/TrackerHeliostat.png");
}

void TrackerHeliostat::evaluate()
{

	if( !m_azimuth.isConnected() ) return;
	if( !m_zenith.isConnected() ) return;

	SoSearchAction* coinSearch = new SoSearchAction();
	coinSearch->setNode( this );
	coinSearch->setInterest( SoSearchAction::FIRST );
	coinSearch->apply( m_scene );
	SoPath* nodePath = coinSearch->getPath( );
	if( !nodePath ) return;


	SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction( SbViewportRegion () );
	getmatrixAction->apply( nodePath );


	SbMatrix objectToWorld = getmatrixAction->getMatrix();
	SbMatrix worldToObject = objectToWorld.inverse();

	SbVec3f globalSunVector( sin( m_zenith.getValue() ) * sin( m_azimuth.getValue() ),
							cos( m_zenith.getValue() ),
							-sin( m_zenith.getValue() )*cos( m_azimuth.getValue() ) );
	SbVec3f i;
	worldToObject.multDirMatrix ( globalSunVector, i );

	SbVec3f focus = aimingPoint.getValue();
	SbVec3f r;
	worldToObject.multVecMatrix( focus, r );
	r.normalize();

	SbVec3f n = ( i + r ) / fabs( r.dot( i ) );
	n.normalize();

	double theta = acos( n[1] );
	SbRotation xRotation( SbVec3f( 1.0, 0.0, 0.0 ), theta );

	double phi = atan2( n[0], n[2] );
	SbRotation yRotation( SbVec3f( 0.0, 1.0, 0.0 ), phi );

	SbRotation rotation = xRotation;
	rotation *= yRotation;

	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( 0.0, 0.0, 0.0 ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( rotation ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( 1.0, 1.0, 1.0 ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( 0.0, 0.0, 1.0, 0.0 ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( 0.0, 0.0, 0.0 ) );

}
