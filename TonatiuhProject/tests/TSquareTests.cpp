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

