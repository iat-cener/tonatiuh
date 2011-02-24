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
#include "RayTracerPhoton.h"
#include "TPhotonMap.h"
#include "TPhotonMapFactory.h"
#include "TShape.h"
#include "TSunShape.h"

RayTracer::RayTracer( InstanceNode* rootNode,
	       InstanceNode* lightNode,
	       TShape* lightShape,
	       TSunShape* const lightSunShape,
	       Transform lightToWorld,
	       RandomDeviate& rand,
	       QMutex* mutex,
	       TPhotonMap* photonMap )
:m_rootNode( rootNode ),
m_lightNode( lightNode ),
m_lightShape( lightShape ),
m_lightSunShape( lightSunShape ),
m_lightToWorld( lightToWorld ),
m_pRand( &rand ),
m_mutex( mutex ),
m_photonMap( photonMap )
{

}

Ray RayTracer::NewPrimitiveRay( ParallelRandomDeviate& rand )
{
	Point3D origin = m_lightShape->Sample( rand.RandomDouble(), rand.RandomDouble() );
	Vector3D direction;
	m_lightSunShape->GenerateRayDirection( direction, rand );
	return m_lightToWorld( Ray( origin, direction ) );
}

QPair< TPhotonMap*, std::vector< RayTracerPhoton > > RayTracer::operator()( double numberOfRays )
{
	std::vector< RayTracerPhoton > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray* ray = new Ray( NewPrimitiveRay( rand ) );

		photonsVector.push_back( RayTracerPhoton( ray->origin, 0, m_lightNode ) );
		int rayLength = 0;

		InstanceNode* intersectedSurface = 0;
		bool isFront = false;

		//Trace the ray
		Ray* reflectedRay = ray;
		while( reflectedRay !=0 )
		{
			reflectedRay = 0;
			intersectedSurface = 0;
			isFront = 0;

			reflectedRay = m_rootNode->Intersect( *ray, rand, &intersectedSurface );

			if( reflectedRay )
			{
				photonsVector.push_back( RayTracerPhoton( (*ray)( ray->maxt ), ++rayLength, intersectedSurface) );

				//Prepare node and ray for next iteration
				delete ray;
				ray = 0;
				ray = reflectedRay;
			}

		}

		if( !(rayLength == 0 && ray->maxt == HUGE_VAL) )
		{
			if( ray->maxt == HUGE_VAL  ) ray->maxt = 0.1;
			photonsVector.push_back( RayTracerPhoton( (*ray)( ray->maxt ), ++rayLength, intersectedSurface) );
		}

		delete ray;
		ray = 0;


	}
	photonsVector.resize( photonsVector.size() );

	return QPair< TPhotonMap*, std::vector< RayTracerPhoton > >( m_photonMap, photonsVector );
}
