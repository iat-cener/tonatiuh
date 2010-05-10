/*
 * RayTracer.h
 *
 *  Created on: 22/04/2010
 *      Author: amutuberria
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <vector>

#include <QMap>
#include <QPair>

#include "Transform.h"

class InstanceNode;
class Photon;
class ParallelRandomDeviate;
class TPhotonMap;
class TShape;
class TSunShape;

class RayTracer
{

public:
	RayTracer( InstanceNode* rootNode,
		       TShape* lightShape,
		       TSunShape* const lightSunShape,
		       Transform lightToWorld,
		       RandomDeviate& rand,
		       TPhotonMap* photonMap );

	typedef QPair< TPhotonMap*, std::vector< Photon* > > result_type;

	QPair< TPhotonMap*, std::vector< Photon* > > operator()( double numberOfRays );



private:
	InstanceNode* m_rootNode;
	TShape* m_lightShape;
	const TSunShape* m_lightSunShape;
	Transform m_lightToWorld;
	RandomDeviate* m_pRand;
	TPhotonMap* m_photonMap;
};


#endif /* RAYTRACER_H_ */
