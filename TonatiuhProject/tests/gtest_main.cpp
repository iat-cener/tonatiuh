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

#include <iostream>

#include <QApplication>

#include <Inventor/Qt/SoQt.h>

#include <gtest/gtest.h>

#include "TDefaultMaterial.h"
#include "TDefaultSunShape.h"
#include "TDefaultTracker.h"
#include "TCube.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"
#include "TSquare.h"
#include "TSceneKit.h"
#include "TSceneTracker.h"
#include "TTrackerForAiming.h"
#include "TTransmissivity.h"

int main(int argc, char **argv )
{

    QApplication a( argc, argv );
    std::cout << "Running main() from gtest_main.cc\n" <<std::endl;

	SoQt::init( (QWidget *) NULL );

	TSceneKit::initClass();
	TMaterial::initClass();
	TDefaultMaterial::initClass();
	TSeparatorKit::initClass();
	TShape::initClass();
	TCube::initClass();
	TLightShape::initClass();
	TShapeKit::initClass();
	TSquare::initClass();
	TLightKit::initClass();
	TSunShape::initClass();
	TDefaultSunShape::initClass();
	TTracker::initClass();
	TDefaultTracker::initClass();
	TSceneTracker::initClass();
	TTrackerForAiming::initClass();
	TTransmissivity::initClass();


    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
