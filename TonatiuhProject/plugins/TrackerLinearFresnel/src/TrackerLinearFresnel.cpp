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

#include <Inventor/nodes/SoTransform.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "trf.h"
#include "TrackerLinearFresnel.h"
#include "Transform.h"
#include "TSceneKit.h"
#include "TSeparatorKit.h"
#include "Vector3D.h"


SO_NODEENGINE_SOURCE( TrackerLinearFresnel );

void TrackerLinearFresnel::initClass()
{
	SO_NODEENGINE_INIT_CLASS( TrackerLinearFresnel, TTrackerForAiming, "TTrackerForAiming" );
}

TrackerLinearFresnel::TrackerLinearFresnel()
:m_previousAimingPointType( 0 ),
 m_infoDisplayed( 0 )
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerLinearFresnel );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );


	// Define input fields and their default values
	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Absolute );
	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Relative );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfAimingPoint, AimingPointType );
	SO_NODE_ADD_FIELD( typeOfAimingPoint, (Absolute) );
	m_infoDisplayed = new SoFieldSensor( TTrackerForAiming::updateTypeOfAimingPoint, this );
	m_infoDisplayed->setPriority( 1 );
	m_infoDisplayed->attach( &typeOfAimingPoint );

	SO_NODE_DEFINE_ENUM_VALUE( Axis, X );
	SO_NODE_DEFINE_ENUM_VALUE( Axis, Y );
	SO_NODE_DEFINE_ENUM_VALUE( Axis, Z );
	SO_NODE_SET_SF_ENUM_TYPE( activeAxis, Axis );
	SO_NODE_ADD_FIELD( activeAxis, (Z) );

	SO_NODE_ADD_FIELD( axisOrigin, ( 0.0, 0.0 ) );



	//ConstructEngineOutput();
	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);

}

TrackerLinearFresnel::~TrackerLinearFresnel()
{
	delete m_infoDisplayed;
	m_infoDisplayed = 0;
}

QString TrackerLinearFresnel::getIcon()
{

	return QString(":/icons/TrackerLinearFresnel.png");
}

void TrackerLinearFresnel::evaluate()
{
	if (!IsConnected()) return;

	SoSearchAction coinSearch;
	coinSearch.setNode( this );

	SoPath* nodePath = m_scene->GetSoPath( &coinSearch );
	if( !nodePath || nodePath == 0 || nodePath->getLength() < 1)
		return;

	SoNodeKitPath* parentPath = static_cast<SoNodeKitPath*>( nodePath->copy() );
	parentPath->pop();
	Transform objectToWorld = trf::GetObjectToWorld( parentPath );
	Transform worldToObject = objectToWorld.GetInverse();

	Vector3D i = worldToObject( GetGobalSunVector() );

	Vector3D localAxis;
	Point3D focusPoint;
	if( activeAxis.getValue() == 0 )
	{
		localAxis  =  Vector3D( 1.0, 0.0, 0.0 ) ;
		focusPoint = Point3D( 0.0, axisOrigin.getValue()[0], axisOrigin.getValue()[1] ) ;
	}
	else if( activeAxis.getValue() == 1 )
	{
		localAxis =  Vector3D( 0.0, 1.0, 0.0 );
		focusPoint = Point3D( axisOrigin.getValue()[0], 0.0, axisOrigin.getValue()[1] ) ;
	}
	else
	{
		localAxis  = Vector3D( 0.0, 0.0, 1.0 ) ;
		focusPoint = Point3D( axisOrigin.getValue()[0], axisOrigin.getValue()[1], 0.0 ) ;
	}
	
	Vector3D focus = Vector3D( focusPoint );
	if (typeOfAimingPoint.getValue() == 0 ) //Absolute
	{
		localAxis  = worldToObject( localAxis );
		focus = Vector3D( worldToObject( focusPoint ) );
	}


	double angle = 0.0;
	//Dawann : in a Fresnel concentrator we use the project of the sun vector on the normal plan of the axis
	//it= the projection of the sun on the normal plan of the axis...
	if( localAxis == Vector3D( 1.0, 0.0, 0.0 ) )
	{
		Vector3D r = Normalize( Vector3D( 0.0, focus.y, focus.z ) );
		Vector3D it = Normalize( Vector3D( 0.0, i.y, i.z ) );
		Vector3D n = Normalize( it + r );
		if( fabs( n.z ) > 0.0  )	angle = atan2( n.z, n.y );
	}
	else if( localAxis == Vector3D( 0.0, 1.0, 0.0 ) )
	{
		Vector3D r = Normalize( Vector3D( focus.x, 0.0, focus.z ) );
		Vector3D it = Normalize( Vector3D( i.x, 0.0, i.z ) );
		Vector3D n = Normalize( it + r );
		if( fabs( n.z ) > 0.0  )	angle = - atan2( n.z, n.x );
	}
	else
	{
		Vector3D r = Normalize( Vector3D( focus.x, focus.y, 0.0 ) );
		Vector3D it = Normalize( Vector3D( i.x, i.y, 0.0 ) );
		Vector3D n = Normalize( it + r );
		if( fabs( n.x ) > 0.0  )	angle = -atan2( n.x, n.y );
	}

	SbVec3f axis = SbVec3f( localAxis.x, localAxis.y, localAxis.z );

	SoTransform* newTransform = new SoTransform();
	newTransform->rotation.setValue( axis, angle );

	SetEngineOutput(newTransform);
}


void TrackerLinearFresnel::SwitchAimingPointType()
{
	if( m_previousAimingPointType == typeOfAimingPoint.getValue() )	return;

	SoSearchAction coinSearch;
	coinSearch.setNode( this );

	SoPath* nodePath = m_scene->GetSoPath( &coinSearch );
	if( !nodePath || nodePath == 0 || nodePath->getLength() < 1)
		return;
	Transform objectToWorld = trf::GetObjectToWorld(nodePath);

	Point3D focus;
	if( activeAxis.getValue() == 0 )	focus =  Point3D( 0.0, axisOrigin.getValue()[0], axisOrigin.getValue()[1] ) ;
	else if( activeAxis.getValue() == 1 )	focus =  Point3D( axisOrigin.getValue()[0], 0.0, axisOrigin.getValue()[1] ) ;
	else	focus =  Point3D( axisOrigin.getValue()[0], axisOrigin.getValue()[1], 0.0 );


	Point3D r;
	if( typeOfAimingPoint.getValue() == 1 )
	{
		Transform worldToObject = objectToWorld.GetInverse();
		r = worldToObject( focus );
	}
	else
		r = objectToWorld( focus );


	if( activeAxis.getValue() == 0 )	axisOrigin.setValue( r.y, r.z );
	else if( activeAxis.getValue() == 1 )	axisOrigin.setValue( r.x, r.z );
	else	axisOrigin.setValue( r.x, r.y );

	m_previousAimingPointType = typeOfAimingPoint.getValue();
}
