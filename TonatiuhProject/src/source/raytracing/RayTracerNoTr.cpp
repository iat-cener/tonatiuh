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

Contributors: Javier Garcia-Barberena, Iï¿½aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QPoint>

#include "DifferentialGeometry.h"
#include "ParallelRandomDeviate.h"
#include "Ray.h"
#include "RayTracerNoTr.h"
#include "TPhotonMap.h"
#include "TLightShape.h"
#include "TSunShape.h"
RayTracerNoTr::RayTracerNoTr( InstanceNode* rootNode,
	       InstanceNode* lightNode,
	       TLightShape* lightShape,
	       TSunShape* const lightSunShape,
	       Transform lightToWorld,
	       RandomDeviate& rand,
	       QMutex* mutex,
	       TPhotonMap* photonMap,
	       QMutex* mutexPhotonMap,
	       QVector< InstanceNode* > exportSuraceList )
:m_exportSuraceList( exportSuraceList ),
m_rootNode( rootNode ),
m_lightNode( lightNode ),
m_lightShape( lightShape ),
m_lightSunShape( lightSunShape ),
m_lightToWorld( lightToWorld ),
m_pRand( &rand ),
m_mutex( mutex ),
m_photonMap( photonMap ),
m_pPhotonMapMutex( mutexPhotonMap )
{
	m_validAreasVector = m_lightShape->GetValidAreasCoord();
}

//generating the ray
bool RayTracerNoTr::NewPrimitiveRay( Ray* ray, ParallelRandomDeviate& rand )
{
	if( m_validAreasVector.size() < 1 )	return false;
	int area = int ( rand.RandomDouble() * m_validAreasVector.size() );

	QPair< int, int > areaIndex = m_validAreasVector[area] ;

	//generating the photon
	Point3D origin = m_lightShape->Sample( rand.RandomDouble(), rand.RandomDouble(), areaIndex.first, areaIndex.second );
	//generating the ray direction
	Vector3D direction;
	m_lightSunShape->GenerateRayDirection( direction, rand );
	//generatin the ray
	*ray =  m_lightToWorld( Ray( origin, direction ) );

	return true;
}

/*!
 * Traces \a numberOfRays rays.
 */
void RayTracerNoTr::operator()( double numberOfRays )
{
	if( m_exportSuraceList.size() < 1 )
		RayTracerCreatingAllPhotons( numberOfRays );
	else if( m_exportSuraceList.size() > 0 &&  m_exportSuraceList.contains( m_lightNode ) )
		RayTracerCreatingLightPhotons( numberOfRays );
	else
		RayTracerNotCreatingLightPhotons( numberOfRays );
}

/*!
 * Traces \a numberOfRays rays and creates photons for all intersections.
 */
void RayTracerNoTr::RayTracerCreatingAllPhotons(  double numberOfRays  )
{
	std::vector< Photon > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray ray;
		if( NewPrimitiveRay( &ray, rand ) )
		{
			photonsVector.push_back( Photon( ray.origin, 1, 0, m_lightNode ) );
			int rayLength = 0;

			InstanceNode* intersectedSurface = 0;
			bool isFront = false;

			//Trace the ray
			bool isReflectedRay = true;
			while( isReflectedRay )
			{
				intersectedSurface = 0;
				isFront = 0;
				Ray reflectedRay;
				isReflectedRay = m_rootNode->Intersect( ray, rand, &isFront, &intersectedSurface, &reflectedRay );

				if( isReflectedRay )
				{
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1) );

					//Prepare node and ray for next iteration
					ray = reflectedRay;
				}

			}

			if( !(rayLength == 0 && ray.maxt == HUGE_VAL) )
			{
				if( ray.maxt == HUGE_VAL  )
				{
					ray.maxt = 0.1;
					photonsVector.push_back( Photon( (ray)( ray.maxt ), 0, ++rayLength, intersectedSurface) );
				}
				else
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1 ) );
			}

		}

	}

	photonsVector.resize( photonsVector.size() );

	m_pPhotonMapMutex->lock();
	m_photonMap->StoreRays( photonsVector );
	m_pPhotonMapMutex->unlock();


}

/*!
 * Traces \a numberOfRays rays. Creates photons for the ray origin and to the selected surfaces
 */
void RayTracerNoTr::RayTracerCreatingLightPhotons(  double numberOfRays  )
{
	std::vector< Photon > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray ray;
		if( NewPrimitiveRay( &ray, rand ) )
		{
			photonsVector.push_back( Photon( ray.origin, 1, 0, m_lightNode ) );
			int rayLength = 0;

			InstanceNode* intersectedSurface = 0;
			bool isFront = false;

			//Trace the ray
			bool isReflectedRay = true;
			while( isReflectedRay )
			{
				intersectedSurface = 0;
				isFront = 0;
				Ray reflectedRay;
				isReflectedRay = m_rootNode->Intersect( ray, rand, &isFront, &intersectedSurface, &reflectedRay );

				if( isReflectedRay )
				{
					if( m_exportSuraceList.contains( intersectedSurface ) )
						photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1 ) );

					//Prepare node and ray for next iteration
					ray = reflectedRay;
				}

			}

			if( m_exportSuraceList.contains( intersectedSurface ) && !(rayLength == 0 && ray.maxt == HUGE_VAL) )
			{
				if( ray.maxt == HUGE_VAL  )
				{
					ray.maxt = 0.1;
					photonsVector.push_back( Photon( (ray)( ray.maxt ), 0, ++rayLength, intersectedSurface) );
				}
				else
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface) );
			}

		}

	}
	photonsVector.resize( photonsVector.size() );

	m_pPhotonMapMutex->lock();
	m_photonMap->StoreRays( photonsVector );
	m_pPhotonMapMutex->unlock();

}

/*!
 * Traces \a numberOfRays rays. Creates photons for the selected surfaces.
 * Photons for the rays origin will not be created.
 */
void RayTracerNoTr::RayTracerNotCreatingLightPhotons(  double numberOfRays  )
{
	std::vector< Photon > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray ray;
		if( NewPrimitiveRay( &ray, rand ) )
		{
			int rayLength = 0;

			InstanceNode* intersectedSurface = 0;
			bool isFront = false;

			//Trace the ray
			bool isReflectedRay = true;
			while( isReflectedRay )
			{
				intersectedSurface = 0;
				isFront = 0;
				Ray reflectedRay;
				isReflectedRay = m_rootNode->Intersect( ray, rand, &isFront, &intersectedSurface, &reflectedRay );

				if( isReflectedRay )
				{
					if( m_exportSuraceList.contains( intersectedSurface ) )
						photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1) );

					//Prepare node and ray for next iteration
					ray = reflectedRay;
				}

			}

			if( m_exportSuraceList.contains( intersectedSurface ) && !(rayLength == 0 && ray.maxt == HUGE_VAL) )
			{
				if( ray.maxt == HUGE_VAL  )
				{
					ray.maxt = 0.1;
					photonsVector.push_back( Photon( (ray)( ray.maxt ), 0, ++rayLength, intersectedSurface) );
				}
				else
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface) );
			}

		}

	}
	photonsVector.resize( photonsVector.size() );

	m_pPhotonMapMutex->lock();
	m_photonMap->StoreRays( photonsVector );
	m_pPhotonMapMutex->unlock();

}
