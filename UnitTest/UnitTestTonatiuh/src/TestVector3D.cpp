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
#include "TestVector3D.h"
#include "Vector3D.h"


CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3D );

void TestVector3D::setUp()
{
	m_p1=new Vector3D( 5.0, 0.0, 2.0);
	m_p2=new Vector3D( 2.0, 1.0, 3.0);
	m_p3=new Vector3D();
}

void TestVector3D::tearDown()
{
	delete 	m_p1;
	delete	m_p2;
	delete	m_p3;
}

void TestVector3D::testEquality()
{
	Vector3D test( 5.0, 0.0, 2.0);
	CPPUNIT_ASSERT( test == (*m_p1) );
	CPPUNIT_ASSERT( !(test == (*m_p2) ) );
}

void TestVector3D::testInequality()
{
	Vector3D test(5.0,0.0,2.0);
	CPPUNIT_ASSERT( !(test != (*m_p1) ) );
	CPPUNIT_ASSERT( test != (*m_p2) );
}

void TestVector3D::testAddVector()
{
	Vector3D result;
	result = (*m_p1)+(*m_p2);
	Vector3D answer(7.0,1.0,5.0);
	CPPUNIT_ASSERT_EQUAL(answer, result);
}

void TestVector3D::testDotProduct()
{
	double result = DotProduct((*m_p1), (*m_p2));
	double answer = 16.0;
	CPPUNIT_ASSERT_EQUAL(result,answer);
}

void TestVector3D::testCrossProduct()
{
    Vector3D h3( 1.5 , 3.2 , 3.1 );
    Vector3D h4( -1 , 1.3 , 9.1 );
    Vector3D answer( 25.09, -16.75, 5.15 );
    Vector3D result = CrossProduct(h3,h4);
    CPPUNIT_ASSERT( result == answer );
}

void TestVector3D::testAbsDotProduct()
{
	Vector3D v1( -3.1 , 2.5 , -5.2 );
	Vector3D v2( 1.4 , -3.8 , 1.2 );
	double answer = 20.08;
	double result = AbsDotProduct(v1,v2);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);
}

void TestVector3D::testDotProductWithNormal()
{
	Vector3D vect(3.1,-1.0,2.4);
	NormalVector norm(4.3,1.9,2.7);
	double answer = 17.91;
	double result = DotProduct(norm,vect);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON);
}

void TestVector3D::testLength()
{
	Vector3D temp(1.2,3.1,4.5);
	double answer = sqrt(31.3);
	double result = temp.Length();

	CPPUNIT_ASSERT_DOUBLES_EQUAL( result, answer, DBL_EPSILON );
}

void TestVector3D::testLengthSquared()
{
	Vector3D temp(1.2,3.1,4.5);
	double answer = 31.3;
	double result = temp.LengthSquared();

	CPPUNIT_ASSERT_EQUAL(result, answer);
}

void TestVector3D::testCrossProductWithNormal()
{
    NormalVector norm( 3.1, 2.5, 5.2 );

    //Cross poduct n x p1
    Vector3D answer1( 5.0, 19.8, -12.5);
    Vector3D result1 = CrossProduct( norm, *m_p1 );

    //Cross poduct p1 x n
    Vector3D answer2( -5.0, -19.8, 12.5);
    Vector3D result2 = CrossProduct( *m_p1, norm  );

    CPPUNIT_ASSERT_EQUAL(result2, answer2);

}

void TestVector3D::testNormalize()
{
    Vector3D t1 (2.0,2.0,0.0);
    Vector3D answer (0.7071067811865475,0.7071067811865475,0.0);
    Vector3D result = Normalize (t1);

	CPPUNIT_ASSERT_EQUAL(answer, result);
}

void TestVector3D::testSameHemisphere()
{
	Vector3D p4(0.0,0.0,0.0);
	bool test = true;
	if ( CrossProduct(( *m_p1),(*m_p2)) == 0)
		test=false;
    CPPUNIT_ASSERT_MESSAGE("The vector is in the same hemisphere",test);
}
