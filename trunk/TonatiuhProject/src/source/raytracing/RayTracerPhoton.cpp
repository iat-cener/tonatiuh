/*
 * RayTracerPhoton.cpp
 *
 *  Created on: 19/01/2011
 *      Author: amutuberria
 */

#include "RayTracerPhoton.h"

RayTracerPhoton::RayTracerPhoton( )
{

}

RayTracerPhoton::RayTracerPhoton( const RayTracerPhoton& photon )
:id( photon.id ), pos( photon.pos ), shapeFront( photon.shapeFront ), intersectedSurface( photon.intersectedSurface )
{

}

RayTracerPhoton::RayTracerPhoton( Point3D pos, int shapeFront, double id, InstanceNode* intersectedSurface )
:id( id ), pos( pos ), shapeFront( shapeFront ), intersectedSurface( intersectedSurface )
{

}

RayTracerPhoton::~RayTracerPhoton()
{

}
