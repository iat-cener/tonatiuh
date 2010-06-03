/*
 * RayTracer.cpp
 *
 *  Created on: 22/04/2010
 *      Author: amutuberria
 */

#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QVector>

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Photon.h"
#include "ParallelRandomDeviate.h"
#include "Ray.h"
#include "RayTracer.h"
#include "TPhotonMap.h"
#include "TPhotonMapFactory.h"
#include "TShape.h"
#include "TSunShape.h"
#include "Trace.h"

RayTracer::RayTracer( InstanceNode* rootNode,
	       TShape* lightShape,
	       TSunShape* const lightSunShape,
	       Transform lightToWorld,
	       RandomDeviate& rand,
	       QMutex* mutex,
	       TPhotonMap* photonMap )
:m_rootNode( rootNode ),
m_lightShape( lightShape ),
m_lightSunShape( lightSunShape ),
m_lightToWorld( lightToWorld ),
m_pRand( &rand ),
m_mutex( mutex ),
m_photonMap( photonMap )
{

}

QPair< TPhotonMap*, std::vector< Photon > > RayTracer::operator()( double numberOfRays )
{
	std::vector< Photon > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray ray;
		ray.origin = m_lightShape->Sample( rand.RandomDouble(), rand.RandomDouble() );
		m_lightSunShape->GenerateRayDirection( ray.direction, rand );
		ray.mint = tgc::Epsilon;
		ray.maxt = tgc::Infinity;
		ray = m_lightToWorld( ray );

	/*	Photon first( ray.origin );
		//Photon* node = &first;

		Photon* node = new Photon( first);
		node->intersectedSurface = 0;*/

		Photon first( ray.origin );
		first.id = 0;
		first.intersectedSurface = 0;
		photonsVector.push_back( first );
		int rayLength = 0;

		InstanceNode* intersectedSurface = 0;
		bool isFront = false;

		//Trace the ray
		bool intersection = true;
		while ( intersection )
		{
			intersectedSurface = 0;
			isFront = 0;
			Ray reflectedRay;

			double tHit = ray.maxt;
			bool isOutputRay = m_rootNode->Intersect( ray, rand, &tHit, &intersectedSurface, &isFront, &reflectedRay );

			ray.maxt = tHit;

			if( isOutputRay )
			{
				Photon next( ray( ray.maxt ) );
				next.id = ++rayLength;
				next.intersectedSurface = intersectedSurface;
				next.surfaceSide = ( isFront ) ? 1.0 : 0.0;
				photonsVector.push_back( next );


				//Prepare node and ray for next iteration
				ray = reflectedRay;
			}
			else intersection = false;
		}

		if( !(rayLength == 0 && ray.maxt == HUGE_VAL) )
		{
				if( ray.maxt == HUGE_VAL  ) ray.maxt = 0.1;

				Point3D endOfRay = ray( ray.maxt );
				Photon lastNode( endOfRay );
				lastNode.id = ++rayLength;
				lastNode.intersectedSurface = intersectedSurface;
				lastNode.surfaceSide = ( isFront ) ? 1.0 : 0.0;
				photonsVector.push_back( lastNode );
		}


	}

	return QPair< TPhotonMap*, std::vector< Photon > >( m_photonMap, photonsVector );
}
