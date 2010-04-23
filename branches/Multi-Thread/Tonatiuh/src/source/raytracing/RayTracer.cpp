/*
 * RayTracer.cpp
 *
 *  Created on: 22/04/2010
 *      Author: amutuberria
 */

#include <QTime>

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Photon.h"
#include "RandomDeviate.h"
#include "RandomDeviateFactory.h"
#include "Ray.h"
#include "RayTracer.h"
#include "TPhotonMap.h"
#include "TPhotonMapFactory.h"
#include "TShape.h"
#include "TSunShape.h"

RayTracer::RayTracer( InstanceNode* rootNode,
	       TShape* lightShape,
	       TSunShape* const lightSunShape,
	       Transform lightToWorld,
	       RandomDeviateFactory& randFactory)
:m_rootNode( rootNode ),
m_lightShape( static_cast< TShape*> ( lightShape->copy() )),
m_lightSunShape( static_cast< TSunShape* > ( lightSunShape->copy() ) ),
m_lightToWorld( lightToWorld ),
m_pRand( randFactory.CreateRandomDeviate() )
{

}

std::vector< Photon* > RayTracer::operator()( QPair< double, QMap< InstanceNode*,QPair< BBox, Transform* > >*  >  data )
{
	double numberOfPhotons = data.first;
	QMap< InstanceNode*, QPair< BBox, Transform* > >* sceneMap = data.second;
	std::vector< Photon* > photonsVector;

	Ray ray;
	double i = 0;
	while ( i < numberOfPhotons )
	{
		ray.origin = m_lightShape->Sample( m_pRand->RandomDouble(), m_pRand->RandomDouble() );
		m_lightSunShape->generateRayDirection( ray.direction, *m_pRand );
		ray.mint = tgc::Epsilon;
		ray.maxt = tgc::Infinity;
		ray = m_lightToWorld( ray );

		Ray* reflectedRay = 0;
		Photon* next = 0;
		Photon* first = new Photon( ray.origin );
		Photon* node = first;
		node->intersectedSurface = 0;
		int rayLength = 0;

		InstanceNode* intersectedSurface = 0;
		bool isFront = false;

		//Trace the ray
		bool intersection = true;
		while ( intersection )
		{
			intersectedSurface = 0;
			isFront = 0;

			QMap< InstanceNode*, QPair< BBox, Transform* > >::const_iterator i = sceneMap->constBegin();


			while (i != sceneMap->constEnd() )
			{

				InstanceNode* shapeKitNode = i.key();
				QPair< BBox, Transform* > nodeData = i.value();

				Ray* childReflected = 0;
				double previusMaxT = ray.maxt;
				bool isChildFront = false;

				childReflected = shapeKitNode->Intersect( ray, *m_pRand, nodeData, &isChildFront );


				if( ray.maxt < previusMaxT )
				{
					delete reflectedRay;
					reflectedRay = 0;
					intersectedSurface = shapeKitNode;
					isFront = isChildFront;
				}
				if( childReflected )	reflectedRay = new Ray( *childReflected );

				delete childReflected;
				childReflected = 0;
				++i;

			 }

			if( reflectedRay )
			{
					Point3D point = ray( ray.maxt );

					next = new Photon( point, node );
					next->intersectedSurface = intersectedSurface;
					next->surfaceSide = ( isFront ) ? 1.0 : 0.0;
					node->next = next;
					node = next;
					rayLength++;

					//Prepare node and ray for next iteration
					ray = *reflectedRay;
					delete reflectedRay;
					reflectedRay = 0;
			}
			else intersection = false;
		}

		if( !(rayLength == 0 && ray.maxt == HUGE_VAL) )
		{
				if( ray.maxt == HUGE_VAL  ) ray.maxt = 0.1;

				Point3D endOfRay = ray( ray.maxt );
				Photon* lastNode = new Photon( endOfRay, node );
				lastNode->intersectedSurface = intersectedSurface;
				lastNode->surfaceSide = ( isFront ) ? 1.0 : 0.0;
				node->next = lastNode;

		}

		photonsVector.push_back( first );
		i++;
	}

	return photonsVector;
}
