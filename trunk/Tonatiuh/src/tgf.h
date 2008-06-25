/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
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
