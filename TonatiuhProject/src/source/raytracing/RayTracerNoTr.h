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
***************************************************************************/


#ifndef RAYTRACERNOTR_H_
#define RAYTRACERNOTR_H_

#include <vector>

#include <QMap>
#include <QPair>
#include <QObject>
#include <QVector>

#include "Transform.h"


class InstanceNode;
class ParallelRandomDeviate;
struct Photon;
class RandomDeviate;
struct RayTracerPhoton;
class QMutex;
class QPoint;
class TPhotonMap;
class TLightShape;
class TSunShape;

class RayTracerNoTr
{

public:
	RayTracerNoTr( InstanceNode* rootNode,
		       InstanceNode* lightNode,
		       TLightShape* lightShape,
		       TSunShape* const lightSunShape,
		       Transform lightToWorld,
		       RandomDeviate& rand,
		       QMutex* mutex,
		       TPhotonMap* photonMap,
		       QMutex* mutexPhotonMap,
		       QVector< InstanceNode* > exportSuraceList );

	typedef void result_type;
	void operator()( double numberOfRays );


private:
	void RayTracerCreatingAllPhotons(  double numberOfRays  );
	void RayTracerCreatingLightPhotons(  double numberOfRays  );
	void RayTracerNotCreatingLightPhotons(  double numberOfRays  );

    QVector< InstanceNode* > m_exportSuraceList;
	InstanceNode* m_rootNode;
	InstanceNode* m_lightNode;
	TLightShape* m_lightShape;
	const TSunShape* m_lightSunShape;
	Transform m_lightToWorld;
	RandomDeviate* m_pRand;
    QMutex* m_mutex;
	TPhotonMap* m_photonMap;
    QMutex* m_pPhotonMapMutex;
	std::vector< QPair< int, int > >  m_validAreasVector;

	bool NewPrimitiveRay( Ray* ray, ParallelRandomDeviate& rand );
};


#endif /* RAYTRACER_H_ */
