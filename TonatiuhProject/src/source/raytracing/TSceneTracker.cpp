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
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransform.h>

#include "gc.h"

#include "NormalVector.h"
#include "Point3D.h"
#include "Transform.h"
#include "TSceneTracker.h"
#include "TSceneKit.h"
#include "Vector3D.h"

SO_NODEENGINE_SOURCE( TSceneTracker );

void TSceneTracker::initClass()
{
	SO_NODEENGINE_INIT_CLASS( TSceneTracker, TTracker, "TTracker" );

}

TSceneTracker::TSceneTracker()
{
	SO_NODEENGINE_CONSTRUCTOR( TSceneTracker );

	// Define input fields and their default values
	SO_NODE_ADD_FIELD( m_azimuth, ( 0.0 ) );
	SO_NODE_ADD_FIELD( m_zenith, ( 90.0 ) );

	//ConstructEngineOutput();
	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);


}

TSceneTracker::~TSceneTracker()
{
	m_azimuth.disconnect();
	m_zenith.disconnect();
}

QString TSceneTracker::getIcon()
{

	return QLatin1String(":/icons/TSceneTracker.png");
}

void TSceneTracker::evaluate()
{

	if ( !m_azimuth.isConnected() || !m_zenith.isConnected() ) return;
	//SetAnglesToScene();

	double azimuth = m_azimuth.getValue();
	double zenith = m_zenith.getValue();

	m_scene->UpdateSunPosition( azimuth, zenith );


	//m_scene->azimuth.setValue( azimuth );

	//double alpha = gc::Pi - GetAzimuth();
	double alpha = gc::Pi - azimuth;

	SbVec3f yAxis( 0.0, 1.0, 0.0 );
	SbRotation yRotation( yAxis, -alpha );

	SbVec3f xAxis( 1.0, 0.0, 0.0 );
	//SbRotation xRotation( xAxis, -GetZenith() );
	//m_scene->zenith.setValue( zenith );
	SbRotation xRotation( xAxis, -zenith );

	SbRotation rotation = yRotation * xRotation;

	SetEngineOutputRotation(rotation);

}
