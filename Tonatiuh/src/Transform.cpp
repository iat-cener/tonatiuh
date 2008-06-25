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
 
#include <cmath>
#include <cfloat>
#include "Transform.h"
#include "Normal.h"
#include "Ray.h"
#include "BBox.h"

Transform::Transform()
{
	m_mdir = m_minv = new Matrix4x4;
}

Transform::Transform( const SoTransform& coinTransform )
{
	SbMatrix coinMatrix;
  	coinMatrix.setTransform( coinTransform.translation.getValue(),
                 			 coinTransform.rotation.getValue(),
                 			 coinTransform.scaleFactor.getValue(),
				             coinTransform.scaleOrientation.getValue(),
				             coinTransform.center.getValue() );	                 	
	
	m_mdir = new Matrix4x4(	coinMatrix[0][0], coinMatrix[1][0], coinMatrix[2][0], coinMatrix[3][0],
							coinMatrix[0][1], coinMatrix[1][1], coinMatrix[2][1], coinMatrix[3][1],
							coinMatrix[0][2], coinMatrix[1][2], coinMatrix[2][2], coinMatrix[3][2],
							coinMatrix[0][3], coinMatrix[1][3], coinMatrix[2][3], coinMatrix[3][3] );
				
	m_minv = m_mdir->Inverse();
}

Transform::Transform( double mat[4][4] )
{
	m_mdir = new Matrix4x4( mat[0][0],mat[0][1],mat[0][2],mat[0][3],
	               	        mat[1][0],mat[1][1],mat[1][2],mat[1][3],
	               	        mat[2][0],mat[2][1],mat[2][2],mat[2][3],
	               	        mat[3][0],mat[3][1],mat[3][2],mat[3][3] );
	m_minv = m_mdir->Inverse();
}

Transform::Transform( const Ptr<Matrix4x4>& mdir )
: m_mdir(mdir)
{
	m_minv = m_mdir->Inverse();
}

Transform::Transform( const Ptr<Matrix4x4>& mdir, const Ptr<Matrix4x4>& minv )
: m_mdir(mdir), m_minv(minv)
{
}

Transform::Transform( double t00, double t01, double t02, double t03,
	                  double t10, double t11, double t12, double t13,
	                  double t20, double t21, double t22, double t23,
	                  double t30, double t31, double t32, double t33 )
{
	m_mdir = new Matrix4x4( t00, t01, t02, t03,
	                        t10, t11, t12, t13,
	                        t20, t21, t22, t23,
	                        t30, t31, t32, t33 );
	m_minv = m_mdir->Inverse();

}

Transform::	Transform( const Transform& rhs )
: m_mdir(rhs.m_mdir), m_minv(rhs.m_minv)
{
}

Transform Transform::GetInverse() const
{
	return Transform( m_minv, m_mdir );	
}	

Transform Transform::Transpose() const
{
	return Transform( m_mdir->Transpose(), m_mdir->Transpose()->Inverse() );	
}

Point3D Transform::operator()( const Point3D& point ) const 
{    
	double x = point.x, y = point.y, z = point.z;
	double xp = m_mdir->m[0][0]*x + m_mdir->m[0][1]*y + m_mdir->m[0][2]*z + m_mdir->m[0][3];
	double yp = m_mdir->m[1][0]*x + m_mdir->m[1][1]*y + m_mdir->m[1][2]*z + m_mdir->m[1][3];
	double zp = m_mdir->m[2][0]*x + m_mdir->m[2][1]*y + m_mdir->m[2][2]*z + m_mdir->m[2][3];
	double wp = m_mdir->m[3][0]*x + m_mdir->m[3][1]*y + m_mdir->m[3][2]*z + m_mdir->m[3][3];

	if( wp == 1.0 ) return Point3D( xp, yp, zp );
	else return Point3D( xp, yp, zp )/wp;

}

void Transform::operator()( const Point3D& point, Point3D* transformedPoint ) const
{
	double x = point.x, y = point.y, z = point.z;
	transformedPoint->x = m_mdir->m[0][0]*x + m_mdir->m[0][1]*y + m_mdir->m[0][2]*z + m_mdir->m[0][3];
	transformedPoint->y = m_mdir->m[1][0]*x + m_mdir->m[1][1]*y + m_mdir->m[1][2]*z + m_mdir->m[1][3];
	transformedPoint->z = m_mdir->m[2][0]*x + m_mdir->m[2][1]*y + m_mdir->m[2][2]*z + m_mdir->m[2][3];
	double transformedW = m_mdir->m[3][0]*x + m_mdir->m[3][1]*y + m_mdir->m[3][2]*z + m_mdir->m[3][3];
	if( transformedW != 1.0 ) *transformedPoint /= transformedW;
}

Vector3D Transform::operator()( const Vector3D& vector ) const
{
	double x = vector.x, y = vector.y, z = vector.z;
	return Vector3D( m_mdir->m[0][0]*x + m_mdir->m[0][1]*y + m_mdir->m[0][2]*z,
			         m_mdir->m[1][0]*x + m_mdir->m[1][1]*y + m_mdir->m[1][2]*z,
			         m_mdir->m[2][0]*x + m_mdir->m[2][1]*y + m_mdir->m[2][2]*z );
}

void Transform::operator()( const Vector3D& vector, Vector3D* transformedVector ) const
{
	double x = vector.x, y = vector.y, z = vector.z;
	transformedVector->x = m_mdir->m[0][0]*x + m_mdir->m[0][1]*y + m_mdir->m[0][2]*z;
	transformedVector->y = m_mdir->m[1][0]*x + m_mdir->m[1][1]*y + m_mdir->m[1][2]*z;
	transformedVector->z = m_mdir->m[2][0]*x + m_mdir->m[2][1]*y + m_mdir->m[2][2]*z;
}

Normal Transform::operator()( const Normal& normal ) const 
{
	double x = normal.x, y = normal.y, z = normal.z;
	return Normal( m_minv->m[0][0]*x + m_minv->m[1][0]*y + m_minv->m[2][0]*z,
                   m_minv->m[0][1]*x + m_minv->m[1][1]*y + m_minv->m[2][1]*z,
                   m_minv->m[0][2]*x + m_minv->m[1][2]*y + m_minv->m[2][2]*z );
}

void Transform::operator()( const Normal& normal, Normal* transformedNormal ) const
{
	double x = normal.x, y = normal.y, z = normal.z;
	transformedNormal->x = m_minv->m[0][0]*x + m_minv->m[1][0]*y + m_minv->m[2][0]*z;
	transformedNormal->y = m_minv->m[0][1]*x + m_minv->m[1][1]*y + m_minv->m[2][1]*z;
	transformedNormal->z = m_minv->m[0][2]*x + m_minv->m[1][2]*y + m_minv->m[2][2]*z;
}

Ray Transform::operator()( const Ray& ray ) const
{
	Ray transformedRay;
	( *this )( ray.origin, &transformedRay.origin );
	( *this )( ray.direction, &transformedRay.direction );
	transformedRay.mint = ray.mint;
	transformedRay.maxt = ray.maxt;
	return transformedRay;	
}

void Transform::operator()( const Ray& ray, Ray* transformedRay ) const
{
	( *this )( ray.origin, &transformedRay->origin );
	( *this )( ray.direction, &transformedRay->direction );
	transformedRay->mint = ray.mint;
	transformedRay->maxt = ray.maxt;
}

BBox Transform::operator()( const BBox& bbox  ) const
{
	const Transform& M = *this;
	BBox ret(  M( Point3D( bbox.pMin.x, bbox.pMin.y, bbox.pMin.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMax.x, bbox.pMin.y, bbox.pMin.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMin.x, bbox.pMax.y, bbox.pMin.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMin.x, bbox.pMin.y, bbox.pMax.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMin.x, bbox.pMax.y, bbox.pMax.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMax.x, bbox.pMax.y, bbox.pMin.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMax.x, bbox.pMin.y, bbox.pMax.z ) ) );
	ret = Union( ret, M( Point3D( bbox.pMax.x, bbox.pMax.y, bbox.pMax.z ) ) );
	return ret;
}

void Transform::operator()( const BBox& bbox, BBox* transformedBbox  ) const
{
	const Transform &M = *this;
	transformedBbox = new BBox(          M( Point3D( bbox.pMin.x, bbox.pMin.y, bbox.pMin.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMax.x, bbox.pMin.y, bbox.pMin.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMin.x, bbox.pMax.y, bbox.pMin.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMin.x, bbox.pMin.y, bbox.pMax.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMin.x, bbox.pMax.y, bbox.pMax.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMax.x, bbox.pMax.y, bbox.pMin.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMax.x, bbox.pMin.y, bbox.pMax.z ) ) );
	*transformedBbox = Union( *transformedBbox, M( Point3D( bbox.pMax.x, bbox.pMax.y, bbox.pMax.z ) ) );
}


Transform Transform::operator*( const Transform& rhs ) const 
{
	Ptr<Matrix4x4> mdir = Mul( m_mdir, rhs.m_mdir );
	Ptr<Matrix4x4> minv = Mul( rhs.m_minv, m_minv );
	return Transform( mdir, minv );
}

bool Transform::SwapsHandedness( ) const 
{
	double det = ( ( m_mdir->m[0][0] * 
	                   ( m_mdir->m[1][1] * m_mdir->m[2][2] - 
	                     m_mdir->m[1][2] * m_mdir->m[2][1] ) ) -
                   ( m_mdir->m[0][1] *
                       ( m_mdir->m[1][0] * m_mdir->m[2][2] -
                         m_mdir->m[1][2] * m_mdir->m[2][0] ) ) +
                   ( m_mdir->m[0][2] *
                       ( m_mdir->m[1][0] * m_mdir->m[2][1] -
                         m_mdir->m[1][1] * m_mdir->m[2][0] ) ) );
	return det < 0.0;
}

bool Transform::operator==( const Transform& tran ) const
{
    if( this == &tran ) 
    	return true;
    else 
    return( 
    	( fabs(m_mdir->m[0][0] - tran.m_mdir->m[0][0]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[0][1] - tran.m_mdir->m[0][1]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[0][2] - tran.m_mdir->m[0][2]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[0][3] - tran.m_mdir->m[0][3]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[1][0] - tran.m_mdir->m[1][0]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[1][1] - tran.m_mdir->m[1][1]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[1][2] - tran.m_mdir->m[1][2]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[1][3] - tran.m_mdir->m[1][3]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[2][0] - tran.m_mdir->m[2][0]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[2][1] - tran.m_mdir->m[2][1]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[2][2] - tran.m_mdir->m[2][2]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[2][3] - tran.m_mdir->m[2][3]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[3][0] - tran.m_mdir->m[3][0]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[3][1] - tran.m_mdir->m[3][1]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[3][2] - tran.m_mdir->m[3][2]) < DBL_EPSILON ) && 
    	( fabs(m_mdir->m[3][3] - tran.m_mdir->m[3][3]) < DBL_EPSILON ) );
}

Transform Translate( const Vector3D& delta ) 
{
	Ptr<Matrix4x4> mdir = new Matrix4x4(   1.0,   0.0,   0.0,  delta.x,
                                           0.0,   1.0,   0.0,  delta.y,
                                           0.0,   0.0,   1.0,  delta.z,
                                           0.0,   0.0,   0.0,      1.0  );

	Ptr<Matrix4x4> minv = new Matrix4x4(   1.0,   0.0,   0.0, -delta.x,
                                           0.0,   1.0,   0.0, -delta.y,
                                           0.0,   0.0,   1.0, -delta.z,
                                           0.0,   0.0,   0.0,      1.0  );
                                           
	return Transform( mdir, minv );
}

Transform Translate( double x, double y, double z)
{
	Ptr<Matrix4x4> mdir = new Matrix4x4(   1.0,   0.0,   0.0,   x,
                                           0.0,   1.0,   0.0,   y,
                                           0.0,   0.0,   1.0,   z,
                                           0.0,   0.0,   0.0, 1.0 );

	Ptr<Matrix4x4> minv = new Matrix4x4(   1.0,   0.0,   0.0,   -x,
                                           0.0,   1.0,   0.0,   -y,
                                           0.0,   0.0,   1.0,   -z,
                                           0.0,   0.0,   0.0,  1.0 );

	return Transform( mdir, minv );
}

Transform Scale( double sx, double sy, double sz ) 
{
	Ptr<Matrix4x4> mdir = new Matrix4x4(    sx,     0.0,    0.0,  0.0,
                                           0.0,      sy,    0.0,  0.0,
                                           0.0,     0.0,     sz,  0.0,
                                           0.0,     0.0,    0.0,  1.0 );
                                           
	Ptr<Matrix4x4> minv = new Matrix4x4( 1.0/sx,    0.0,    0.0,  0.0,
                                            0.0, 1.0/sy,    0.0,  0.0,
                                            0.0,    0.0, 1.0/sz,  0.0,
                                            0.0,    0.0,    0.0,  1.0 );

	return Transform( mdir, minv );
}

Transform RotateX( double angle ) 
{
	// angle is assumed to be in radians.
	double sinAngle = sin( angle );
	double cosAngle = cos( angle );

	Ptr<Matrix4x4> mdir = new Matrix4x4( 1.0,      0.0,       0.0, 0.0,
                                         0.0, cosAngle, -sinAngle, 0.0,
                                         0.0, sinAngle,  cosAngle, 0.0,
                                         0.0,      0.0,       0.0, 1.0  );
                             
	return Transform( mdir, mdir->Transpose() );
}

Transform RotateY(double angle) 
{
	// angle is assumed to be in radians.	
	double sinAngle = sin( angle );
	double cosAngle = cos( angle );
	
	Ptr<Matrix4x4> mdir = new Matrix4x4( cosAngle, 0.0, sinAngle, 0.0,
                                              0.0, 1.0,      0.0, 0.0,
                                        -sinAngle, 0.0, cosAngle, 0.0,
                                              0.0, 0.0,      0.0, 1.0  ); 
                                 
	return Transform( mdir, mdir->Transpose() );
}


Transform RotateZ( double angle ) 
{
	// angle is assumed to be in radians.	
	double sinAngle = sin( angle );
	double cosAngle = cos( angle );
	
	Ptr<Matrix4x4> mdir = new Matrix4x4( cosAngle, -sinAngle, 0.0, 0.0,
                                         sinAngle,  cosAngle, 0.0, 0.0,
                                              0.0,       0.0, 1.0, 0.0,
                                              0.0,       0.0, 0.0, 1.0 );
                                              
	return Transform( mdir, mdir->Transpose() );
}

Transform Rotate( double angle, const Vector3D& axis ) 
{
	// angle is assumed to be in radians.
	Vector3D a = Normalize( axis );
	double s = sin( angle );
	double c = cos( angle );
	double m[4][4];

	m[0][0] = a.x * a.x + (1.0 - a.x * a.x) * c;
	m[0][1] = a.x * a.y * (1.0 - c) - a.z * s;
	m[0][2] = a.x * a.z * (1.0 - c) + a.y * s;
	m[0][3] = 0.0;

	m[1][0] = a.x * a.y * (1.0 - c) + a.z * s;
	m[1][1] = a.y * a.y + (1.0 - a.y * a.y) * c;
	m[1][2] = a.y * a.z * (1.0 - c) - a.x * s;
	m[1][3] = 0.0;

	m[2][0] = a.x * a.z * (1.0 - c) - a.y * s;
	m[2][1] = a.y * a.z * (1.0 - c) + a.x * s;
	m[2][2] = a.z * a.z + (1.0 - a.z * a.z) * c;
	m[2][3] = 0.0;

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;

	Ptr<Matrix4x4> mdir = new Matrix4x4( m );
	return Transform( mdir, mdir->Transpose() );
}

Transform LookAt( const Point3D& pos, const Point3D& look, const Vector3D& up )
{
	double m[4][4];
	m[0][3] = pos.x;
	m[1][3] = pos.y;
	m[2][3] = pos.z;
	m[3][3] = 1.0;
	
	Vector3D dir = Normalize( look - pos );
	Vector3D right = CrossProduct( dir, Normalize( up ) );
	Vector3D newUp = CrossProduct( right, dir );

	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;
	m[3][0] = 0.0;
	
	m[0][1] = dir.x;
	m[1][1] = dir.y;
	m[2][1] = dir.z;
	m[3][1] = 0.0;
	
	m[0][2] = newUp.x;
	m[1][2] = newUp.y;
	m[2][2] = newUp.z;
	m[3][2] = 0.0;

	Ptr<Matrix4x4> camToWorld = new Matrix4x4( m );
	return Transform( camToWorld->Inverse(), camToWorld );
}

std::ostream& operator<<( std::ostream& os, const Transform& tran )
{
	os << (*tran.GetMatrix());
	return os;
}
