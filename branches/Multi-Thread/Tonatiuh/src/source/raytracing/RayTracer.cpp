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
	       TPhotonMap* photonMap )
:m_rootNode( rootNode ),
m_lightShape( static_cast< TShape*> ( lightShape->copy() )),
m_lightSunShape( static_cast< TSunShape* > ( lightSunShape->copy() ) ),
m_lightToWorld( lightToWorld ),
m_pRand( &rand ),
m_photonMap( photonMap )
{

}

QPair< TPhotonMap*, std::vector< Photon* > > RayTracer::operator()( double numberOfRays )
{
	std::vector< Photon* > photonsVector;

	ParallelRandomDeviate* rand = new ParallelRandomDeviate( m_pRand );

	/*double nRay = 0;
	std::cout<<"numberOfRays: "<<numberOfRays<<"\n"<<std::endl;
	while ( nRay < numberOfRays )
	{
		//std::cout<<"nRay: "<<nRay<<" "<<rand->RandomDouble()<<"\n"<<std::endl;
		rand->RandomDouble();
		++nRay;
	}
	delete rand;*/

	Ray ray;
	double nRay = 0;
	while ( nRay < numberOfRays )
	{
		ray.origin = m_lightShape->Sample( rand->RandomDouble(), rand->RandomDouble() );
		m_lightSunShape->generateRayDirection( ray.direction, *rand );
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


			reflectedRay = m_rootNode->Intersect( ray, *rand, &intersectedSurface, &isFront );

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
		nRay++;
	}

	//delete rand;

	return QPair< TPhotonMap*, std::vector< Photon* > >( m_photonMap, photonsVector );
}
