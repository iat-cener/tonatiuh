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
 
#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <iostream>

#include <Inventor/nodes/SoTransform.h>

#include "Matrix4x4.h"
#include "Ptr.h"

struct Point3D;
struct Vector3D;
struct Normal;
struct Ray;
struct BBox;

class Transform 
{
public:
	Transform( );
	Transform( const SoTransform& coinTransform );
	Transform( double mat[4][4] );
	Transform( const Ptr<Matrix4x4>& mdir );
	Transform( const Ptr<Matrix4x4>& mdir,  const Ptr<Matrix4x4>& minv );
	Transform( double t00, double t01, double t02, double t03,
               double t10, double t11, double t12, double t13,
	           double t20, double t21, double t22, double t23,
	           double t30, double t31, double t32, double t33 ); 
	Transform( const Transform& rhs );

	Transform Transpose() const;
	Transform GetInverse() const ;
	Point3D operator()( const Point3D& point ) const;
	void operator()( const Point3D& point, Point3D* transformedPoint ) const;
	Vector3D operator()( const Vector3D& vector ) const;
	void operator()( const Vector3D& vector, Vector3D* transformedVector ) const;
	Normal operator()( const Normal& normal ) const;
	void operator()( const Normal& normal, Normal* transformedNormal ) const;
	Ray operator()( const Ray& ray ) const;
	void operator()( const Ray& ray, Ray* transformedRay ) const;
	BBox operator()( const BBox& bbox  ) const;
	void operator()( const BBox& bbox, BBox* transformedBbox  ) const;
	Transform operator*( const Transform& rhs ) const;
	bool SwapsHandedness( ) const;
	
	bool operator==( const Transform& mat ) const;

	Ptr<Matrix4x4> GetMatrix() const {return m_mdir;}

	friend std::ostream& operator<<( std::ostream& os, const Transform& tran );
	 
private:
	Ptr<Matrix4x4> m_mdir;
	Ptr<Matrix4x4> m_minv; 
};

Transform Translate( const Vector3D& delta );
Transform Translate( double x, double y, double z);
Transform Scale( double x, double y, double z );
Transform RotateX( double angle );
Transform RotateY( double angle );
Transform RotateZ( double angle );
Transform Rotate( double angle, const Vector3D& axis );
Transform LookAt( const Point3D& pos, const Point3D& look, const Vector3D& up );

//std::ostream& operator<<( std::ostream& os, const Transform& tran );

#endif /*TRANSFORMATION_H_*/

