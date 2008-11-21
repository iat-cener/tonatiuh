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

#include "NormalVector.h"
#include "TestNormalVector.h"
#include "Vector3D.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestNormalVector);

void TestNormalVector::setUp()
{
	m_p1 = new NormalVector(5.0,0.0,2.0);
	m_p2 = new NormalVector(2.0,1.0,3.0);
	m_p3 = new NormalVector();
}

void TestNormalVector::tearDown()
{
	delete 	m_p1;
	delete	m_p2;
	delete	m_p3;
}

void TestNormalVector::testLengthSquared()
{
	NormalVector temp(1.2,3.1,4.5);
	double answer = 31.3;
	double result = temp.LengthSquared();

	CPPUNIT_ASSERT_EQUAL(result, answer);
}

void TestNormalVector::testLength()
{
	NormalVector temp(1.2,3.1,4.5);
	double answer = sqrt(31.3);
	double result = temp.Length();

	CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON );
}

void TestNormalVector::testDotProduct()
{
	double answer = DotProduct((*m_p1), (*m_p2));
	double test = 16.0;

	CPPUNIT_ASSERT_EQUAL(answer,test);
}

void TestNormalVector::testNormalize()
{
	NormalVector t1 (2.0,2.0,0.0);
	NormalVector answer (0.7071067811865475,0.7071067811865475,0.0);
	NormalVector result = Normalize (t1);

	CPPUNIT_ASSERT_EQUAL(answer, result);
}

void TestNormalVector::testAbsDotProduct()
{
	NormalVector n1( -3.1 , 2.5 , -5.2 );
	NormalVector n2( 1.4 , -3.8 , 1.2 );
	double answer = 20.08;
	double result = AbsDotProduct(n1,n2);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);
}

void TestNormalVector::testScalarMultiplication()
{
	NormalVector result = 5*(*m_p1);
	NormalVector answer(25.0, 0.0, 10.0);

	CPPUNIT_ASSERT_EQUAL( result, answer );
}

