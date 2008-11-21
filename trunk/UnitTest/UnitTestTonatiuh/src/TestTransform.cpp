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

#include "Point3D.h"
#include "TestTransform.h"
#include "Transform.h"
#include "Vector3D.h"

const double Pi = 3.1415926535897932385;

CPPUNIT_TEST_SUITE_REGISTRATION(TestTransform);

void TestTransform::setUp()
{
	m_tran1 = new Transform(1.0,0.0,0.0,0.0,
							0.0,1.0,0.0,0.0,
							0.0,0.0,1.0,0.0,
							0.0,0.0,0.0,1.0 );

	m_tran2 = new Transform(1.1, 1.2, 1.3, 1.4,
							2.1, 2.2, 2.3, 2.4,
							3.1, 3.2, 3.3, 3.4,
							4.1, 4.2, 4.3, 4.4 );
}

void TestTransform::tearDown()
{
	delete m_tran1;
	delete m_tran2;
}

void TestTransform::testEquality()
{
	Transform* ident = new Transform(new Matrix4x4());

	CPPUNIT_ASSERT_EQUAL( (*ident), (*m_tran1) );
}

void TestTransform::testInverse()
{
	Transform* rotz = new Transform(new Matrix4x4(0.0,1.0,0.0,0.0,
									-1.0,0.0,0.0,0.0,
									0.0,0.0,1.0,0.0,
									0.0,0.0,0.0,1.0 ) );
	Transform* invrotz = new Transform(new Matrix4x4(0.0,-1.0,0.0,0.0,
									1.0,0.0,0.0,0.0,
									0.0,0.0,1.0,0.0,
									0.0,0.0,0.0,1.0 ) );
	Transform* result = new Transform(rotz->GetInverse());
	Transform* invinv = new Transform(rotz->GetInverse().GetInverse());

	CPPUNIT_ASSERT_EQUAL( (*invrotz), (*result) );
	CPPUNIT_ASSERT_EQUAL( (*rotz), (*invinv) );
}

void TestTransform::testTranslate()
{
	Transform* trans=new Transform(new Matrix4x4(	1.0,0.0,0.0,3.2,
													0.0,1.0,0.0,1.3,
													0.0,0.0,1.0,4.5,
													0.0,0.0,0.0,1.0 ));
	Vector3D v(3.2,1.3,4.5);
	Transform* result = new Transform(Translate(3.2,1.3,4.5));
	Transform* resultVector= new Transform(Translate(v));

	CPPUNIT_ASSERT_EQUAL( (*trans), (*result) );
	CPPUNIT_ASSERT_EQUAL( (*trans), (*resultVector) );
}

void TestTransform::testRotateX(){
	double ang = 30 * ( Pi / 180.0 ) * 30 ;
	Transform* rotx = new Transform( RotateX( ang) );

	Transform* result = new Transform (new Matrix4x4(	1.0,  0.0,  	  0.0,	       0.0,
														0.0,  cos( ang ), -sin( ang ), 0.0,
														0.0,  sin( ang ), cos( ang ),  0.0,
														0.0,  0.0,  	  0.0,  	   1.0 ));

	CPPUNIT_ASSERT_EQUAL( (*result), (*rotx) );

}

void TestTransform::testRotateY(){

	double ang = 30 *( Pi / 180.0 );
	Transform* roty = new Transform( RotateY( ang ) );

	Transform* result = new Transform( new Matrix4x4(	cos(ang), 0.0, sin(ang), 0.0,
														0.0,      1.0, 0.0,		 0.0,
													   -sin(ang), 0.0, cos(ang), 0.0,
														0.0,  	  0.0, 0.0,		 1.0 ));

	CPPUNIT_ASSERT_EQUAL( (*result), (*roty) );

}

void TestTransform::testRotateZ(){

	double ang = 30 *( Pi / 180.0 );
	Transform* rotz = new Transform( RotateZ( ang ) );

	Transform* result = new Transform( new Matrix4x4(	cos(ang), -sin(ang), 0.0,  0.0,
														sin(ang),  cos(ang), 0.0,  0.0,
														0.0,  	   0.0,		 1.0,  0.0,
														0.0,  	   0.0,		 0.0,  1.0 ) );

	CPPUNIT_ASSERT_EQUAL( (*result), (*rotz) );

}

void TestTransform::testRotate()
{
	Vector3D v( 1.1, 2.2, 3.3);
	Transform* rot = new Transform( Rotate( ( ( Pi/180.0 ) * 30 ), v ) );

	Transform* result = new Transform (new Matrix4x4(	0.875595018,	-0.381752635,  0.295970084, 0.0,
														0.420031091,	 0.90430386,  -0.076212937,	0.0,
														-0.2385524,	     0.191048305,  0.95215193,	0.0,
														0.0,  		     0.0,  		   0.0, 		1.0 ));
	Ptr<Matrix4x4> result_mdir=	result->GetMatrix();
	Ptr<Matrix4x4> rot_mdir=	rot->GetMatrix();
	for (int i=1; i<4; i++)
		for (int j=1; j<4; j++)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mdir->m[i][j], rot_mdir->m[i][j], 0.000000001);

}

void TestTransform::testLookAt()
{
	CPPUNIT_ASSERT( false );
}

void TestTransform::testCoordenateSystem()
{
	//We supposse that is right-hand coordenate system
	Vector3D axis_x(1.0, 0.0, 0.0);
	Vector3D axis_y(0.0, 1.0, 0.0);

	Transform rotz = RotateZ( ( Pi/180.0 ) * 90 );

	Vector3D transaxis_x=rotz(axis_x);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Is not Right-hand coordinate system", axis_y, transaxis_x);

}
void TestTransform::testMulOrder()
{
	double ang = 30 * ( Pi/180.0 );

	//One rotation
	Point3D p1(0,0,1);

	Transform* rotX =new Transform (RotateX( ang ));

	Point3D* result=new Point3D((* rotX )(p1));

	Point3D* answer1= new Point3D (0,-sin( ang ), cos( ang ));


	CPPUNIT_ASSERT_EQUAL_MESSAGE("Rotations error 1", (*result), (*answer1) );


	//Two rotation
	Point3D p2(0,0,1);

	rotX =new Transform (RotateX( ang ));
	Transform* rotZ =new Transform (RotateZ( ang ));

	Transform* rotZX = new Transform ((*rotZ) * (*rotX));

	result=new Point3D((*rotZX)(p2));

	Point3D* answer2= new Point3D (sin(ang)*sin(ang),-cos( ang )*sin(ang), cos( ang ));


	CPPUNIT_ASSERT_EQUAL_MESSAGE("Rotations error 2", (*result), (*answer2) );


	//Two rotation
	Point3D p3(0,0,1);

	rotX =new Transform (RotateX( ang ));
	rotZ =new Transform (RotateZ( ang ));

	Transform*  rotY =new Transform (RotateY( ang ));

	Transform* rotZXY = new Transform ((*rotY) * (*rotZ) * (*rotX));

	result=new Point3D((*rotZXY)(p3));

	Point3D* answer3= new Point3D ( sin(ang)*sin(ang)*cos(ang)+cos( ang)*sin(ang),
									-cos( ang )*sin(ang),
									-sin(ang)*sin(ang)*sin (ang) +cos(ang)*cos( ang ));


	//CPPUNIT_ASSERT_DOUBLES_EQUAL("Rotations error 3", (*result), (*answer3) );
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).x, (*answer3).x, 0.000000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).y, (*answer3).y, 0.000000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).z, (*answer3).z, 0.000000001);

	//Rotate and translate
	Point3D p4(0,0,1);

	rotX =new Transform (RotateX( ang ));
	Transform* translate =new Transform (Translate( 1, 0, -1 ));

	Transform* transRotX = new Transform ( (*translate) * (*rotX));

	result=new Point3D((*transRotX)(p4));

	Point3D* answer4= new Point3D (1, - sin(ang), cos( ang ) - 1);


	//CPPUNIT_ASSERT_EQUAL_MESSAGE("Rotations error 4", (*result), (*answer4) );
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).x, (*answer4).x, 0.000000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).y, (*answer4).y, 0.000000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((*result).z, (*answer4).z, 0.000000001);
}
