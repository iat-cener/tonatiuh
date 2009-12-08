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

#include "tgc.h"
#include "tgf.h"
#include "TnMatrix2x2.h"

TnMatrix2x2::TnMatrix2x2( )
{
	m[0][0] = 1.0; m[0][1] = 0.0;
	m[1][0] = 0.0; m[1][1] = 1.0;
}

TnMatrix2x2::TnMatrix2x2( double array[2][2] )
{
	m[0][0] = array[0][0]; m[0][1] = array[0][1];
	m[1][0] = array[1][0]; m[1][1] = array[1][1];
}

TnMatrix2x2::TnMatrix2x2( double t00, double t01,
	                  double t10, double t11 )
{
	m[0][0] = t00; m[0][1] = t01;
	m[1][0] = t10; m[1][1] = t11;
}

TnMatrix2x2::TnMatrix2x2( const TnMatrix2x2& rhs )
{
	m[0][0] = rhs.m[0][0]; m[0][1] = rhs.m[0][1];
	m[1][0] = rhs.m[1][0]; m[1][1] = rhs.m[1][1];
}

TnMatrix2x2::~TnMatrix2x2( )
{
}

TnMatrix2x2& TnMatrix2x2::operator+=( const TnMatrix2x2& matrix )
{
    m[0][0] += matrix.m[0][0];  m[0][1] += matrix.m[0][1];
    m[1][0] += matrix.m[1][0];  m[1][1] += matrix.m[1][1];
	return *this;
}

TnMatrix2x2 TnMatrix2x2::operator+( const TnMatrix2x2& matrix ) const
{
	return TnMatrix2x2( m[0][0]+matrix.m[0][0], m[0][1]+matrix.m[0][1],
	                  m[1][0]+matrix.m[1][0], m[1][1]+matrix.m[1][1]  );
}

TnMatrix2x2& TnMatrix2x2::operator-=( const TnMatrix2x2& matrix )
{
    m[0][0] -= matrix.m[0][0];  m[0][1] -= matrix.m[0][1];
    m[1][0] -= matrix.m[1][0];  m[1][1] -= matrix.m[1][1];
	return *this;
}

TnMatrix2x2 TnMatrix2x2::operator-( const TnMatrix2x2& matrix ) const
{
	return TnMatrix2x2( m[0][0]-matrix.m[0][0], m[0][1]-matrix.m[0][1],
	                  m[1][0]-matrix.m[1][0], m[1][1]-matrix.m[1][1]  );
}

TnMatrix2x2& TnMatrix2x2::operator*=( const TnMatrix2x2& matrix )
{
	*this = (*this) * matrix;
	return *this;
}

TnMatrix2x2 TnMatrix2x2::operator*( const TnMatrix2x2& matrix ) const
{
	double array[2][2] = { { 0.0, 0.0 },
			               { 0.0, 0.0 } };
	for ( int i = 0; i < 2; ++i )
		for ( int j = 0; j < 2; ++j )
			array[i][j] = m[i][0] * matrix.m[0][j] +
			              m[i][1] * matrix.m[1][j];
	return TnMatrix2x2( array );
}

TnMatrix2x2& TnMatrix2x2::operator*=( double scalar )
{
    m[0][0] *= scalar;  m[0][1] *= scalar;
    m[1][0] *= scalar;  m[1][1] *= scalar;
	return *this;
}

TnMatrix2x2 TnMatrix2x2::operator*( double scalar ) const
{
	return TnMatrix2x2( m[0][0]*scalar, m[0][1]*scalar,
	                  m[1][0]*scalar, m[1][1]*scalar  );
}

TnMatrix2x2& TnMatrix2x2::operator/=( double scalar )
{
	if( scalar <= tgc::Epsilon ) tgf::SevereError( "Division by zero in TnMatrix2x2::operator/=" );
	long double inverseScalar = 1.0 / scalar;
    m[0][0] *= inverseScalar;  m[0][1] *= inverseScalar;
    m[1][0] *= inverseScalar;  m[1][1] *= inverseScalar;
	return *this;
}

TnMatrix2x2 TnMatrix2x2::operator/( double scalar ) const
{
	if( scalar <= tgc::Epsilon ) tgf::SevereError( "Division by zero in TnMatrix2x2::operator/" );
	long double inverseScalar = 1.0 / scalar;
	return TnMatrix2x2( m[0][0]*inverseScalar, m[0][1]*inverseScalar,
	                  m[1][0]*inverseScalar, m[1][1]*inverseScalar  );
}

TnMatrix2x2 TnMatrix2x2::operator-() const
{
	return TnMatrix2x2( -m[0][0], -m[0][1],
	                  -m[1][0], -m[1][1]  );
}

bool TnMatrix2x2::operator==( const TnMatrix2x2& matrix ) const
{
	if( this == &matrix ) return true;
    else return( !( fabs( m[0][0] - matrix.m[0][0]) > tgc::Epsilon ) &&
				 !( fabs( m[0][1] - matrix.m[0][1]) > tgc::Epsilon ) &&
				 !( fabs( m[1][0] - matrix.m[1][0]) > tgc::Epsilon ) &&
				 !( fabs( m[1][1] - matrix.m[1][1]) > tgc::Epsilon ) );
}

bool TnMatrix2x2::operator!=( const TnMatrix2x2& matrix ) const
{
	if( this == &matrix ) return false;
    else return( ( fabs( m[0][0] - matrix.m[0][0]) > tgc::Epsilon ) ||
				 ( fabs( m[0][1] - matrix.m[0][1]) > tgc::Epsilon ) ||
				 ( fabs( m[1][0] - matrix.m[1][0]) > tgc::Epsilon ) ||
				 ( fabs( m[1][1] - matrix.m[1][1]) > tgc::Epsilon ) );
}

double TnMatrix2x2::Determinant() const
{
	return  m[0][0]*m[1][1] - m[0][1]*m[1][0];
}

TnMatrix2x2 TnMatrix2x2::Transpose( ) const
{
	return TnMatrix2x2( m[0][0], m[1][0],
	                  m[0][1], m[1][1]  );
}

TnMatrix2x2 TnMatrix2x2::Inverse() const
{
	double det = Determinant();
	if ( det <= tgc::Epsilon ) tgf::SevereError( "Singular matrix in TnMatrix2x2::Inverse()" );

	double alpha = 1.0/det;
	return TnMatrix2x2(   m[1][1]*alpha, - m[0][1]*alpha,
			          - m[1][0]*alpha,   m[0][0]*alpha );
}

TnMatrix2x2 operator*( double scalar, const TnMatrix2x2& matrix )
{
	return TnMatrix2x2(   scalar*matrix.m[0][0], scalar*matrix.m[0][1],
			            scalar*matrix.m[1][0], scalar*matrix.m[0][0] );
}

std::ostream& operator<<( std::ostream& os, const TnMatrix2x2& matrix )
{
	for (int i = 0; i < 2; ++i)
	{
		os << "[ ";
		for (int j = 0; j < 2; ++j)
		{
			if( fabs( matrix.m[i][j] ) < tgc::Epsilon ) os << "0";
			else os << matrix.m[i][j];
			if (j != 1) os << ", ";
		}
		os << " ] " << std::endl;
	}
	return os;
}

