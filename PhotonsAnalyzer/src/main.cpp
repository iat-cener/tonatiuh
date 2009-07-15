/*
 * main.cpp
 *
 *  Created on: Jul 15, 2009
 *      Author: amutuberria
 */
#include <QApplication>

#include "MainWindow.h"


//!  Application entry point.
/*!
  PhotonsAnalizer main() function. It starts SoQt and Coin3D. It also initializes the
  application specific Coin3D extension subclasses, and the application loop.
*/

int main( int argc, char ** argv )
{
	QApplication::setColorSpec( QApplication::CustomColor );

    QApplication a( argc, argv );

   	MainWindow mw;
    mw.show();

	return a.exec();
}

