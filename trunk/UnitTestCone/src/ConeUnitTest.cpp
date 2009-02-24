#include <QApplication>

#include <Inventor/Qt/SoQt.h>

#include <qxcppunit/testrunner.h>
#include <qxcppunit/testrunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "ShapeCone.h"
#include "TShape.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SoQt::init( (QWidget *) NULL );

	TShape::initClass();
	ShapeCone::initClass();

	QxCppUnit::TestRunner runner;

	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run();

	return 0;
}


