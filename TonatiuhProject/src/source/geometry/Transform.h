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

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <iostream>

#include "Matrix4x4.h"
#include "Ptr.h"

struct Point3D;
struct Vector3D;
struct NormalVector;
class Ray;
struct BBox;

class Transform
{
public:
	Transform( );
	Transform( double mat[4][4] );
	Transform( const Ptr<Matrix4x4>& mdir );
	Transform( const Ptr<Matrix4x4>& mdir,  const Ptr<Matrix4x4>& minv );
	Transform( double t00, double t01, double t02, double t03,
               double t10, double t11, double t12, double t13,
	           double t20, double t21, double t22, double t23,
	           double t30, double t31, double t32, double t33 );

	Point3D operator()( const Point3D& point ) const;
	void operator()( const Point3D& point, Point3D& transformedPoint ) const;
	Vector3D operator()( const Vector3D& vector ) const;
	void operator()( const Vector3D& vector, Vector3D& transformedVector ) const;
	NormalVector operator()( const NormalVector& normal ) const;
	void operator()( const NormalVector& normal, NormalVector& transformedNormal ) const;
	Ray operator()( const Ray& ray ) const;
	void operator()( const Ray& ray, Ray& transformedRay ) const;
	BBox operator()( const BBox& bbox  ) const;
	void operator()( const BBox& bbox, BBox& transformedBbox  ) const;
	Transform operator*( const Transform& rhs ) const;

	bool operator==( const Transform& mat ) const;

	Ptr<Matrix4x4> GetMatrix() const {return m_mdir;}
	Transform Transpose() const;
	Transform GetInverse() const ;
	bool SwapsHandedness( ) const;
	Vector3D multMatrixVec(const Vector3D & src) const;
	Vector3D multDirMatrix(const Vector3D & src) const;

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

std::ostream& operator<<( std::ostream& os, const Transform& tran );

#endif /*TRANSFORMATION_H_*/

