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
 
#ifndef TTRACKER_H_
#define TTRACKER_H_

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/nodes/SoTransform.h>

#include "trt.h"
#include "Vector3D.h"

class QString; 
class TSceneKit;
class TLightKit;
class Transform;
class Vector3D;


class TTracker : public SoNodeEngine
{
	SO_NODEENGINE_ABSTRACT_HEADER( TTracker );

public:
    static void initClass();
	virtual QString getIcon() = 0;
		
	void Disconnect();
	//void SetLightAngles(TLightKit * coinLight );
	void SetAzimuthAngle( trt::TONATIUH_REAL* azimuthField );
	void SetZenithAngle( trt::TONATIUH_REAL* zenithField );
	void SetSceneKit( TSceneKit* scene );
	//void SetAnglesToScene();
	void ConnectParentTranform(SoTransform* parentTransform);
	//double GetAzimuth() { return m_azimuth.getValue();};
	//double GetZenith() { return m_zenith.getValue();};

	virtual void Evaluate( Vector3D sunVectorW, Transform parentWT0 );

protected:
	//Constructor
	TTracker();
	virtual ~TTracker(); 


	void SetEngineOutput( SoTransform* newTransform );
	void SetEngineOutputIdentity();
	void SetEngineOutputRotation(SbRotation rotation);
	Vector3D GetGobalSunVector();
	//bool IsConnected();

	trt::TONATIUH_REAL m_azimuth;
	TSceneKit* m_scene;
	trt::TONATIUH_REAL m_zenith;

	SoEngineOutput  outputTranslation;
	SoEngineOutput  outputRotation;
	SoEngineOutput  outputScaleFactor;
	SoEngineOutput  outputScaleOrientation ;
	SoEngineOutput  outputCenter;

private:
	virtual void evaluate() = 0;

};

#endif /*TTRACKER_H_*/
