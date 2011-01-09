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

#include <cmath>

#include "BBox.h"
#include "Ray.h"
#include "tgc.h"
#include "Vector3D.h"
#include "Point3D.h"



BBox::BBox()
: pMin( tgc::Infinity, tgc::Infinity, tgc::Infinity ),
  pMax( -tgc::Infinity, -tgc::Infinity, -tgc::Infinity )
{
}

BBox::BBox( const Point3D& point )
: pMin(point), pMax(point)
{
}

BBox::BBox( const Point3D& point1, const Point3D& point2 )
{
   pMin.x = point1.x < point2.x ? point1.x : point2.x;
   pMin.y = point1.y < point2.y ? point1.y : point2.y;
   pMin.z = point1.z < point2.z ? point1.z : point2.z;

   pMax.x = point1.x > point2.x ? point1.x : point2.x;
   pMax.y = point1.y > point2.y ? point1.y : point2.y;
   pMax.z = point1.z > point2.z ? point1.z : point2.z;

}

bool BBox::Overlaps( const BBox& bbox ) const
{
   return ( pMax.x >= bbox.pMin.x ) && ( pMin.x <= bbox.pMax.x ) &&
          ( pMax.y >= bbox.pMin.y ) && ( pMin.y <= bbox.pMax.y ) &&
          ( pMax.z >= bbox.pMin.z ) && ( pMin.z <= bbox.pMax.z);
}

bool BBox::Inside( const Point3D& point) const
{
   return ( point.x >= pMin.x && point.x <= pMax.x &&
            point.y >= pMin.y && point.y <= pMax.y &&
            point.z >= pMin.z && point.z <= pMax.z );
}

void BBox::Expand( double delta )
{
   pMin.x -= delta;
   pMin.y -= delta;
   pMin.z -= delta;

   pMax.x += delta;
   pMax.y += delta;
   pMax.z += delta;
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
   return 2;
}

void BBox::BoundingSphere( Point3D& center, double& radius ) const
{
   center = Point3D( (pMin.x+pMax.x)*0.5, (pMin.y+pMax.y)*0.5, (pMin.z+pMax.z)*0.5 );
   radius = Distance( center, pMax );
}

bool BBox::IntersectP( const Ray& ray, double* hitt0, double* hitt1 ) const
{
    double t0 = ray.mint;
    double t1 = ray.maxt;
    double tmin, tmax, tymin, tymax, tzmin, tzmax;

    double invDirection = ray.invDirection().x;
	if( invDirection >= 0.0 )
	{
       tmin = ( pMin.x - ray.origin.x ) * invDirection;
	   tmax = ( pMax.x - ray.origin.x ) * invDirection;
	}
	else
	{
	   tmin = ( pMax.x - ray.origin.x ) * invDirection;
	   tmax = ( pMin.x - ray.origin.x ) * invDirection;
	}

	invDirection = ray.invDirection().y;
	if( invDirection >= 0.0 )
	{
	   tymin = ( pMin.y - ray.origin.y ) * invDirection;
	   tymax = ( pMax.y - ray.origin.y ) * invDirection;
	}
	else
	{
	   tymin = ( pMax.y - ray.origin.y ) * invDirection;
	   tymax = ( pMin.y - ray.origin.y ) * invDirection;
	}

	if ( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

	if ( tymin > tmin ) tmin = tymin;
	if ( tymax < tmax ) tmax = tymax;

	invDirection = ray.invDirection().z;
	if( invDirection >= 0.0 )
	{
	   tzmin = ( pMin.z - ray.origin.z ) * invDirection;
	   tzmax = ( pMax.z - ray.origin.z ) * invDirection;
	}
	else
	{
	   tzmin = ( pMax.z - ray.origin.z ) * invDirection;
	   tzmax = ( pMin.z - ray.origin.z ) * invDirection;
	}

	if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

	if ( tzmin > tmin ) tmin = tzmin;
	if ( tzmax < tmax ) tmax = tzmax;
	if ( ( tmin < t1 ) && ( tmax > t0 ) )
	{
		if( tmin < t0 ) tmin = t0;
		if( tmax > t1 ) tmax = t1;
		if( hitt0 ) *hitt0 = tmin;
		if( hitt1 ) *hitt1 = tmax;
		return true;
	}
	else return false;
}

BBox Union( const BBox& bbox, const Point3D& point )
{
   BBox unionBox;
   unionBox.pMin.x = ( bbox.pMin.x < point.x ) ? bbox.pMin.x : point.x;
   unionBox.pMin.y = ( bbox.pMin.y < point.y ) ? bbox.pMin.y : point.y;
   unionBox.pMin.z = ( bbox.pMin.z < point.z ) ? bbox.pMin.z : point.z;
   unionBox.pMax.x = ( bbox.pMax.x > point.x ) ? bbox.pMax.x : point.x;
   unionBox.pMax.y = ( bbox.pMax.y > point.y ) ? bbox.pMax.y : point.y;
   unionBox.pMax.z = ( bbox.pMax.z > point.z ) ? bbox.pMax.z : point.z;
   return unionBox;
}

BBox Union( const BBox& bbox1, const BBox& bbox2 )
{
   BBox unionBox;
   unionBox.pMin.x = ( bbox1.pMin.x < bbox2.pMin.x ) ? bbox1.pMin.x : bbox2.pMin.x;
   unionBox.pMin.y = ( bbox1.pMin.y < bbox2.pMin.y ) ? bbox1.pMin.y : bbox2.pMin.y;
   unionBox.pMin.z = ( bbox1.pMin.z < bbox2.pMin.z ) ? bbox1.pMin.z : bbox2.pMin.z;
   unionBox.pMax.x = ( bbox1.pMax.x > bbox2.pMax.x ) ? bbox1.pMax.x : bbox2.pMax.x;
   unionBox.pMax.y = ( bbox1.pMax.y > bbox2.pMax.y ) ? bbox1.pMax.y : bbox2.pMax.y;
   unionBox.pMax.z = ( bbox1.pMax.z > bbox2.pMax.z ) ? bbox1.pMax.z : bbox2.pMax.z;
   return unionBox;
}

std::ostream& operator<<( std::ostream& os, const BBox& bbox )
{
    os << "pMin: " << bbox.pMin << std::endl;
    os << "pMax: " << bbox.pMax << std::endl;
    return os;
}
