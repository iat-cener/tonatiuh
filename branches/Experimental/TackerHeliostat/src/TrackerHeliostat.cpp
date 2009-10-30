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
#include <iostream>

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "tgc.h"
#include "Trace.h"
#include "TrackerHeliostat.h"
#include "Transform.h"
#include "Vector3D.h"

SO_NODEENGINE_SOURCE( TrackerHeliostat );

void TrackerHeliostat::initClass()
{
	Trace trace( "TrackerHorizontal::initClass", false );

	SO_NODEENGINE_INIT_CLASS( TrackerHeliostat, TTracker, "TTracker" );

}

TrackerHeliostat::TrackerHeliostat()
{
	Trace trace( "TrackerHeliostat::TrackerHeliostat", false );

	SO_NODEENGINE_CONSTRUCTOR( TrackerHeliostat );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );
	SO_NODE_ADD_FIELD( aimingPoint, ( 0.0, 0.0, 0.0 ) );
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation );


}

TrackerHeliostat::~TrackerHeliostat()
{
	Trace trace( "TrackerHeliostat::~TrackerHeliostat", false );
}

QString TrackerHeliostat::getIcon()
{
	Trace trace( "TrackerHeliostat::getIcon", false );

	return QString(":/icons/TrackerHeliostat.png");
}

void TrackerHeliostat::evaluate()
{
	Trace trace( "TrackerHeliostat::evaluate", false );

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


	SbMatrix objectToWorldMatrix = getmatrixAction->getMatrix();
	Transform objectToWorld( objectToWorldMatrix[0][0], objectToWorldMatrix[1][0], objectToWorldMatrix[2][0], objectToWorldMatrix[3][0],
							 objectToWorldMatrix[0][1], objectToWorldMatrix[1][1], objectToWorldMatrix[2][1], objectToWorldMatrix[3][1],
							 objectToWorldMatrix[0][2], objectToWorldMatrix[1][2], objectToWorldMatrix[2][2], objectToWorldMatrix[3][2],
							 objectToWorldMatrix[0][3], objectToWorldMatrix[1][3], objectToWorldMatrix[2][3], objectToWorldMatrix[3][3] );
	Transform worldToObject = objectToWorld.GetInverse();


	Vector3D globalSunVector( sin( m_zenith.getValue() ) * sin( m_azimuth.getValue() ),
						cos( m_zenith.getValue() ),
						-sin( m_zenith.getValue() )*cos( m_azimuth.getValue() ) );
	Vector3D i = worldToObject( globalSunVector );


	SbVec3f focus = aimingPoint.getValue();
	Point3D rDir( focus[0], focus[1], focus[2] );
	Vector3D r( worldToObject( rDir ) );
	r = Normalize( r );

	Vector3D nVector = ( i + r ) / fabs(  DotProduct( r, i ) );
	NormalVector n = Normalize( NormalVector( nVector ) );

	double theta = acos( n.y );
	SbRotation xRotation( SbVec3f( 1.0, 0.0, 0.0 ), theta );

	double phi = atan2( n.x, n.z );
	SbRotation yRotation( SbVec3f( 0.0, 1.0, 0.0 ), phi );

	SbRotation rotation = xRotation;
	rotation *= yRotation;

	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( rotation ) );

}
