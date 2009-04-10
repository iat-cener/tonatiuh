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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>
#include <iostream>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "InstanceNode.h"
#include "Matrix4x4.h"
#include "Photon.h"
#include "TPhotonMap.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Trace.h"
#include "Transform.h"
#include "TSeparatorKit.h"

void tgf::SevereError( std::string errorMessage )
{
	std::cerr << errorMessage << std::endl;
	exit(-1);
}

void tgf::Warning( std::string warningMessage )
{
	std::cerr << warningMessage << std::endl;
}

bool tgf::IsOdd( int number )
{
	bool answer = number & 1;
	return answer;
}

bool tgf::Quadratic( double A, double B, double C, double *t0, double *t1)
{
	// Find discriminant
	double discrim = B*B - 4.0*A*C;
	if (discrim < 0.) return false;

	// Compute quadratic root values
	double q = -0.5;
	if ( B < 0 ) q *= B - sqrt( discrim );
	else q *= B + sqrt( discrim ) ;
	*t0 = q / A;
	*t1 = C / q;
	if(*t0 > *t1) std::swap( *t0, *t1 );
	return true;
}

double tgf::AlternateBoxMuller( RandomDeviate& rand )
{
	Trace trace( "tgf::AlternateBoxMuller", false );

	static bool firsttime = true;
	static double x1;
	static double x2;

	if (firsttime)
	{
		double s = 2;
		double u1;
		double u2;
		while(s > 1)
		{
	 		u1 = 2 * rand.RandomDouble( ) - 1;
	 		u2 = 2 * rand.RandomDouble( ) - 1;
			s = u1 * u1 + u2 * u2;
		}

		double z = sqrt( -2 * log( s ) / s );
		x1 = z * u1;
		x2 = z * u2;

	 	firsttime = false;
	 	return x1;
	}
	else
	{
		firsttime = true;
		return x2;
	}


}

bool tgf::TraceRay( Ray& ray, InstanceNode* instanceNode, TPhotonMap& photonMap, RandomDeviate& rand)
{
	Trace trace( "tgf::TraceRay", false );

	Ray* reflectedRay = 0;
	Photon* node = new Photon( ray.origin, 0, 0 );
	Photon* next = 0;
	TSeparatorKit* coinNode = dynamic_cast< TSeparatorKit* > ( instanceNode->GetNode() );
	int rayLength = 0;

	//Trace the ray
	bool intersection = true;
	while ( intersection )
	{
		reflectedRay = coinNode->Intersect( ray, rand );

		if( reflectedRay )
		{
			Point3D point = ray( ray.maxt );

			next = new Photon( point, node, 0 );
			node->m_next = next;
			photonMap.store( node );
			node = next;
			rayLength++;

			//Prepare node and ray for next iteration
			ray = *reflectedRay;
			delete reflectedRay;
		}
		else intersection = false;
	}


	/*if( ray.maxt == HUGE_VAL  ) ray.maxt = 0.1;

	Point3D endOfRay = ray( ray.maxt );
	Photon* lastNode = new Photon( endOfRay, node, 0 );
	node->m_next = lastNode;
	photonMap.store( node );
	photonMap.store( lastNode );*/

	if( rayLength == 0 && ray.maxt == HUGE_VAL ) return false;
	if( ray.maxt == HUGE_VAL  ) ray.maxt = 0.1;

	Point3D endOfRay = ray( ray.maxt );
	Photon* lastNode = new Photon( endOfRay, node, 0 );
	node->m_next = lastNode;
	photonMap.store( node );
	photonMap.store( lastNode );
	return true;
}

SoSeparator* tgf::DrawPhotonMapPoints( const TPhotonMap& map )
{
	Trace trace( "tgf::DrawPhotonMapPoints", false );

	SoSeparator* drawpoints=new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	for( unsigned long i = 1 ; i < map.StoredPhotons().getValue(); i++ )
	{
		Point3D photon = map.GetPhoton(i)->m_pos;
		points->point.set1Value( i, photon.x, photon.y, photon.z );
	}

	SoMaterial* myMaterial = new SoMaterial;
  	myMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);
  	drawpoints->addChild(myMaterial);
	drawpoints->addChild(points);

  	SoDrawStyle* drawstyle = new SoDrawStyle;
  	drawstyle->pointSize = 3;
  	drawpoints->addChild(drawstyle);

	SoPointSet* pointset = new SoPointSet;
  	drawpoints->addChild(pointset);

  	return drawpoints;
}

SoSeparator* tgf::DrawPhotonMapRays( const TPhotonMap& map, unsigned long numberOfRays, double fraction )
{
	Trace trace( "tgf::DrawPhotonMapRays", false );

	SoSeparator* drawrays = new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	int drawRays = (int) ( numberOfRays * ( fraction / 100 ) );
	if( drawRays == 0 ) drawRays = 1;
	unsigned long ray = 0;
	int* lines = new int[drawRays];

	unsigned long rayLength = 0;
	unsigned long drawnRay = 0;

	unsigned long numberOfPhoton = 0;

	for( unsigned long i = 1; i<= map.StoredPhotons().getValue(); i++ )
	//for( unsigned long i = 0; i < map.StoredPhotons(); i++ )
	{

		if ( !map.GetPhoton(i)->m_prev )
		{
			if ( ray % ( numberOfRays/drawRays ) == 0 )
			{
				Photon* node = map.GetPhoton(i);
				rayLength = 0;

				while ( node )
				{
				    Point3D photon = node->m_pos;
					points->point.set1Value( numberOfPhoton, photon.x, photon.y, photon.z );
					node = node->m_next;
					rayLength++;
					numberOfPhoton++;
				}

				lines[drawnRay]= rayLength;
				drawnRay++;

			}
			ray++;
		}
	}

	SoMaterial* myMaterial = new SoMaterial;
  	myMaterial->diffuseColor.setValue(1.0, 1.0, 0.8);
  	drawrays->addChild( myMaterial );
    drawrays->addChild( points );

	SoLineSet* lineset = new SoLineSet;
  	lineset->numVertices.setValues( 0, drawnRay, lines );
  	drawrays->addChild( lineset );

  	delete lines;
  	return drawrays;
}

SbMatrix tgf::MatrixFromTransform( const Transform& transform )
{
	Trace trace( "tgf::AlternateBoxMuller", false );

	Ptr<Matrix4x4> transformMatrix = transform.GetMatrix()->Transpose();
	return SbMatrix( transformMatrix->m[0][0], transformMatrix->m[0][1], transformMatrix->m[0][2], transformMatrix->m[0][3],
											transformMatrix->m[1][0], transformMatrix->m[1][1], transformMatrix->m[1][2], transformMatrix->m[1][3],
											transformMatrix->m[2][0], transformMatrix->m[2][1], transformMatrix->m[2][2], transformMatrix->m[2][3],
											transformMatrix->m[3][0], transformMatrix->m[3][1], transformMatrix->m[3][2], transformMatrix->m[3][3] );

}
