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

#ifndef TLIGHTKIT_H_
#define TLIGHTKIT_H_

#include <QDateTime>
#include <QPair>
#include <QVector>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodekits/SoLightKit.h>


#include "trt.h"
#include "TSunShape.h"

class BBox;
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
    //SO_KIT_CATALOG_ENTRY_HEADER(iconHints);
 	SO_KIT_CATALOG_ENTRY_HEADER(iconTexture);
 	SO_KIT_CATALOG_ENTRY_HEADER(tsunshape);



public:
    TLightKit();
    static void initClass();

    //void ChangePosition( QDateTime newTime, double longitude, double latitude );
    void ChangePosition( double newAzimuth, double newZenith/*, double newDistance*/ );
    void GetPositionData( QDateTime* time, double* longitude, double* latitude );

    void Update( BBox box );
    void ComputeLightSourceArea( QVector< QPair< TShapeKit*, Transform > > surfacesList );

    trt::TONATIUH_REAL azimuth;
    trt::TONATIUH_REAL zenith;

private:
    virtual ~TLightKit();
    void UpdateSunPosition();

    QDateTime m_time;
    SoSFDouble m_longitude;
    SoSFDouble m_latitude;


};

#endif /*TLIGHTKIT_H_*/
