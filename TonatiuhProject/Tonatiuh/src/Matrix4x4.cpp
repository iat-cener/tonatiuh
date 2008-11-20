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

#include <cstring>
#include <cmath>
#include <cfloat>

#include "Matrix4x4.h"
#include "Trace.h"

const double Epsilon = DBL_EPSILON;

Matrix4x4::Matrix4x4( )
{
	Trace trace( "Matrix4x4::Matrix4x4", false );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			if( i == j )
				m[i][j] = 1.0;
			else
				m[i][j] = 0.0;
		}
	}
}

Matrix4x4::Matrix4x4( double mat[4][4] )
{
	Trace trace( "Matrix4x4::Matrix4x4", false );
	memcpy( m, mat, 16*sizeof( double ) );
}

Matrix4x4::Matrix4x4( double t00, double t01, double t02, double t03,
	                  double t10, double t11, double t12, double t13,
	                  double t20, double t21, double t22, double t23,
	                  double t30, double t31, double t32, double t33)
{
	Trace trace( "Matrix4x4::Matrix4x4", false );

	m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
	m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
	m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
	m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
}

Matrix4x4::Matrix4x4( const Matrix4x4& rhs )
: RefCount()
{
	Trace trace( "Matrix4x4::Matrix4x4", false );
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) m[i][j] = rhs.m[i][j];
}

bool Matrix4x4::operator==( const Matrix4x4& mat ) const
{
	Trace trace( "Matrix4x4::operator==", false );
    if( this == &mat )
    	return true;
    else
    return(
    	( fabs(m[0][0] - mat.m[0][0]) < DBL_EPSILON ) &&
    	( fabs(m[0][1] - mat.m[0][1]) < DBL_EPSILON ) &&
    	( fabs(m[0][2] - mat.m[0][2]) < DBL_EPSILON ) &&
    	( fabs(m[0][3] - mat.m[0][3]) < DBL_EPSILON ) &&
    	( fabs(m[1][0] - mat.m[1][0]) < DBL_EPSILON ) &&
    	( fabs(m[1][1] - mat.m[1][1]) < DBL_EPSILON ) &&
    	( fabs(m[1][2] - mat.m[1][2]) < DBL_EPSILON ) &&
    	( fabs(m[1][3] - mat.m[1][3]) < DBL_EPSILON ) &&
    	( fabs(m[2][0] - mat.m[2][0]) < DBL_EPSILON ) &&
    	( fabs(m[2][1] - mat.m[2][1]) < DBL_EPSILON ) &&
    	( fabs(m[2][2] - mat.m[2][2]) < DBL_EPSILON ) &&
    	( fabs(m[2][3] - mat.m[2][3]) < DBL_EPSILON ) &&
    	( fabs(m[3][0] - mat.m[3][0]) < DBL_EPSILON ) &&
    	( fabs(m[3][1] - mat.m[3][1]) < DBL_EPSILON ) &&
    	( fabs(m[3][2] - mat.m[3][2]) < DBL_EPSILON ) &&
    	( fabs(m[3][3] - mat.m[3][3]) < DBL_EPSILON ) );
}

std::ostream& operator<<( std::ostream& os, const Matrix4x4& mat )
{
	Trace trace( "Matrix4x4 operator<<", false );
	for (int i = 0; i < 4; ++i)
	{
		os << "[ ";
		for (int j = 0; j < 4; ++j)
		{
			if( mat.m[i][j] < Epsilon && mat.m[i][j] > -Epsilon )
				os << "0";
			else
				os << mat.m[i][j];
			if (j != 3)
				os << ", ";
		}
		os << " ] " << std::endl;
	}
	return os;
}

Ptr<Matrix4x4> Matrix4x4::Transpose( ) const
{
	Trace trace( "Matrix4x4::Transpose", false );

	return new Matrix4x4( m[0][0], m[1][0], m[2][0], m[3][0],
	                      m[0][1], m[1][1], m[2][1], m[3][1],
	                      m[0][2], m[1][2], m[2][2], m[3][2],
	                      m[0][3], m[1][3], m[2][3], m[3][3] );
}

Ptr<Matrix4x4> Matrix4x4::Inverse() const
{
	Trace trace( "Matrix4x4::Inverse", false );

	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	double minv[4][4];
	memcpy( minv, m, 16*sizeof( double ) );
	for (int i = 0; i < 4; i++)
	{
		int irow = -1, icol = -1;
		double big = 0.;
		// Choose pivot
		for (int j = 0; j < 4; j++)
		{
			if (ipiv[j] != 1)
			{
				for (int k = 0; k < 4; k++)
				{
					if (ipiv[k] == 0)
					{
						if ( fabs( minv[j][k] ) >= big )
						{
							big = double( fabs( minv[j][k] ) );
							irow = j;
							icol = k;
						}
					}
					//else if ( ipiv[k] > 1 ) tgf::SevereError( "Singular matrix in Matrix4x4::Inverse()" );
				}
			}
		}
		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol)
		{
			for (int k = 0; k < 4; ++k)
				std::swap(minv[irow][k], minv[icol][k]);
		}
		indxr[i] = irow;
		indxc[i] = icol;
		//if ( minv[icol][icol] == 0.0 ) tgf::SevereError( "Singular matrix in Matrix4x4::Inverse()" );
		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		double pivinv = 1.f / minv[icol][icol];
		minv[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++)
			minv[icol][j] *= pivinv;
		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++)
		{
			if ( j != icol )
			{
				double save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++)
					minv[j][k] -= minv[icol][k]*save;
			}
		}
	}
	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--)
	{
		if (indxr[j] != indxc[j])
		{
			for (int k = 0; k < 4; k++)
				std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
		}
	}
	return new Matrix4x4(minv);
}

Ptr<Matrix4x4> Mul( const Ptr<Matrix4x4>& m1, const Ptr<Matrix4x4>& m2 )
{
	Trace trace( "Matrix4x4 Mul", false );
	double r[4][4];
	for( int i = 0; i < 4; ++i )
		for( int j = 0; j < 4; ++j )
			r[i][j] = m1->m[i][0] * m2->m[0][j] +
			          m1->m[i][1] * m2->m[1][j] +
			          m1->m[i][2] * m2->m[2][j] +
			          m1->m[i][3] * m2->m[3][j];
	return new Matrix4x4(r);
}
