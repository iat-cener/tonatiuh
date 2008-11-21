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

#include <cmath>
#include <cfloat>

#include "Matrix4x4.h"
#include "TestMatrix4x4.h"

const double Epsilon = DBL_EPSILON;

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TestMatrix4x4);

void TestMatrix4x4::setUp()
{
	m_identity = new Matrix4x4(	1.0,0.0,0.0,0.0,
								0.0,1.0,0.0,0.0,
								0.0,0.0,1.0,0.0,
								0.0,0.0,0.0,1.0 );

	m_mat = new Matrix4x4(	1.1, 1.2, 1.3, 1.4,
							2.1, 2.2, 2.3, 2.4,
							3.1, 3.2, 3.3, 3.4,
							4.1, 4.2, 4.3, 4.4 );
}

void TestMatrix4x4::tearDown()
{
	delete m_identity;
	delete m_mat;
}

void TestMatrix4x4::TestEquality()
{
	Matrix4x4* newmat = new Matrix4x4();
	CPPUNIT_ASSERT_EQUAL((*newmat), (*m_identity));
}

void TestMatrix4x4::TestTranspose()
{
	Matrix4x4* transpose = new Matrix4x4( 	1.1, 2.1, 3.1, 4.1,
											1.2, 2.2, 3.2, 4.2,
											1.3, 2.3, 3.3, 4.3,
											1.4, 2.4, 3.4, 4.4 );

	Matrix4x4* tranresult = new Matrix4x4( (*m_mat->Transpose() ) );
	Matrix4x4* trantran = new Matrix4x4( (*(m_mat->Transpose())->Transpose() ) );

	CPPUNIT_ASSERT_EQUAL((*transpose),(*tranresult));
	CPPUNIT_ASSERT_EQUAL((*m_mat),(*trantran));
}

void TestMatrix4x4:: TestInverse()
{
	Matrix4x4 *rotz = new Matrix4x4(0.0,1.0,0.0,0.0,
									-1.0,0.0,0.0,0.0,
									0.0,0.0,1.0,0.0,
									0.0,0.0,0.0,1.0 );

	Matrix4x4 *invrotz = new Matrix4x4(	0.0,-1.0,0.0,0.0,
										1.0,0.0,0.0,0.0,
										0.0,0.0,1.0,0.0,
										0.0,0.0,0.0,1.0 );

	Matrix4x4 *invresult = new Matrix4x4((*rotz->Inverse()));
	Matrix4x4 *invinv = new Matrix4x4( (*(rotz->Inverse())->Inverse() ) );

	CPPUNIT_ASSERT_EQUAL( (*invrotz), (*invresult) );
	CPPUNIT_ASSERT_EQUAL( (*rotz), (*invinv) );
}
