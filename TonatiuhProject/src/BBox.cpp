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

#include <cmath>

#include "BBox.h"
#include "Ray.h"
#include "tgc.h"
#include "Vector3D.h"
#include "Point3D.h"



BBox::BBox()
: pMin( tgc::Infinity, tgc::Infinity, tgc::Infinity ), pMax( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity )
{
}

BBox::BBox( const Point3D& point )
: pMin(point), pMax(point)
{
}

BBox::BBox( const Point3D& point1, const Point3D& point2 )
{

	pMin = Point3D( std::min( point1.x, point2.x ),
				    std::min( point1.y, point2.y ),
				    std::min( point1.z, point2.z ) );
	pMax = Point3D( std::max( point1.x, point2.x ),
				    std::max( point1.y, point2.y ),
				    std::max( point1.z, point2.z) );
}

bool BBox::Overlaps( const BBox& bbox ) const
{
	bool xOverlap = ( pMax.x >= bbox.pMin.x ) && ( pMin.x <= bbox.pMax.x );
	bool yOverlap = ( pMax.y >= bbox.pMin.y ) && ( pMin.y <= bbox.pMax.y);
	bool zOverlap = ( pMax.z >= bbox.pMin.z ) && ( pMin.z <= bbox.pMax.z);
	return ( xOverlap && yOverlap && zOverlap );
}

bool BBox::Inside( const Point3D& point) const
{
	return ( point.x >= pMin.x && point.x <= pMax.x &&
             point.y >= pMin.y && point.y <= pMax.y &&
             point.z >= pMin.z && point.z <= pMax.z );
}

void BBox::Expand( double delta )
{
	pMin -= Vector3D( delta, delta, delta );
	pMax += Vector3D( delta, delta, delta );
}

double BBox::Volume() const
{
	Vector3D diagonal = pMax - pMin;
	return diagonal.x * diagonal.y * diagonal.z;
}

int BBox::MaximumExtent() const
{
	Vector3D diagonal = pMax - pMin;
	if( diagonal.x > diagonal.y && diagonal.x > diagonal.z) return 0;
	else if( diagonal.y > diagonal.z) return 1;
	else return 2;
}

void BBox::BoundingSphere( Point3D* center, float* radius ) const
{
	*center = Point3D( (pMin.x+pMax.x)*0.5, (pMin.y+pMax.y)*0.5, (pMin.z+pMax.z)*0.5 );
    *radius = Distance( *center, pMax );
}

bool BBox::IntersectP( const Ray& ray, double* hitt0, double* hitt1 ) const
{
    double t0 = ray.mint;
    double t1 = ray.maxt;

    for( int i = 0; i < 3; ++i )
    {
    	double invRayDir = 1.0 / ray.direction[i];
    	double tNear = ( pMin[i] - ray.origin[i] ) * invRayDir;
    	double tFar = ( pMax[i] - ray.origin[i] ) * invRayDir;
    	if( tNear > tFar ) std::swap( tNear, tFar );
    	t0 = tNear > t0 ? tNear : t0;
    	t1 = tFar < t1 ? tFar : t1;
    	if( t0 > t1 ) return false;
    }
    if( hitt0 ) *hitt0 = t0;
    if( hitt1 ) *hitt1 = t1;
    return true;
}

std::ostream& operator<<( std::ostream& os, const BBox& bbox )
{
    os << "pMin: " << bbox.pMin << std::endl;
    os << "pMax: " << bbox.pMax << std::endl;
    return os;
}

BBox Union( const BBox& bbox, const Point3D& point )
{
	BBox result;
	result.pMin.x = std::min( bbox.pMin.x, point.x );
	result.pMin.y = std::min( bbox.pMin.y, point.y );
	result.pMin.z = std::min( bbox.pMin.z, point.z );
	result.pMax.x = std::max( bbox.pMax.x, point.x );
	result.pMax.y = std::max( bbox.pMax.y, point.y );
	result.pMax.z = std::max( bbox.pMax.z, point.z );
	return result;
}

BBox Union( const BBox& bbox1, const BBox& bbox2 )
{
	BBox result;
	result.pMin.x = std::min( bbox1.pMin.x, bbox2.pMin.x);
	result.pMin.y = std::min( bbox1.pMin.y, bbox2.pMin.y );
	result.pMin.z = std::min( bbox1.pMin.z, bbox2.pMin.z );
	result.pMax.x = std::max( bbox1.pMax.x, bbox2.pMax.x );
	result.pMax.y = std::max( bbox1.pMax.y, bbox2.pMax.y);
	result.pMax.z = std::max( bbox1.pMax.z, bbox2.pMax.z );
	return result;
}
