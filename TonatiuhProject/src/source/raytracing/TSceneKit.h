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

#ifndef TSCENEKIT_H_
#define TSCENEKIT_H_

#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/actions/SoSearchAction.h>
#include "TSeparatorKit.h"
#include "trt.h"
#include "tgf.h"

class QString;
class Vector3D;
class Transform;

class TSceneKit : public SoSceneKit
{
	typedef SoSceneKit inherited;

    SO_KIT_HEADER( TSceneKit );
 	SO_KIT_CATALOG_ENTRY_HEADER( transmissivity );

public:
 	TSceneKit();
    static void initClass();

    trt::TONATIUH_REAL* GetAzimuthAngle();
    trt::TONATIUH_REAL* GetZenithAngle();

	SoPath* GetSoPath( SoSearchAction* action );

	void UpdateSunPosition( double azimuthValue, double zenithValue );

    trt::TONATIUH_REAL azimuth;
    trt::TONATIUH_REAL zenith;
protected:
    virtual ~TSceneKit();
    void UpdateTrackersTransform( SoBaseKit* branch, Vector3D sunVector, Transform parentOTW );
};


#endif /* TSCENEKIT_H_ */
