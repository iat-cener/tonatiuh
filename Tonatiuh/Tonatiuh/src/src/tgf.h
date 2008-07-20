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
 
#ifndef TGF_H_
#define TGF_H_

#include <QDir>
#include <QStringList>
#include <QList>

#include <Inventor/SbBox.h> 

#include "Vector3D.h"
#include "Ray.h"
#include "MersenneTwister.h"
#include "PhotonMap.h"
#include "TShapeKit.h"
#include "Transform.h"

#include <map>

class InstanceNode;

namespace tgf 
{	
	double Clamp( double val, double low, double high );
	int Clamp( int val, int low, int high );
	int Round2Int( double val ) ;
	int Log2Int( float v ); 
	
	void InitRand();
	double RandomDouble();
	
    bool Quadratic( double A, double B, double C, double* t0, double* t1);
    void ConcentricSampleDisk( double u1, double u2, double* dx, double* dy );
    Vector3D CosineSampleHemisphere( double u1, double u2 );
    Vector3D UniformSampleHemisphere( double u1, double u2); 
    bool SameHemisphere( const Vector3D &w, const Vector3D &wp );
    
    void SevereError( std::string errorMessage );
    void Warning( std::string warningMessage );

	bool IsPowerOf2( int v );
	double Radians( double deg );
	double CosTheta( const Vector3D &w );
	double SinTheta( const Vector3D &w );
	double SinTheta2( const Vector3D &w );
	double CosPhi( const Vector3D &w );
	double SinPhi( const Vector3D &w );
	void LatinHypercube( double* samples, int nSamples, int dims );
	
	void BuildFileList( QDir parentDirectory, QStringList& fileList );
	
	void TraceRay( Ray& ray, InstanceNode* instanceNode, PhotonMap& photonMap, RandomDeviate& rand );
	bool IsOdd( int number );
	double computeSigmaOptica( double sigmaSlope, double sigmaSpecularity );
	
	SoSeparator* DrawPhotonMapPoints( const PhotonMap& map );
	SoSeparator* DrawPhotonMapRays( const PhotonMap& map, unsigned long numberOfRays, double fraction );
	
	SoSeparator* DrawPhotonMapPoints ( const PhotonMap& map );
	SoSeparator* DrawPhotonMapRays ( const PhotonMap& map, unsigned long numberOfRays, double fraction );
	
	double AlternateBoxMuller( RandomDeviate& rand );
}

#endif /*TGF_H_*/
