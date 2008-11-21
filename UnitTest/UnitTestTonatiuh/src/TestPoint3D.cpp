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

#include "Point3D.h"

#include "TestPoint3D.h"
#include "Vector3D.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPoint3D);


void TestPoint3D::setUp()
{
	m_p1 = new Point3D( 1.0, 2.0, 3.0 );
	m_p2 = new Point3D( 2.0, 5.0, 4.0 );
	m_p3 = new Point3D( 2-1, 2.0, 1.0+2.0 );
}

void TestPoint3D::tearDown()
{
	delete 	m_p1;
	delete	m_p2;
	delete	m_p3;
}


void TestPoint3D::testEquality()
{
	CPPUNIT_ASSERT_MESSAGE("Two points aren't equal",( m_p1->x == m_p3->x ) && ( m_p1->y == m_p3->y ) &&( m_p1->z == m_p3->z ) );
}

void TestPoint3D::testAddPoints()
{
	Point3D  result;
	result = (*m_p1)+(*m_p2);
	Point3D answer(3.0,7.0,7.0);

	CPPUNIT_ASSERT_EQUAL(answer, result);
}

void TestPoint3D::testAddVector()
{
	Vector3D v(5.0,0.0,2.0);
	Point3D result1,result2;
	result1 = (*m_p1)+v;
	result2 = (*m_p2)+v;
	Point3D answer1(6.0,2.0,5.0);
	Point3D answer2(7.0,5.0,6.0);

	CPPUNIT_ASSERT_EQUAL(answer1, result1);
	CPPUNIT_ASSERT_EQUAL(answer2, result2);
}

void TestPoint3D::testProduct()
{
	double scalar = 16;
	Point3D result1,result2;

	result1 = (*m_p1)*scalar;
	result2 = (*m_p2)*scalar;

	Point3D answer1(16.0,32.0,48.0);
	Point3D answer2(32.0,80.0,64.0);

	CPPUNIT_ASSERT_EQUAL(answer1, result1);
	CPPUNIT_ASSERT_EQUAL(answer2, result2);

	result1 = scalar*(*m_p1);
	result2 = scalar* (*m_p2);

	CPPUNIT_ASSERT_EQUAL(answer1, result1);
	CPPUNIT_ASSERT_EQUAL(answer2, result2);

}

void TestPoint3D::testDistance()
{
	double answer=3.31662479;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(Distance((*m_p1),(*m_p2)),answer,0.000000001);
}
void TestPoint3D::testDistanceSquared()
{
	double answer=11.0;

	CPPUNIT_ASSERT_DOUBLES_EQUAL(DistanceSquared((*m_p1),(*m_p2)),answer,0.000000001);
}
