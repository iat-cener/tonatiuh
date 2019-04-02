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


#include "TLightShape.h"
#include "Point3D.h"

#include <gtest/gtest.h>
#include "TestsAuxiliaryFunctions.h"
#include <time.h>

const double maximumCoordinate = 5000000.0;
const unsigned long int maximumNumberOfTests = 60000;

TEST(TLightShapeTests , DefaultConstructor){

	TLightShape* shape= new TLightShape;

	EXPECT_DOUBLE_EQ(double(shape->xMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->xMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMin.getValue()),-0.5);
	EXPECT_DOUBLE_EQ(double(shape->zMax.getValue()),0.5);
	EXPECT_DOUBLE_EQ(double(shape->delta.getValue()),100.0);
}
