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
***************************************************************************/

#include <cstring>
#include "gc.h"
#include "gf.h"

#include "Matrix4x4.h"

Matrix4x4::Matrix4x4( )
: RefCount()
{
	for( int i = 0; i < 4; ++i )
		for( int j = 0; j < 4; ++j )
		{
			if( i == j ) m[i][j] = 1.0;
			else m[i][j] = 0.0;
		}
}

Matrix4x4::Matrix4x4( double array[4][4] )
: RefCount()
{
	memcpy( m, array, 16*sizeof( double ) );
}

Matrix4x4::Matrix4x4( double t00, double t01, double t02, double t03,
	                  double t10, double t11, double t12, double t13,
	                  double t20, double t21, double t22, double t23,
	                  double t30, double t31, double t32, double t33 )
: RefCount()
{
	m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
	m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
	m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
	m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
}

Matrix4x4::Matrix4x4( const Matrix4x4& rhs )
: RefCount()
{
	for ( int i = 0; i < 4; ++i )
		for ( int j = 0; j < 4; ++j) m[i][j] = rhs.m[i][j];
}

Matrix4x4::~Matrix4x4( )
{
}

bool Matrix4x4::operator==( const Matrix4x4& matrix ) const
{
	if( this == &matrix ) return true;
    else
    return(
    	( fabs( m[0][0] - matrix.m[0][0]) < gc::Epsilon ) &&
    	( fabs( m[0][1] - matrix.m[0][1]) < gc::Epsilon ) &&
    	( fabs( m[0][2] - matrix.m[0][2]) < gc::Epsilon ) &&
    	( fabs( m[0][3] - matrix.m[0][3]) < gc::Epsilon ) &&
    	( fabs( m[1][0] - matrix.m[1][0]) < gc::Epsilon ) &&
    	( fabs( m[1][1] - matrix.m[1][1]) < gc::Epsilon ) &&
    	( fabs( m[1][2] - matrix.m[1][2]) < gc::Epsilon ) &&
    	( fabs( m[1][3] - matrix.m[1][3]) < gc::Epsilon ) &&
    	( fabs( m[2][0] - matrix.m[2][0]) < gc::Epsilon ) &&
    	( fabs( m[2][1] - matrix.m[2][1]) < gc::Epsilon ) &&
    	( fabs( m[2][2] - matrix.m[2][2]) < gc::Epsilon ) &&
    	( fabs( m[2][3] - matrix.m[2][3]) < gc::Epsilon ) &&
    	( fabs( m[3][0] - matrix.m[3][0]) < gc::Epsilon ) &&
    	( fabs( m[3][1] - matrix.m[3][1]) < gc::Epsilon ) &&
    	( fabs( m[3][2] - matrix.m[3][2]) < gc::Epsilon ) &&
    	( fabs( m[3][3] - matrix.m[3][3]) < gc::Epsilon ) );
}

Ptr<Matrix4x4> Matrix4x4::Transpose( ) const
{
	return new Matrix4x4( m[0][0], m[1][0], m[2][0], m[3][0],
	                      m[0][1], m[1][1], m[2][1], m[3][1],
	                      m[0][2], m[1][2], m[2][2], m[3][2],
	                      m[0][3], m[1][3], m[2][3], m[3][3] );
}

Ptr<Matrix4x4> Matrix4x4::Inverse( ) const
{
	double det = m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][2]*m[2][3]*m[3][0] - m[0][0]*m[1][3]*m[2][2]*m[3][1] + m[0][0]*m[1][2]*m[2][3]*m[3][1]
                -m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][0]*m[1][3]*m[2][1]*m[3][2] + m[0][1]*m[1][0]*m[2][3]*m[3][2] - m[0][0]*m[1][1]*m[2][3]*m[3][2]
			    +m[0][3]*(  m[1][2]*m[2][1]*m[3][0] - m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1] + m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] - m[1][0]*m[2][1]*m[3][2] )
			    +m[3][3]*(  m[0][1]*m[1][2]*m[2][0] - m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2] + m[0][0]*m[1][1]*m[2][2] )
			    +m[0][2]*( -m[1][3]*m[2][1]*m[3][0] + m[1][1]*m[2][3]*m[3][0] + m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] - m[1][1]*m[2][0]*m[3][3] + m[1][0]*m[2][1]*m[3][3]);

	if ( fabs( det ) < gc::Epsilon ) gf::SevereError( "Singular matrix in Matrix4x4::Inverse()" );
	double alpha = 1.0/det;

	double inv00 = ( -m[1][3]*m[2][2]*m[3][1] + m[1][2]*m[2][3]*m[3][1] + m[1][3]*m[2][1]*m[3][2] - m[1][1]*m[2][3]*m[3][2] - m[1][2]*m[2][1]*m[3][3] + m[1][1]*m[2][2]*m[3][3] )*alpha;
	double inv01 = (  m[0][3]*m[2][2]*m[3][1] - m[0][2]*m[2][3]*m[3][1] - m[0][3]*m[2][1]*m[3][2] + m[0][1]*m[2][3]*m[3][2] + m[0][2]*m[2][1]*m[3][3] - m[0][1]*m[2][2]*m[3][3] )*alpha;
	double inv02 = ( -m[0][3]*m[1][2]*m[3][1] + m[0][2]*m[1][3]*m[3][1] + m[0][3]*m[1][1]*m[3][2] - m[0][1]*m[1][3]*m[3][2] - m[0][2]*m[1][1]*m[3][3] + m[0][1]*m[1][2]*m[3][3] )*alpha;
	double inv03 = (  m[0][3]*m[1][2]*m[2][1] - m[0][2]*m[1][3]*m[2][1] - m[0][3]*m[1][1]*m[2][2] + m[0][1]*m[1][3]*m[2][2] + m[0][2]*m[1][1]*m[2][3] - m[0][1]*m[1][2]*m[2][3] )*alpha;
	double inv10 = (  m[1][3]*m[2][2]*m[3][0] - m[1][2]*m[2][3]*m[3][0] - m[1][3]*m[2][0]*m[3][2] + m[1][0]*m[2][3]*m[3][2] + m[1][2]*m[2][0]*m[3][3] - m[1][0]*m[2][2]*m[3][3] )*alpha;
	double inv11 = ( -m[0][3]*m[2][2]*m[3][0] + m[0][2]*m[2][3]*m[3][0] + m[0][3]*m[2][0]*m[3][2] - m[0][0]*m[2][3]*m[3][2] - m[0][2]*m[2][0]*m[3][3] + m[0][0]*m[2][2]*m[3][3] )*alpha;
	double inv12 = (  m[0][3]*m[1][2]*m[3][0] - m[0][2]*m[1][3]*m[3][0] - m[0][3]*m[1][0]*m[3][2] + m[0][0]*m[1][3]*m[3][2] + m[0][2]*m[1][0]*m[3][3] - m[0][0]*m[1][2]*m[3][3] )*alpha;
	double inv13 = ( -m[0][3]*m[1][2]*m[2][0] + m[0][2]*m[1][3]*m[2][0] + m[0][3]*m[1][0]*m[2][2] - m[0][0]*m[1][3]*m[2][2] - m[0][2]*m[1][0]*m[2][3] + m[0][0]*m[1][2]*m[2][3] )*alpha;
	double inv20 = ( -m[1][3]*m[2][1]*m[3][0] + m[1][1]*m[2][3]*m[3][0] + m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] - m[1][1]*m[2][0]*m[3][3] + m[1][0]*m[2][1]*m[3][3] )*alpha;
	double inv21 = (  m[0][3]*m[2][1]*m[3][0] - m[0][1]*m[2][3]*m[3][0] - m[0][3]*m[2][0]*m[3][1] + m[0][0]*m[2][3]*m[3][1] + m[0][1]*m[2][0]*m[3][3] - m[0][0]*m[2][1]*m[3][3] )*alpha;
	double inv22 = ( -m[0][3]*m[1][1]*m[3][0] + m[0][1]*m[1][3]*m[3][0] + m[0][3]*m[1][0]*m[3][1] - m[0][0]*m[1][3]*m[3][1] - m[0][1]*m[1][0]*m[3][3] + m[0][0]*m[1][1]*m[3][3] )*alpha;
	double inv23 = (  m[0][3]*m[1][1]*m[2][0] - m[0][1]*m[1][3]*m[2][0] - m[0][3]*m[1][0]*m[2][1] + m[0][0]*m[1][3]*m[2][1] + m[0][1]*m[1][0]*m[2][3] - m[0][0]*m[1][1]*m[2][3] )*alpha;
	double inv30 = (  m[1][2]*m[2][1]*m[3][0] - m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1] + m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] - m[1][0]*m[2][1]*m[3][2] )*alpha;
	double inv31 = ( -m[0][2]*m[2][1]*m[3][0] + m[0][1]*m[2][2]*m[3][0] + m[0][2]*m[2][0]*m[3][1] - m[0][0]*m[2][2]*m[3][1] - m[0][1]*m[2][0]*m[3][2] + m[0][0]*m[2][1]*m[3][2] )*alpha;
	double inv32 = (  m[0][2]*m[1][1]*m[3][0] - m[0][1]*m[1][2]*m[3][0] - m[0][2]*m[1][0]*m[3][1] + m[0][0]*m[1][2]*m[3][1] + m[0][1]*m[1][0]*m[3][2] - m[0][0]*m[1][1]*m[3][2] )*alpha;
	double inv33 = ( -m[0][2]*m[1][1]*m[2][0] + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2] + m[0][0]*m[1][1]*m[2][2] )*alpha;

	return new Matrix4x4( inv00, inv01, inv02, inv03, inv10, inv11, inv12, inv13, inv20, inv21, inv22, inv23, inv30, inv31, inv32, inv33 );
}

Ptr<Matrix4x4> Mul( const Ptr<Matrix4x4>& m1, const Ptr<Matrix4x4>& m2 )
{
	double r[4][4];
	for( int i = 0; i < 4; ++i )
		for( int j = 0; j < 4; ++j )
			r[i][j] = m1->m[i][0] * m2->m[0][j] +
			          m1->m[i][1] * m2->m[1][j] +
			          m1->m[i][2] * m2->m[2][j] +
			          m1->m[i][3] * m2->m[3][j];
	return new Matrix4x4(r);
}

std::ostream& operator<<( std::ostream& os, const Matrix4x4& matrix )
{
	for (int i = 0; i < 4; ++i)
	{
		os << "[ ";
		for (int j = 0; j < 4; ++j)
		{
			if( fabs( matrix.m[i][j] ) < gc::Epsilon ) os << "0";
			else os << matrix.m[i][j];
			if (j != 3) os << ", ";
		}
		os << " ] " << std::endl;
	}
	return os;
}
