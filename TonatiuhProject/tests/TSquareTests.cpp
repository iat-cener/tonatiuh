/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#include "TSquare.h"
#include "BBox.h"
#include "NormalVector.h"

#include <gtest/gtest.h>

TEST(TShapeTests, DefaultConstructor){

	TSquare* square = new TSquare;

	EXPECT_DOUBLE_EQ(square->m_sideLength.getValue(),10.0);
}

TEST(TShapeTests, TShapeGetArea){

	double value=7.2351;
	TSquare* square=new TSquare;

	EXPECT_DOUBLE_EQ(square->GetArea(),100.0);

	square->m_sideLength.setValue(value);

	EXPECT_DOUBLE_EQ(square->GetArea(),value*value);

}

TEST(TShapeTests, TShapeGetBBox){

	double value=7.2351;
	TSquare* square=new TSquare;

	square->m_sideLength.setValue(value);

	EXPECT_DOUBLE_EQ(square->GetBBox().pMin.x,-value/2);
	EXPECT_DOUBLE_EQ(square->GetBBox().pMin.y,0.0);
	EXPECT_DOUBLE_EQ(square->GetBBox().pMin.z,-value/2);
	EXPECT_DOUBLE_EQ(square->GetBBox().pMax.x,value/2);
	EXPECT_DOUBLE_EQ(square->GetBBox().pMax.y,0.0);
	EXPECT_DOUBLE_EQ(square->GetBBox().pMax.z,value/2);



}

TEST(TShapeTests, TShapeSample){

	double value=7.2351;
	TSquare* square=new TSquare;

	square->m_sideLength.setValue(value);

	double x1=double(12/150);
	double z1=double(1450/5231);

	EXPECT_DOUBLE_EQ(square->Sample(x1,z1).x,(x1*value)-(value/2));
	EXPECT_DOUBLE_EQ(square->Sample(x1,z1).y,0);
	EXPECT_DOUBLE_EQ(square->Sample(x1,z1).z,(z1*value)-(value/2));

}

