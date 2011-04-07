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

#include <Inventor/SbLinear.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransform.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "tgc.h"
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

	SO_NODEENGINE_ADD_OUTPUT( outputTranslation, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputRotation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleFactor, SoSFVec3f);
	SO_NODEENGINE_ADD_OUTPUT( outputScaleOrientation, SoSFRotation);
	SO_NODEENGINE_ADD_OUTPUT( outputCenter, SoSFVec3f);


}

TSceneTracker::~TSceneTracker()
{
}

QString TSceneTracker::getIcon()
{

	return QString(":/icons/TSceneTracker.png");
}

void TSceneTracker::evaluate()
{

	if( !m_azimuth.isConnected() ) return;
	if( !m_zenith.isConnected() ) return;

	if( m_scene )
	{
		m_scene->azimuth.setValue( m_azimuth.getValue() );
		m_scene->zenith.setValue( m_zenith.getValue() );
	}

	double alpha = tgc::Pi - m_azimuth.getValue();

	SbVec3f yAxis( 0.0, 1.0, 0.0 );
	SbRotation yRotation( yAxis, -alpha );

	SbVec3f xAxis( 1.0, 0.0, 0.0 );
	SbRotation xRotation( xAxis, -m_zenith.getValue() );

	SbRotation rotation = yRotation * xRotation;


	SO_ENGINE_OUTPUT( outputTranslation, SoSFVec3f, setValue( SbVec3f( 0.0, 0.0, 0.0 ) ) );
	SO_ENGINE_OUTPUT( outputRotation, SoSFRotation, setValue( rotation ) );
	SO_ENGINE_OUTPUT( outputScaleFactor, SoSFVec3f, setValue( SbVec3f( 1.0, 1.0, 1.0 ) ) );
	SO_ENGINE_OUTPUT( outputScaleOrientation, SoSFRotation, setValue( SbRotation() ) );
	SO_ENGINE_OUTPUT( outputCenter, SoSFVec3f, setValue( SbVec3f( 0.0, 0.0, 0.0 ) ) );

}
