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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#ifndef TTRACKER_H_
#define TTRACKER_H_

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/nodes/SoTransform.h>

#include "trt.h"
#include "TSceneKit.h"

/*!
 * @class TTracker
 * @brief Abstract base class for all custom sun light trackers in the scene.
 *
 * TTracker defines the interface for trackers that determine how a node
 * should be transformed in response to changes in a light source.
 *
 * Subclasses of TTracker must implement:
 * - Icon representation (`GetIcon`).
 * - Computation of the transformation to perfrom when the light position in chnaged (`Evaluate`).
 *
 * This class serves as a base for custom tracker models and can be extended to implement pecialized tracking behavior.
 */
class TTracker : public SoNodeEngine
{
	SO_NODEENGINE_ABSTRACT_HEADER( TTracker );

public:
    static void initClass();
	virtual std::string GetIcon() = 0;
		
	void Disconnect();
	void SetAzimuthAngle( trt::TONATIUH_REAL* azimuthField );
	void SetZenithAngle( trt::TONATIUH_REAL* zenithField );
	void SetSceneKit( TSceneKit* scene );
	void ConnectParentTranform(SoTransform* parentTransform);

	virtual void Evaluate( Vector3D sunVectorW, Transform parentWT0 );

protected:
	//Constructor
	TTracker();
	virtual ~TTracker(); 


	void SetEngineOutput( SoTransform* newTransform );
	void SetEngineOutputIdentity();
	void SetEngineOutputRotation(SbRotation rotation);
	Vector3D GetGobalSunVector();

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
