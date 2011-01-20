/*
 * RayTracerPhoton.h
 *
 *  Created on: 19/01/2011
 *      Author: amutuberria
 */

#ifndef RAYTRACERPHOTON_H_
#define RAYTRACERPHOTON_H_

#include "InstanceNode.h"
#include "Point3D.h"

struct RayTracerPhoton
{
	RayTracerPhoton( );
	RayTracerPhoton( const RayTracerPhoton& photon );
	RayTracerPhoton( Point3D pos, double id = 0, InstanceNode* intersectedSurface = 0 );
	~RayTracerPhoton();

	double id;
	Point3D pos;
	InstanceNode* intersectedSurface;
};

#endif /* RAYTRACERPHOTON_H_ */
