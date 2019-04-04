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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>

#include <Inventor/nodes/SoTransform.h>

#include "RandomDeviate.h"
#include "tgf.h"
#include "Transform.h"




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

SbMatrix tgf::MatrixFromTransform( const Transform& transform )
{
	Ptr<Matrix4x4> transformMatrix = transform.GetMatrix()->Transpose();
	float m00 = float ( transformMatrix->m[0][0] );
	float m01 = float ( transformMatrix->m[0][1] );
	float m02 = float ( transformMatrix->m[0][2] );
	float m03 = float ( transformMatrix->m[0][3] );
	float m10 = float ( transformMatrix->m[1][0] );
	float m11 = float ( transformMatrix->m[1][1] );
	float m12 = float ( transformMatrix->m[1][2] );
	float m13 = float ( transformMatrix->m[1][3] );
	float m20 = float ( transformMatrix->m[2][0] );
	float m21 = float ( transformMatrix->m[2][1] );
	float m22 = float ( transformMatrix->m[2][2] );
	float m23 = float ( transformMatrix->m[2][3] );
	float m30 = float ( transformMatrix->m[3][0] );
	float m31 = float ( transformMatrix->m[3][1] );
	float m32 = float ( transformMatrix->m[3][2] );
	float m33 = float ( transformMatrix->m[3][3] );

	SbVec3f axis1( m00, m10, m20 );
	SbVec3f axis2( m01, m11, m21 );
	//axis2.normalize();

	SbVec3f axis3( m02, m12, m22 );
	//axis3.normalize();

	return SbMatrix( axis1[0], axis2[0], axis3[0], m03,
			axis1[1], axis2[1], axis3[1], m13,
			axis1[2], axis2[2], axis3[2], m23,
			m30, m31, m32, m33 );

}

Transform tgf::TransformFromMatrix( SbMatrix const& matrix )
{
	Transform transform;
	/*if( matrix.det4() < tgc::Epsilon )
		transform = Transform( new Matrix4x4(), new Matrix4x4() );
	else*/
		transform = Transform( matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
							 matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
							 matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
							 matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3] );

	return transform;
}

Transform tgf::TransformFromSoTransform( SoTransform* const & soTransform )
{
	return TransformFromMatrix( MatrixFromSoTransform( soTransform ) );
}

SbMatrix tgf::MatrixFromSoTransform( SoTransform* const & soTransform )
{
	SbMatrix sbMatrix;
	sbMatrix.setTransform( 	soTransform->translation.getValue(),
							soTransform->rotation.getValue(),
						    soTransform->scaleFactor.getValue(),
						    soTransform->scaleOrientation.getValue(),
						    soTransform->center.getValue() );

	return sbMatrix;

}

