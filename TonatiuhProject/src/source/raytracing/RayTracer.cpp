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
#include "TPhotonMap.h"
#include "TPhotonMapFactory.h"
#include "TShape.h"
#include "TSunShape.h"
#include "Trace.h"

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

QPair< TPhotonMap*, std::vector< Photon > > RayTracer::operator()( double numberOfRays )
{
	std::vector< Photon > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );

	Ray ray;
	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		ray.origin = m_lightShape->Sample( rand.RandomDouble(), rand.RandomDouble() );
		Vector3D rayDirection;
		m_lightSunShape->GenerateRayDirection( rayDirection, rand );
		ray.setDirection( rayDirection );
		ray.mint = tgc::Epsilon;
		ray.maxt = tgc::Infinity;
		ray = m_lightToWorld( ray );

		Photon first( ray.origin );
		first.id = 0;
		first.intersectedSurface = m_lightNode;
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
