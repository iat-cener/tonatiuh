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

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>
#include <QString>
#include <iostream>

#include "tgf.h"
#include "TestTGF.h"
#include "Matrix4x4.h"
#include "Transform.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( TestTGF );

void TestTGF::setUp()
{

}

void TestTGF::tearDown()
{

}

void TestTGF::test_isOdd()
{
	int test = 1;
	bool answer;

	answer = tgf::IsOdd(test);
	CPPUNIT_ASSERT_MESSAGE("Expected true, got false", answer);
	answer = tgf::IsOdd(test+1);

	CPPUNIT_ASSERT_EQUAL( answer, false );
}

void TestTGF::test_Quadratic()
{
	double a = 1;
	double b = -3;
	double c = -4;
	double val1,val2;

	double answer1 = -1;
	double answer2 = 4;

	tgf::Quadratic(a,b,c,&val1,&val2);

	CPPUNIT_ASSERT_EQUAL( val1, answer1 );
	CPPUNIT_ASSERT_EQUAL( val2, answer2 );

}

void TestTGF::test_MatrixFromTransform()
{
	Transform tran(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	SbMatrix start(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	SbMatrix expected = start.transpose();
	SbMatrix result;

	result = tgf::MatrixFromTransform(tran);
	SbBool answer = expected.equals(result.getValue(),.01f);

	CPPUNIT_ASSERT_MESSAGE("Matrix from transform failed", answer);
}
