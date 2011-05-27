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

#ifndef TRF_H_
#define TRF_H_

#include <QMap>
#include <QPair>

#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "Photon.h"
#include "RayTracerPhoton.h"
#include "TPhotonMap.h"
#include "Ray.h"
#include "TShape.h"
#include "TSunShape.h"
#include "Transform.h"
#include "TSeparatorKit.h"
#include "TAnalyzerKit.h"
#include "TShapeKit.h"

class InstanceNode;
class RandomDeviate;
class TPhotonMap;

namespace trf
{
	void ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO,QVector< QPair< TShapeKit*, Transform > >* surfacesList, bool insertInSurfaceList );
	void CreatePhotonMap( TPhotonMap*& photonMap, QPair< TPhotonMap* , std::vector< RayTracerPhoton > > photonsList );

	int ExportAll( QString fileName , double wPhoton, TPhotonMap* photonMap );
	int ExportSurfaceGlobalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap );
	int ExportSurfaceLocalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap );
	SoSeparator* DrawPhotonMapPoints( const TPhotonMap& map);
	SoSeparator* DrawPhotonMapRays( const TPhotonMap& map, unsigned long numberOfRays, double fraction );
}


/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 *The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
inline void trf::ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO,QVector< QPair< TShapeKit*, Transform > >* surfacesList, bool insertInSurfaceList )
{

	if( !instanceNode ) return;
	SoBaseKit* coinNode = static_cast< SoBaseKit* > ( instanceNode->GetNode() );
	if( !coinNode ) return;

	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart( "transform", true ) );
		Transform objectToWorld = tgf::TransformFromSoTransform( nodeTransform );
		Transform worldToObject = objectToWorld.GetInverse();

		BBox nodeBB;
		Transform nodeWTO(worldToObject * parentWTO );
		instanceNode->SetIntersectionTransform( nodeWTO );

		bool insertChildInSurfaceList=insertInSurfaceList;
		if( coinNode->getTypeId().isDerivedFrom( TAnalyzerKit::getClassTypeId() ) )
		{
			insertChildInSurfaceList = false;
		}
		for( int index = 0; index < instanceNode->children.count() ; ++index )
		{
			InstanceNode* childInstance = instanceNode->children[index];
			ComputeSceneTreeMap(childInstance, nodeWTO, surfacesList, insertChildInSurfaceList );

			nodeBB = Union( nodeBB, childInstance->GetIntersectionBBox() );
		}

		instanceNode->SetIntersectionBBox( nodeBB );

	}
	else if (coinNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId()))
	{
		Transform shapeTransform;
		Transform shapeToWorld;

		SoTransform* nodeTransform = static_cast< SoTransform* >(((SoBaseKit*)coinNode)->getPart( "transform", false ) );
		if (nodeTransform)
		{
			shapeToWorld = tgf::TransformFromSoTransform( nodeTransform );
			Transform worldToObject = shapeToWorld.GetInverse();
			shapeTransform = worldToObject * parentWTO ;
		}
		else
		{
			shapeTransform = parentWTO;
			shapeToWorld = shapeTransform.GetInverse();
		}


		BBox shapeBB;

		if(  instanceNode->children.count() > 0 )
		{
			InstanceNode* shapeInstance = 0;
			if( instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
				shapeInstance =  instanceNode->children[0];
			else if(  instanceNode->children.count() > 1 )	shapeInstance =  instanceNode->children[1];

			if( shapeInstance )
			{
				TShape* shapeNode = static_cast< TShape* > ( shapeInstance->GetNode() );
				shapeBB = shapeToWorld( shapeNode->GetBBox() );

				instanceNode->SetIntersectionTransform( shapeTransform );
				instanceNode->SetIntersectionBBox( shapeBB );

				if (insertInSurfaceList)
				{
					TShapeKit* surface = static_cast< TShapeKit* > ( coinNode );
					surfacesList->push_back( QPair< TShapeKit*, Transform >( surface, shapeTransform ) );
				}
			}
		}

	}
}


inline void trf::CreatePhotonMap( TPhotonMap*& photonMap, QPair< TPhotonMap* , std::vector< RayTracerPhoton > > photonsList )
{
	if( !photonMap )  photonMap = photonsList.first;

	std::vector< RayTracerPhoton >::iterator it;
	it = photonsList.second.begin();

	int rayLength;
	while( it < photonsList.second.end() )
	{
		rayLength = 1;
		Photon* first = new Photon( it->pos, it->shapeFront, 0, 0, 0, it->intersectedSurface );
		Photon* nextPhoton = first;

		while( (++it)<photonsList.second.end() && ( it->id > 0 ) )
		{
			Photon* photon = new Photon( it->pos, it->shapeFront, nextPhoton, 0, 0, it->intersectedSurface );

			nextPhoton->next = photon;
			nextPhoton = photon;

			rayLength++;
		}
		photonMap->StoreRay( first, rayLength );
	}
	photonsList.second.clear();
}

#endif /* TRF_H_ */
