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


#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <mutex>
#include <vector>
#include <utility>

#include "RayTracingLibrary.h"

#include "TPhotonMap.h"
#include "Transform.h"
#include "TSceneNode.h"
#include "TSunNode.h"
#include "TTransmissivity.h"

class ParallelRandomDeviate;
class RandomDeviate;
class Ray;

struct LightOriginShape
{
	double cellHeight;
	double cellWidth;

	//double height;
	double zMinSunPlane;
	double zMaxSunPlane;

	//double width;
	double xMinSunPlane;
	double xMaxSunPlane;


	int heightElements;
	int widthElements;

	//first height (z dimension), second width (x dimension)
	std::vector< std::pair< int, int > >  validSunAreasVector;

	double GetValidArea() const
	{
		return ( validSunAreasVector.size() * cellHeight * cellWidth );
	}
};

//!  RayTracer is base class for  is a container to execute the ray tracing algorithm in a scene.
/*!
  RayTracer class provides an interface for different ray tracing algorithms in order to execute the ray tracer technique to objects scene.
*/

class RAYTRACING_API RayTracer
{
public:
	RayTracer();
	virtual ~RayTracer();

	bool Run( unsigned long numberOfRays );
	void SetPhotonMap( TPhotonMap* photonMap  = 0 );
	void SetRandomNumberGenerator( RandomDeviate* randomNumberGenerator = 0 );
	//virtual bool SetScene( std::shared_ptr< TSceneNode >&  scene, std::vector<std::string > notFirstStageNodesURL = std::vector<std::string >() );
	virtual bool SetScene( std::shared_ptr< TSceneNode >&  scene );

protected:
	bool NewRay( Ray* ray, ParallelRandomDeviate& rand );
	virtual void RunRaytracer( unsigned long numberOfRays ) = 0;
	virtual void RunRaytracerWithTransmissivity( unsigned long numberOfRays ) = 0;

protected:
	RandomDeviate* m_pRandomNumberGenerator;
	std::mutex m_randomMutex;

	TPhotonMap* m_pPhotonMap;
	std::shared_ptr< TSunNode > m_pSunNode;
	std::shared_ptr< TTransmissivity > m_pTransmissivityNode;
	LightOriginShape m_lightOriginShape;
	Transform m_sunTransformSTW;

	std::string m_lightNodeURL;
};


#endif /* RAYTRACER_H_ */
