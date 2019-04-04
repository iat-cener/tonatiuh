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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#include "gc.h"
#include "Transform.h"
#include "Vector3D.h"

#include "TDefaultTransmissivity.h"
#include "TSceneKit.h"
#include "TSeparatorKit.h"
#include "TTracker.h"
#include "TTransmissivity.h"

SO_KIT_SOURCE(TSceneKit);

/**
 * Does initialization common for all objects of the TSceneKit class.
 * This includes setting up the type system, among other things.
 */
void TSceneKit::initClass()
{
	SO_KIT_INIT_CLASS(TSceneKit, SoSceneKit, "SceneKit");
}

/**
 * Creates a new TSceneKit node.
 */
TSceneKit::TSceneKit()
{
    SO_KIT_CONSTRUCTOR(TSceneKit);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY( transmissivity, TTransmissivity, TDefaultTransmissivity, TRUE, topSeparator, "", TRUE);


	SO_NODE_ADD_FIELD( azimuth, ( gc::Pi ) );
	SO_NODE_ADD_FIELD( zenith, ( 0.0f ) );

    SO_KIT_INIT_INSTANCE();

}

/**
 * TSceneKit destructor.
 */
TSceneKit::~TSceneKit()
{

}

trt::TONATIUH_REAL* TSceneKit::GetAzimuthAngle()
{
	return ( &azimuth );
}

trt::TONATIUH_REAL* TSceneKit::GetZenithAngle()
{
	return ( &zenith );
}


/*!
 * Returns the path from the scene node to the node in \a action.
 */
SoPath* TSceneKit::GetSoPath( SoSearchAction* action )
{
	TSeparatorKit* sunNode = static_cast< TSeparatorKit* > (getPart( "childList[0]", false ) );
	if( !sunNode )	return NULL;

	TSeparatorKit* rootNode = static_cast< TSeparatorKit* > ( sunNode->getPart( "childList[0]", false ) );
	if( !rootNode )	return NULL;


	action->setInterest( SoSearchAction::FIRST );
	action->apply( rootNode );
	SoPath* nodePath = action->getPath( );
	return nodePath;
}

void TSceneKit::UpdateSunPosition( double azimuthValue, double zenithValue )
{
	azimuth = azimuthValue;
	zenith = zenithValue;

	Vector3D sunVector( sin( azimuth.getValue() ) * sin( zenith.getValue()  ),
			cos( zenith.getValue()  ),
			-sin( zenith.getValue() ) * cos( azimuth.getValue()  ) );


	Transform sceneOTW( 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 );


	SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( getPart( "childList", true ) );
	if( !coinPartList || coinPartList->getNumChildren() < 1 )	return;


	//SunNode
	SoBaseKit* sunNode = static_cast< SoBaseKit* >( coinPartList->getChild( 0 ) );
	if( !sunNode )	return;
	SoNodeKitListPart* sunNodePartList = static_cast< SoNodeKitListPart* >( sunNode->getPart( "childList", true ) );
	if( !sunNodePartList )	return;

	for( int index = 0; index < sunNodePartList->getNumChildren(); ++index )
	{
		SoBaseKit* coinChild = static_cast< SoBaseKit* >( sunNodePartList->getChild( index ) );
		UpdateTrackersTransform( coinChild, sunVector, sceneOTW );
	}

}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneKit::UpdateTrackersTransform( SoBaseKit* branch, Vector3D sunVector, Transform parentOTW )
{
	if( !branch )	return;

	SoNode* tracker = branch->getPart( "tracker", false );
	if( tracker )
	{

		TTracker* trackerNode = static_cast< TTracker* >( tracker );
		trackerNode->Evaluate( sunVector, parentOTW.GetInverse() );
		return;
	}

	if( branch->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		SoTransform* nodeTransform = static_cast< SoTransform* >(branch->getPart( "transform", true ) );
		Transform nodeTransformationOTW = tgf::TransformFromSoTransform( nodeTransform );
		//Transform nodeTransformationWTO = groupNode->GetTrasformation().GetInverse();
		Transform nodeOTW = nodeTransformationOTW * parentOTW;

		SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( branch->getPart( "childList", false ) );
		if ( coinPartList )
		{
			for( int index = 0; index < coinPartList->getNumChildren(); ++index )
			{
				SoBaseKit* coinChild = static_cast< SoBaseKit* >( coinPartList->getChild( index ) );
				if( coinChild )		UpdateTrackersTransform( coinChild, sunVector, nodeOTW );
			}
		}

	}

}
