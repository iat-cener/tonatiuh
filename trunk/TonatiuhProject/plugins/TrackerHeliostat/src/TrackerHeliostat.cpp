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
#include "TrackerHeliostat.h"
#include "Transform.h"
#include "TSceneKit.h"
#include "Vector3D.h"
#include "trf.h"

SO_NODEENGINE_SOURCE( TrackerHeliostat );

void TrackerHeliostat::initClass()
{
	TTracker::initClass();
	TTrackerForAiming::initClass();
	SO_NODEENGINE_INIT_CLASS( TrackerHeliostat, TTrackerForAiming, "TTrackerForAiming" );
}

TrackerHeliostat::TrackerHeliostat()
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerHeliostat );
	// Define input fields and their default values
	/*SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );*/
	SO_NODE_ADD_FIELD( aimingPoint, ( 0.0, 0.0, 0.0 ) );

	SO_NODE_DEFINE_ENUM_VALUE( Rotations, YX );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, YZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, XZ );
	SO_NODE_DEFINE_ENUM_VALUE( Rotations, ZX );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfRotation, Rotations );
	SO_NODE_ADD_FIELD( typeOfRotation, (YX) );

	//ConstructEngineOutput();
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

	if (!IsConnected()) return;
	SoPath* nodePath= m_scene->GetSoPath(this );
	if (!nodePath) return;
	Transform objectToWorld = trf::GetObjectToWorld(nodePath);

	Transform worldToObject = objectToWorld.GetInverse();

	Vector3D i = worldToObject.multDirMatrix ( GetGobalSunVector() );
	if (i.length() == 0.0f) return;
	i = Normalize(i);

	Vector3D focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );
	Vector3D r;
	if (typeOfAimingPoint.getValue() == 0) //Absolute
	{
		r = worldToObject.multVecMatrix( focus );
	}
	else
	{
		r= focus;
	}
	if (r.length() == 0.0f) return;
	r = Normalize(r);

	Vector3D n = ( i + r );
	if (n.length() == 0.0f) return;
	n = Normalize(n);

	Vector3D Axe1;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 1 ))// YX or YZ
	{
		Axe1 = Vector3D(0.0f, 1.0f, 0.0f);
	}
	else if (typeOfRotation.getValue() == 2 ) // XZ
	{
		Axe1 = Vector3D(1.0f, 0.0f, 0.0f);
	}
	else // ZX
	{
		Axe1 = Vector3D(0.0f, 0.0f, 1.0f);
	}
	Vector3D t = CrossProduct(n, Axe1);
	//Vector3D t( n[2], 0.0f, -n[0] );
	if (t.length() == 0.0f) return;
	t = Normalize(t);

	Vector3D p = CrossProduct(t,n);
	if (p.length() == 0.0f) return;
	p = Normalize(p);


	SbMatrix transformMatrix;
	if ((typeOfRotation.getValue() == 0 ) || (typeOfRotation.getValue() == 3 ))// YX ou  ZX
	{
		 transformMatrix = SbMatrix( t[0], t[1], t[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  p[0], p[1], p[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}
	else // YZ
	{
		transformMatrix = SbMatrix( p[0], p[1], p[2], 0.0,
								  n[0], n[1], n[2], 0.0,
								  t[0], t[1], t[2], 0.0,
								  0.0, 0.0, 0.0, 1.0 );
	}



	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);

}

void TrackerHeliostat::SwitchAimingPointType()
{
	SoPath* nodePath= m_scene->GetSoPath( this );
	if (!nodePath) return;
	Transform objectToWorld = trf::GetObjectToWorld(nodePath);

	Vector3D focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );
	
	if (typeOfAimingPoint.getValue() == 1)
	{
		Transform worldToObject = objectToWorld.GetInverse();
		Vector3D r = worldToObject.multVecMatrix( focus );
		aimingPoint.setValue(r[0],r[1],r[2]);
	}
	else
	{
		Vector3D r = objectToWorld.multVecMatrix( focus );
		aimingPoint.setValue(r[0],r[1],r[2]);
	}
}
