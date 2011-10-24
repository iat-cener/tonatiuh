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

#include "DifferentialGeometry.h"
#include "ParallelRandomDeviate.h"
#include "Ray.h"
#include "RayTracer.h"
#include "RayTracerPhoton.h"
#include "TPhotonMap.h"
#include "TLightShape.h"
#include "TSunShape.h"

RayTracer::RayTracer( InstanceNode* rootNode,
	       InstanceNode* lightNode,
	       TLightShape* lightShape,
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


bool RayTracer::NewPrimitiveRay( Ray* ray, ParallelRandomDeviate& rand, int a,int b)
{
    //generacion del rayo
	Point3D origin = m_lightShape->Sample( rand.RandomDouble(), rand.RandomDouble(),a,b );
	//printf("launch points\n");
	//printf("created:%f,%f \n",origin.x,origin.z);

	//if( !m_lightShape->IsIntoValidArea( origin ) )	return false;
	//printf("points\n");
	Vector3D direction;
	m_lightSunShape->GenerateRayDirection( direction, rand );
	*ray =  m_lightToWorld( Ray( origin, direction ) );

	return true;
}


QPair< TPhotonMap*, std::vector< RayTracerPhoton > > RayTracer::operator()( double numberOfRays )
{
	std::vector< RayTracerPhoton > photonsVector;
	ParallelRandomDeviate rand( m_pRand, m_mutex );
	//calculo el numero de celdas de las que se pueden lanzar rayos
	int numberOfAvailableCells=0;
	for( int i = 0; i < m_lightShape->m_heightElements; i++ ){
		for( int j = 0; j < m_lightShape->m_widthElements; j++ ){
			if(m_lightShape->m_lightAreaMatrix[i][j]==1){
				numberOfAvailableCells++;
			}
		}
	}
	//numero de rayos por celda
    double numberOfRaysperCell=numberOfRays/numberOfAvailableCells;
    double resto=numberOfRays-(numberOfAvailableCells*int(numberOfRaysperCell));
    int i;
    if(resto!=0){
    printf("Number of available cells:%d-->rays %d-->rays per cell:: %f;;resto %d",numberOfAvailableCells,int(numberOfRays),numberOfRaysperCell,int(resto));
    }
    for(int a=0; a<m_lightShape->m_heightElements;a++){
		for(int b=0; b<m_lightShape->m_widthElements; b++){
			if(m_lightShape->m_lightAreaMatrix[a][b]==1){
				if(resto>0){
					i=0;
					resto--;
				}
				else{
					i=1;
				}
				while(i<=numberOfRaysperCell){
				std::vector<Ray> currentRaysWay;
				Ray ray;
				if( NewPrimitiveRay( &ray, rand,a, b ) )
				{
					photonsVector.push_back( RayTracerPhoton( ray.origin, 1, 0, m_lightNode ) );
					int rayLength = 0;

					InstanceNode* intersectedSurface = 0;
					bool isFront = false;
					bool isDirectSun = true;

					//Trace the ray
					bool isReflectedRay = true;
					while( isReflectedRay )
					{
						intersectedSurface = 0;
						isFront = 0;
						Ray reflectedRay;
						isReflectedRay = m_rootNode->Intersect( ray, rand, &isFront, &intersectedSurface, &reflectedRay );

						if (!isDirectSun) currentRaysWay.push_back(ray);
						if( isReflectedRay )
						{
							photonsVector.push_back( RayTracerPhoton( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface) );

							//Prepare node and ray for next iteration
							/*delete ray;
							ray = 0;*/
							ray = reflectedRay;
							isDirectSun = false;
						}

					}

					if( !(rayLength == 0 && ray.maxt == HUGE_VAL) )
					{
						if( ray.maxt == HUGE_VAL  )
						{
							ray.maxt = 0.1;
							photonsVector.push_back( RayTracerPhoton( (ray)( ray.maxt ), 0, ++rayLength, intersectedSurface) );
						}
						else
							photonsVector.push_back( RayTracerPhoton( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface) );
					}
					if (currentRaysWay.size()>0)
					{
						currentRaysWay.resize(currentRaysWay.size());
						m_rootNode->Analyze(&currentRaysWay,m_mutex);
						currentRaysWay.clear();
					}

				}
            i++;
			}
		  }
		 i=0;
		}
	}
	photonsVector.resize( photonsVector.size() );

	return QPair< TPhotonMap*, std::vector< RayTracerPhoton > >( m_photonMap, photonsVector );
}

