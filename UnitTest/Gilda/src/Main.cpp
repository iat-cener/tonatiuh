/*
 * Main.cpp
 *
 *  Created on: Feb 6, 2009
 *      Author: ENGINEERING
 */


#include <Inventor/Qt/SoQt.h>

#include <qxcppunit/testrunner.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include "Vector3D.h"
#include "Point3D.h"
#include "TestPoint3D.h"
#include "TestVector3D.h"


#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SoQt::init( (QWidget *) NULL );

	QxCppUnit::TestRunner runner;
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run();

	return 0;
}
