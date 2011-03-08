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
#include "tgf.h"
#include "TrackerLinearFresnel.h"
#include "Transform.h"
#include "Vector3D.h"


SO_NODEENGINE_SOURCE( TrackerLinearFresnel );

void TrackerLinearFresnel::initClass()
{
	SO_NODEENGINE_INIT_CLASS( TrackerLinearFresnel, TTracker, "TTracker" );

}

TrackerLinearFresnel::TrackerLinearFresnel()
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerLinearFresnel );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );

	SO_NODE_DEFINE_ENUM_VALUE( Axis, X );
	SO_NODE_DEFINE_ENUM_VALUE( Axis, Y );
	SO_NODE_DEFINE_ENUM_VALUE( Axis, Z );
	SO_NODE_SET_SF_ENUM_TYPE( activeAxis, Axis );
	SO_NODE_ADD_FIELD( activeAxis, (Z) );

	SO_NODE_ADD_FIELD( axisOrigin, ( 0.0, 0.0 ) );


	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);


}

TrackerLinearFresnel::~TrackerLinearFresnel()
{
}

QString TrackerLinearFresnel::getIcon()
{

	return QString(":/icons/TrackerLinearFresnel.png");
}

void TrackerLinearFresnel::evaluate()
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

	Transform objectToWorld = tgf::TransformFromMatrix( getmatrixAction->getMatrix( ) );
	Transform worldToObject = objectToWorld.GetInverse();

	Vector3D globalSunVector( sin( m_azimuth.getValue() ) * sin( m_zenith.getValue() ),
			 cos( m_zenith.getValue() ),
			-cos( m_azimuth.getValue() ) * sin( m_zenith.getValue() ) );

	Vector3D i = worldToObject( globalSunVector );

	Vector3D localAxis;
	Point3D focus;
	if( activeAxis.getValue() == 0 )
	{
		localAxis  = worldToObject( Vector3D( 1.0, 0.0, 0.0 ) );
		focus = worldToObject( Point3D( 0.0, axisOrigin.getValue()[0], axisOrigin.getValue()[1] ) );
	}
	else if( activeAxis.getValue() == 1 )
	{
		localAxis = worldToObject( Vector3D( 0.0, 1.0, 0.0 ) );
		focus = worldToObject( Point3D( axisOrigin.getValue()[0], 0.0, axisOrigin.getValue()[1] ) );
	}
	else
	{
		focus = worldToObject( Point3D( axisOrigin.getValue()[0], axisOrigin.getValue()[1], 0.0 ) );
		localAxis  = worldToObject( Vector3D( 0.0, 0.0, 1.0 ) );
	}


	double angle = 0.0;
	if( localAxis == Vector3D( 1.0, 0.0, 0.0 ) )
	{
		Vector3D r = Normalize( Vector3D( 0.0, focus.y, focus.z ) );
		Vector3D n = Normalize( i + r );
		if( fabs( n.z ) > 0.0  )	angle = atan2( n.z, n.y );
	}
	else if( localAxis == Vector3D( 0.0, 1.0, 0.0 ) )
	{
		Vector3D r = Normalize( Vector3D( focus.x, 0.0, focus.z ) );
		Vector3D n = Normalize( i + r );
		if( fabs( n.z ) > 0.0  )	angle = - atan2( n.z, n.x );
	}
	else
	{
		Vector3D r = Normalize( Vector3D( focus.x, focus.y, 0.0 ) );
		Vector3D n = Normalize( i + r );

		if( fabs( n.x ) > 0.0  )	angle = -atan2( n.x, n.y );
	}

	SbVec3f axis = SbVec3f( localAxis.x, localAxis.y, localAxis.z );

	SoTransform* newTransform = new SoTransform();
	newTransform->rotation.setValue( axis, angle );

	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( newTransform->translation.getValue() ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( newTransform->rotation.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( newTransform->scaleFactor.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( newTransform->scaleOrientation.getValue() ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( newTransform->center.getValue() ) );

}
