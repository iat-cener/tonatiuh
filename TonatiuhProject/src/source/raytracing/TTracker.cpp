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

#include <QString>

#include "Transform.h"

#include "TSceneKit.h"
#include "TTracker.h"
#include "TLightKit.h"
#include "tgc.h"


SO_NODEENGINE_ABSTRACT_SOURCE( TTracker );

void TTracker::initClass()
{
	SO_NODEENGINE_INIT_ABSTRACT_CLASS( TTracker, SoNodeEngine, "NodeEngine" );
}

TTracker::TTracker()
:m_scene( 0 )
{
}

TTracker::~TTracker()
{
	m_scene = 0;
	//m_azimuth.disconnect();
	//m_zenith.disconnect();
}


void TTracker::Disconnect()
{
	m_azimuth.disconnect();
	m_zenith.disconnect();
}

/*
void TTracker::SetLightAngles(TLightKit * coinLight )
{
	m_azimuth.connectFrom( &(coinLight->azimuth) );
	m_zenith.connectFrom( &(coinLight->zenith) );
}
*/


void TTracker::SetAzimuthAngle( trt::TONATIUH_REAL* azimuthField )
{
	m_azimuth.connectFrom( azimuthField );
}



void TTracker::SetZenithAngle( trt::TONATIUH_REAL* zenithField )
{
	m_zenith.connectFrom( zenithField );
}

void TTracker::SetSceneKit( TSceneKit* scene )
{
	m_scene = scene;
}

void TTracker::ConnectParentTranform(SoTransform* parentTransform)
{
	parentTransform->translation.connectFrom( &outputTranslation );
	parentTransform->rotation.connectFrom( &outputRotation );
	parentTransform->scaleFactor.connectFrom( &outputScaleFactor );
	parentTransform->scaleOrientation.connectFrom( &outputScaleOrientation );
	parentTransform->center.connectFrom( &outputCenter );
}


void TTracker::Evaluate( Vector3D /*sunVector*/, Transform /*parentWT0*/ )
{

}

void TTracker::SetEngineOutput(SoTransform* newTransform)
{
	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( newTransform->translation.getValue() ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( newTransform->rotation.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( newTransform->scaleFactor.getValue() ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( newTransform->scaleOrientation.getValue() ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( newTransform->center.getValue() ) );
}

void TTracker::SetEngineOutputRotation(SbRotation rotation)
{
	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( SbVec3f( 0.0, 0.0, 0.0 ) ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( rotation ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( SbVec3f( 1.0, 1.0, 1.0 ) ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( SbRotation() ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( SbVec3f( 0.0, 0.0, 0.0 ) ) );
}

void TTracker::SetEngineOutputIdentity()
{
	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( 0.0, 0.0, 0.0 ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( 0.0, 0.0, 1.0, 0.0 ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( 1.0, 1.0, 1.0 ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( 0.0, 0.0, 1.0, 0.0 ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( 0.0, 0.0, 0.0 ) );
}

/*
bool TTracker::IsConnected()
{
	if( !m_azimuth.isConnected() ) return false;
	if( !m_zenith.isConnected() ) return false;
	return true;
}
*/

/*
Vector3D TTracker::GetGobalSunVector()
{
	Vector3D globalSunVector( sin( m_azimuth.getValue() ) * sin( m_zenith.getValue() ),
				 cos( m_zenith.getValue() ),
				-cos( m_azimuth.getValue() ) * sin( m_zenith.getValue() ) );
	return globalSunVector;
}

/*
void TTracker::SetAnglesToScene()
{
	if( m_scene )
	{
		m_scene->azimuth.setValue( m_azimuth.getValue() );
		m_scene->zenith.setValue( m_zenith.getValue() );
	}
}
*/
