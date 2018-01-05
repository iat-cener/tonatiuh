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

#ifndef MATRIX4X4_H_
#define MATRIX4X4_H_

#include <iostream>

#include "GeometryLibrary.h"
#include "RefCount.h"
#include "Ptr.h"

class GEOMETRY_API Matrix4x4 : public RefCount
{
public:
	Matrix4x4( );
	Matrix4x4( double array[4][4] );
	Matrix4x4( double t00, double t01, double t02, double t03,
	           double t10, double t11, double t12, double t13,
	           double t20, double t21, double t22, double t23,
	           double t30, double t31, double t32, double t33 );
    Matrix4x4( const Matrix4x4& rhs );
	~Matrix4x4( );
	Ptr<Matrix4x4> Transpose( ) const;
	Ptr<Matrix4x4> Inverse( ) const;

	bool operator==( const Matrix4x4& matrix ) const;

	double m[4][4];

};

Ptr<Matrix4x4> Mul( const Ptr<Matrix4x4>& m1, const Ptr<Matrix4x4>& m2 );
std::ostream& operator<<( std::ostream& os, const Matrix4x4& matrix );


#endif /*MATRIX4X4_H_*/



