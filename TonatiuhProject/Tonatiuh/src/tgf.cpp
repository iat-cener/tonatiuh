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

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cmath>

#include "InstanceNode.h"
#include "tgf.h"
#include "tgc.h"
#include "Vector3D.h"
#include "Ray.h"
#include "DifferentialGeometry.h"
#include "TMaterial.h"
#include "Trace.h"
#include "TSeparatorKit.h"





#if (defined(__linux__) && defined(__i386__)) || defined(WIN32)
#define FAST_INT 1
#endif
#define _doublemagicroundeps	      (.5-1.4e-11)

double tgf::RandomDouble()
{
	MersenneTwister* rand = 0;
	static bool firstRandomDouble = true;

	if ( firstRandomDouble )
	{
		rand = new MersenneTwister();
		firstRandomDouble = false;
	}
	return rand->RandomDouble();
}

double tgf::Radians(double deg)
{
	return (tgc::Pi/180.0) * deg;
}

bool tgf::IsPowerOf2(int v)
{
	return (v & (v - 1)) == 0;
}

int tgf::Round2Int( double val )
{
	#ifdef FAST_INT
	#define _doublemagic			double (6755399441055744.0)
	//2^52 * 1.5,  uses limited precision to floor
		val		= val + _doublemagic;
		return ((long*)&val)[0];
	#else
		return int (val+_doublemagicroundeps);
	#endif
}

int tgf::Log2Int( float v )
{
	return ((*(int *) &v) >> 23) - 127;
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

void tgf::ConcentricSampleDisk( double u1, double u2, double *dx, double *dy )
{
	double r, theta;
	// Map uniform random numbers to $[-1,1]^2$
	double sx = 2 * u1 - 1;
	double sy = 2 * u2 - 1;
	// Map square to $(r,\theta)$
	// Handle degeneracy at the origin
	if (sx == 0.0 && sy == 0.0)
	{
		*dx = 0.0;
		*dy = 0.0;
		return;
	}
	if (sx >= -sy)
	{
		if (sx > sy)
		{
			// Handle first region of disk
			r = sx;
			if (sy > 0.0)
				theta = sy/r;
			else
				theta = 8.0 + sy/r;
		}
		else
		{
			// Handle second region of disk
			r = sy;
			theta = 2.0 - sx/r;
		}
	}
	else
	{
		if (sx <= sy)
		{
			// Handle third region of disk
			r = -sx;
			theta = 4.0 - sy/r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx/r;
		}
	}
	theta *= M_PI / 4.;
	*dx = r*cos(theta);
	*dy = r*sin(theta);
}

Vector3D tgf::CosineSampleHemisphere( double u1, double u2 )
{
	Vector3D ret;
	ConcentricSampleDisk( u1, u2, &ret.x, &ret.y );
	ret.z = sqrt( std::max( 0., 1. - ret.x*ret.x - ret.y*ret.y ) );
	return ret;
}

bool tgf::SameHemisphere( const Vector3D &w, const Vector3D &wp )
{
	return w.z * wp.z > 0.0;
}

Vector3D tgf::UniformSampleHemisphere( double u1, double u2)
{
	double z = u1;
	double r = sqrt(std::max(0.0, 1.0 - z*z));
	double phi = 2 * tgc::Pi * u2;
	double x = r * cos( phi );
	double y = r * sin( phi );
	return Vector3D(x, y, z);
}

void tgf::SevereError( std::string errorMessage )
{
	std::cerr << errorMessage << std::endl;
	exit(-1);
}

void tgf::Warning( std::string warningMessage )
{
	std::cerr << warningMessage << std::endl;
}

double tgf::Clamp( double val, double low, double high )
{
	if ( val < low ) return low;
	else if ( val > high ) return high;
	else return val;
}

int tgf::Clamp( int val, int low, int high )
{
	if ( val < low ) return low;
	else if ( val > high ) return high;
	else return val;
}

double tgf::CosTheta( const Vector3D &w)
{
	return w.z;
}

double tgf::SinTheta( const Vector3D &w )
{
	return sqrt( std::max( 0.0, 1.0 - w.z*w.z ) );
}

double tgf::SinTheta2( const Vector3D &w )
{
	return 1.0 - CosTheta(w)*CosTheta(w);
}

double tgf::CosPhi(const Vector3D &w)
{
	return w.x / SinTheta(w);
}

double tgf::SinPhi(const Vector3D &w)
{
	return w.y / SinTheta(w);
}

void tgf::BuildFileList( QDir parentDirectory, QStringList& fileList )
{
    QString parentDirectoryPath( parentDirectory.absolutePath().append( "/" ) );

    foreach( QString fileName, parentDirectory.entryList( QDir::Files, QDir::Unsorted ) )
    {
    	fileList << parentDirectoryPath + fileName;
    }

    foreach( QString directoryName, parentDirectory.entryList( QDir::Dirs, QDir::Unsorted ) )
   	{
   		if( ( directoryName != "." ) && ( directoryName!= ".." ) )
   		{
   			QDir subdirectory( parentDirectoryPath + directoryName );
   			BuildFileList( subdirectory, fileList );
   		}
   	}
}

void tgf::TraceRay( Ray& ray, InstanceNode* instanceNode, PhotonMap& photonMap, RandomDeviate& rand)
{
	Trace trace( "tgf::TraceRay", false );

	Ray* reflectedRay = 0;
	Photon* node = new Photon( ray.origin, 0, 0 );
	Photon* next = 0;

	TSeparatorKit* coinNode = dynamic_cast< TSeparatorKit* > ( instanceNode->GetNode() );

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
			photonMap.store(node);
			node = next;

			//Prepare node and ray for next iteration
			ray = *reflectedRay;
			delete reflectedRay;
		}
		else intersection = false;
	}

	if( ray.maxt == HUGE_VAL  ) ray.maxt = 0.00001;

	Point3D endOfRay = ray( ray.maxt );
	Photon* lastNode = new Photon( endOfRay, node, 0 );
	node->m_next = lastNode;
	photonMap.store( node );
	photonMap.store( lastNode );
}

bool tgf::IsOdd( int number )
{
	bool answer = number & 1;
	return answer;
}


double tgf::computeSigmaOptica( double sigmaSlope, double sigmaSpecularity )
{
	//Computes the overall standard deviation of a reflective surface in milliradiands in terms of
	//the corresponding standard deviations of the slope and specularity errors
	return sqrt( 4*( sigmaSlope*sigmaSlope ) + ( sigmaSpecularity*sigmaSpecularity ) ) / 1000;
}

SoSeparator* tgf::DrawPhotonMapPoints( const PhotonMap& map )
{
	Trace trace( "tgf::DrawPhotonMapPoints", true );
	SoSeparator* drawpoints=new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	for( unsigned long i = 0 ; i < map.StoredPhotons(); i++ )
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

SoSeparator* tgf::DrawPhotonMapRays( const PhotonMap& map, unsigned long numberOfRays, double fraction )
{
	Trace trace( "tgf::DrawPhotonMapRays", false );
	SoSeparator* drawrays = new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	unsigned long drawRays = (unsigned long) ( numberOfRays * ( fraction / 100 ) );
	if( drawRays == 0 ) drawRays = 1;
	unsigned long ray = 0;
	int lines[drawRays];

	unsigned long rayLength = 0;
	unsigned long drawnRay = 0;

	unsigned long numberOfPhoton = 0;

	for( unsigned long i = 1; i<= map.StoredPhotons(); i++ )
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

  	return drawrays;
}

double tgf::AlternateBoxMuller( RandomDeviate& rand )
{
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
