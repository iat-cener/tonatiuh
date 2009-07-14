/*
 * main.cpp
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 */
#include <QApplication>

#include <Inventor/Qt/SoQt.h>

#include "MainWindow.h"


//!  Application entry point.
/*!
  Ehecatl main() function. It starts SoQt and Coin3D. It also initializes the
  application specific Coin3D extension subclasses, and the application loop.
*/

int main( int argc, char ** argv )
{
	QApplication::setColorSpec( QApplication::CustomColor );

    QApplication a( argc, argv );
	SoQt::init( (QWidget *) NULL );


   	MainWindow mw;
    mw.show();

	return a.exec();
}

