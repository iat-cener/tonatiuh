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

#include <cmath>

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "gc.h"
#include "Transform.h"

#include "TrackerOneAxis.h"
#include "trf.h"
#include "TSeparatorKit.h"
#include "TSceneKit.h"

SO_NODEENGINE_SOURCE( TrackerOneAxis );

void TrackerOneAxis::initClass()
{
	//TTracker::initClass();
	SO_NODEENGINE_INIT_CLASS( TrackerOneAxis, TTracker, "TTracker" );

}

TrackerOneAxis::TrackerOneAxis()
{
	SO_NODEENGINE_CONSTRUCTOR( TrackerOneAxis );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( gc::Pi ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 0.0 ) );

	//ConstructEngineOutput();
	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);

}

TrackerOneAxis::~TrackerOneAxis()
{
}

QString TrackerOneAxis::getIcon()
{
	return QString(":/icons/TrackerOneAxis.png");
}

void TrackerOneAxis::Evaluate( Vector3D sunVectorW, Transform parentWT0 )
{
	Vector3D s = parentWT0( sunVectorW );
	Vector3D p( 1.0f, 0.0f, 0.0f);


	Vector3D n;
	Vector3D t;
	if( fabs( DotProduct( s, p ) ) < 1.0 )
	{
		//n = ( s - ( DotProduct( s, p )*  p ) ) / ( sqrt( 1 - DotProduct( s, p ) * DotProduct( s, p ) ) );
	//	n = Normalize( s - ( DotProduct( s, p )*  p ) ) / ( sqrt( 1 - DotProduct( s, p ) * DotProduct( s, p ) ) );
		//t = Normalize( CrossProduct( p, n ) );
		t = Normalize( CrossProduct( p, s ) );
		n = Normalize( CrossProduct( t, p ) );
	}
	else
	{
		t = Vector3D( 0.0f, 0.0f, 1.0f );
		n = Normalize( CrossProduct( t, p ) );
	}


	SbMatrix transformMatrix( t[0], t[1], t[2], 0.0,
								n[0], n[1], n[2], 0.0,
								p[0], p[1], p[2], 0.0,
								0.0, 0.0, 0.0, 1.0 );

	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);
}

void TrackerOneAxis::evaluate()
{

	/*if (!IsConnected()) return;

	SoSearchAction coinSearch;
	coinSearch.setNode( this );

	SoPath* nodePath = m_scene->GetSoPath( &coinSearch );
	if( !nodePath || nodePath == 0 || nodePath->getLength() < 1)
		return;
	SoNodeKitPath* parentPath = static_cast< SoNodeKitPath* >( nodePath );
	parentPath->pop();

	if( !parentPath ) return;

	Transform objectToWorld = trf::GetObjectToWorld( parentPath );
	Transform worldToObject = objectToWorld.GetInverse();

	Vector3D s = worldToObject( GetGobalSunVector() );
	Vector3D p( 1.0f, 0.0f, 0.0f);

	Vector3D n;
	Vector3D t;
	if( fabs( DotProduct( s, p ) ) < 1.0 )
	{
		//n = ( s - ( DotProduct( s, p )*  p ) ) / ( sqrt( 1 - DotProduct( s, p ) * DotProduct( s, p ) ) );
	//	n = Normalize( s - ( DotProduct( s, p )*  p ) ) / ( sqrt( 1 - DotProduct( s, p ) * DotProduct( s, p ) ) );
		//t = Normalize( CrossProduct( p, n ) );
		t = Normalize( CrossProduct( p, s ) );
		n = Normalize( CrossProduct( t, p ) );
	}
	else
	{
		t = Vector3D( 0.0f, 0.0f, 1.0f );
		n = Normalize( CrossProduct( t, p ) );
	}


	SbMatrix transformMatrix( t[0], t[1], t[2], 0.0,
								n[0], n[1], n[2], 0.0,
								p[0], p[1], p[2], 0.0,
								0.0, 0.0, 0.0, 1.0 );

	SoTransform* newTransform = new SoTransform();
	newTransform->setMatrix( transformMatrix );

	SetEngineOutput(newTransform);
*/
}
