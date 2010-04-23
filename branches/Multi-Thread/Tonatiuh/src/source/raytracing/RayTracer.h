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
class RandomDeviateFactory;
class TShape;
class TSunShape;

class RayTracer
{

public:
	RayTracer( InstanceNode* rootNode,
		       TShape* lightShape,
		       TSunShape* const lightSunShape,
		       Transform lightToWorld,
		       RandomDeviateFactory& randFactory);

	typedef std::vector< Photon* > result_type;

	std::vector< Photon* > operator()( QPair< double, QMap< InstanceNode*,QPair< BBox, Transform* > >*  > data );



private:
	InstanceNode* m_rootNode;
	TShape* m_lightShape;
	const TSunShape* m_lightSunShape;
	Transform m_lightToWorld;
	RandomDeviate* m_pRand;
};


#endif /* RAYTRACER_H_ */
