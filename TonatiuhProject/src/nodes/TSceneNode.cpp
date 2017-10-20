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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>

#include "TNodesList.h"
#include "Transform.h"
#include "TSceneNode.h"
#include "TSunNode.h"
#include "TTrackerNode.h"
#include "Vector3D.h"

/******************************
 * TSceneNode
 ******************************/

TNodeType TSceneNode::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* TSceneNode::CreateInstance( )
{
  return ( new TSceneNode() );
}


/*!
 * Initializes TGroupNode type.
 */
void TSceneNode::Init()
{
	m_nodeType = TNodeType::CreateType( TNodeType::FromName( "ContainerNode" ), "SceneNode", &TSceneNode::CreateInstance );
}

/*!
 * TGroupNode : public TNode
 */
TSceneNode::TSceneNode()
:TContainerNode(),
 m_childrenListName( "childrenList" ),
 m_lightName( "light" ),
 m_transmisivityName( "transmisivity" )
{
	//setObjectName(GetType().GetName().c_str() );
	SetName( GetType().GetName() );

	//Parts
	AppendPart( m_lightName, TNodeType::FromName( "SunNode" ) , 0  );
	AppendPart( m_transmisivityName, TNodeType::FromName( "TransmisivitNode" ) , 0  );
	AppendPart( m_childrenListName, TNodeType::FromName( "GroupNode" ) , 0  );
}

/*!
 * Destructor.
 */
TSceneNode::~TSceneNode()
{
	SetPart( m_lightName, 0 );
	SetPart( m_childrenListName, 0 );
}


/*
 * If sun node is defined in the scene, changes the sun node position to the coordinates defined by the angles \a azimuth and zenith. These angles are in radians.
 * Updates the sun and the trackers.
 */
void TSceneNode::ChangeSunPosition( double azimuth, double zenith )
{
	TSunNode* sunNode = m_partsList[m_lightName]->as<TSunNode>();
	if( !sunNode )	return;

	sunNode->ChangeSunPosition( azimuth, zenith );
	UpdateTrackers( azimuth, zenith );

}

/*!
 * Returns icon file name
 */
std::string TSceneNode::GetIcon() const
{
	return ( ":/icons/tscenenode.png" );
}

/*!
 * Returns the type of node.
 */
TNodeType TSceneNode::GetType() const
{
	return ( TSceneNode::m_nodeType );
}

/*!
 * Replaces the node of the part \a name with \a node. If the part does not exit, false is returned.
 * The previous node of the part is not destroyed.
 *
 */
bool TSceneNode::SetPart( const std::string name, TNode* node  )
{
	if( !TContainerNode::SetPart( name, node  ) ) return ( false );
	return ( true );
}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneNode::UpdateTrackersTransform( TNode* branch, Vector3D sunVector, Transform parentWT0 )
{
	if( !branch )	return;

	if( TTrackerNode* trackerNode = branch->as<TTrackerNode>() )
	{
		trackerNode->UpdateTrackerTransform( sunVector, parentWT0 );
		return;
	}

	if( const TGroupNode* groupNode = branch->as<TGroupNode>() )
	{
		Transform nodeTransformationWTO = groupNode->GetTrasformation().GetInverse();
		Transform nodeWTO = nodeTransformationWTO * parentWT0;

		TNodesList* listNode = groupNode->GetPart( m_childrenListName )->as<TNodesList>();
		if( !listNode )	return;

		int numberOfChildren = listNode->Count();
		for( int l = 0; l < numberOfChildren; l++ )
		{
			TNode* childNode = listNode->Item( l );
			if( childNode )	UpdateTrackersTransform( childNode, sunVector, nodeWTO );

		}
	}
}

/*!
 * Updates all trackers transform for the current sun angles.
 */
void TSceneNode::UpdateTrackers( double azimuth, double zenith )
{
	Vector3D sunVector( sin( azimuth ) * sin( zenith ),
			cos( zenith ),
			-sin( zenith ) * cos( azimuth ) );


	Transform sceneOTW( 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 );


	TNode* childrenRootNode = GetPart( m_childrenListName );
	if( !childrenRootNode )	return;

	UpdateTrackersTransform( childrenRootNode, sunVector, sceneOTW );

}
