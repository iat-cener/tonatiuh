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

#ifndef TESTSQUARE_H_
#define TESTSQUARE_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/SbLinear.h>
#include "TShape.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "BBox.h"
#include "Point3D.h"
#include "TSquare.h"

class BBox;
struct Point3D;
struct NormalVector;

class TestSquare: public CPPUNIT_NS::TestFixture, public TSquare
{
	CPPUNIT_TEST_SUITE(TestSquare);
	CPPUNIT_TEST(test_computeBBox);
	CPPUNIT_TEST(test_get_Normal);
	CPPUNIT_TEST(test_get_Icon);
	CPPUNIT_TEST(test_intersect);
	CPPUNIT_TEST(test_intersectP);
	CPPUNIT_TEST(test_GetPoint3D);
	CPPUNIT_TEST(test_Sample);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void test_init_Class();
	void test_computeBBox();
	void test_get_Normal();
	void test_get_Icon();
	void test_intersect();
	void test_intersectP();
	void test_GetPoint3D();
	void test_Sample();

private:
	SbBox3f m_bbox;
	TSquare* m_square;
};

#endif /* TESTSQUARE_H_ */
