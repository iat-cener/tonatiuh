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
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransform.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "tgc.h"
#include "TrackerHeliostat.h"
#include "Transform.h"
#include "TSceneKit.h"
#include "TSeparatorKit.h"
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

	/*SO_NODE_DEFINE_ENUM_VALUE( Rotations, YX );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, YZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, XZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, ZX );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfRotation, Rotations );
	SO_NODE_ADD_FIELD( typeOfRotation, (YX) );*/

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

	TSeparatorKit* sunNode = static_cast< TSeparatorKit* > ( m_scene->getPart( "childList[0]", false ) );
	if( !sunNode )	return;

	TSeparatorKit* rootNode = static_cast< TSeparatorKit* > ( sunNode->getPart( "childList[0]", false ) );
	if( !rootNode )	return;

	SoSearchAction* coinSearch = new SoSearchAction();
	coinSearch->setNode( this );
	coinSearch->setInterest( SoSearchAction::FIRST );
	coinSearch->apply( rootNode );
	SoPath* nodePath = coinSearch->getPath( );
	if( !nodePath ) return;


	SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction( SbViewportRegion () );
	getmatrixAction->apply( nodePath );


	SbMatrix objectToWorld = getmatrixAction->getMatrix();
	SbMatrix worldToObject = objectToWorld.inverse();

	SbVec3f globalSunVector( sin( m_azimuth.getValue() ) * sin( m_zenith.getValue() ),
							 cos( m_zenith.getValue() ),
							-cos( m_azimuth.getValue() ) * sin( m_zenith.getValue() ) );


	//SbVec3f globalSunVector( 0.0, 1.0, 0.0 );

	SbVec3f i;
	worldToObject.multDirMatrix ( globalSunVector, i );


	SbVec3f focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );



	SbVec3f r;
	worldToObject.multVecMatrix( focus, r );

	r.normalize();


	SbVec3f n = ( i + r );
	n.normalize();

	SbVec3f t = SbVec3f( n[2], 0.0f, -n[0] );
	t.normalize();

	SbVec3f p = t.cross(n);
	p.normalize();


	SbMatrix transformMatrix( t[0], t[1], t[2], 0.0,
							  n[0], n[1], n[2], 0.0,
							  p[0], p[1], p[2], 0.0,
							  0.0, 0.0, 0.0, 1.0 );


	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );


	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( newTransform->translation.getValue() ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( newTransform->rotation.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( newTransform->scaleFactor.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( newTransform->scaleOrientation.getValue() ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( newTransform->center.getValue() ) );

}
