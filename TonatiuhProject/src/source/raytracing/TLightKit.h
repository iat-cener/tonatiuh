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

#ifndef TLIGHTKIT_H_
#define TLIGHTKIT_H_

#include <QDateTime>
#include <QPair>
#include <QVector>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodekits/SoLightKit.h>


#include "trt.h"
#include "TSunShape.h"

struct BBox;
class Transform;
class TShapeKit;

/*!
  \class TLightKit TLightKit.h TLightKit.h
  \brief The TLightKit class provides a kit with a transform, a light, a shape and a sunshape to represents the sun light.


  The TLightKit structure:
  \verbatim
  CLASS TLightKit
  -->"this"
        "callbackList"
  -->   "transformGroup"
  -->      "transform"
  -->      "light"
  -->      "iconSeparator"
  -->         "icon"
  -->      "tsunshape"
  \endverbatim

  Detailed information on catalog parts:
  \verbatim
  CLASS TLightKit
  PVT   "this",  TLightKit  ---
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ]
  PVT   "transformGroup",  SoTransformSeparator  ---
        "transform",  SoTransform  ---
        "light",  SoLight  --- , (default type = SoDirectionalLight)
  PVT   "iconSeparator",  SoSeparator  ---
        "icon",  SoNode  --- , (default type = SoCube)
        "tsunshape",  TSunShape  --- , (default type = TDefaultSunShape)
  \endverbatim

*/

class TLightKit : public SoLightKit
{
    SO_KIT_HEADER(TLightKit);
 	SO_KIT_CATALOG_ENTRY_HEADER(iconMaterial);
 	SO_KIT_CATALOG_ENTRY_HEADER(iconTexture);
 	SO_KIT_CATALOG_ENTRY_HEADER(tsunshape);



public:
    TLightKit();
    static void initClass();

    void ChangePosition( double newAzimuth, double newZenith );

    void Update( BBox box );
    void ComputeLightSourceArea( int widthDivisions,int heightDivisions,QVector< QPair< TShapeKit*, Transform > > surfacesList );

    trt::TONATIUH_REAL azimuth;
    trt::TONATIUH_REAL zenith;
    SoSFString disabledNodes;

private:
    virtual ~TLightKit();
    void UpdateSunPosition();


};

#endif /*TLIGHTKIT_H_*/
