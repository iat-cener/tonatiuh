// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>

#include <QApplication>

#include <Inventor/Qt/SoQt.h>

#include <gtest/gtest.h>

#include "TAnalyzerKit.h"
#include "TAnalyzerLevel.h"
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
	TAnalyzerKit::initClass();
	TAnalyzerResultKit::initClass();
	TAnalyzerParameter::initClass();
	TAnalyzerResult::initClass();
	TAnalyzerLevel::initClass();
	TTransmissivity::initClass();


    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
