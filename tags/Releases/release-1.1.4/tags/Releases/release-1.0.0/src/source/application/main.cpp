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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QApplication>
#include <QDir>

#include <Inventor/Qt/SoQt.h>

#include "MainWindow.h"
#include "TDefaultMaterial.h"
#include "TDefaultSunShape.h"
#include "TDefaultTracker.h"
#include "TCube.h"
#include "TLightKit.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"
#include "TSquare.h"

/*!
  \mainpage
The Tonatiuh project aims to create an open source, cutting-edge, accurate, and easy to use Monte Carlo ray tracer for the optical simulation of solar concentrating systems. It intends to advance the state-of-the-art of the simulation tools available for the design and analysis of solar concentrating systems, and to make those tools freely available to anyone interested in using and improving them. Some of the most relevant design goals of Tonatiuh are:
<ol>
  <li>To develop a robust theoretical foundation that will facilitate the optical simulation of almost any type of solar concentrating systems.</li>
  <li>To exhibit a clean and flexible software architecture, that will allow the user to adapt, expand, increase, and modify its functionalities with ease.</li>
  <li>To achieve operating system independence at source level, and run on all major platforms with none, or minor, modifications to its source code.</li>
  <li>To provide the users with an advanced and easy-of-use Graphic User Interface (GUI).</li>
  </ol>

 <b>IMPORTANT NOTE: the online documentation for the Tonatiuh is still a work-in-progress.</b>
*/


//!  Application entry point.
/*!
  Tonatiuh's main() function. It starts SoQt and Coin3D. It also initializes the
  application specific Coin3D extension subclasses, and the application loop.
*/

int main( int argc, char ** argv )
{
	QApplication::setColorSpec( QApplication::CustomColor );

    QApplication a( argc, argv );


	QApplication::addLibraryPath( QApplication::applicationDirPath()
	        + QDir::separator() + "marble" );

	SoQt::init( (QWidget *) NULL );


	TMaterial::initClass();
	TDefaultMaterial::initClass();
	TSeparatorKit::initClass();
	TShape::initClass();
	TCube::initClass();
	TShapeKit::initClass();
	TSquare::initClass();
	TLightKit::initClass();
	TSunShape::initClass();
	TDefaultSunShape::initClass();
	TTracker::initClass();
	TDefaultTracker::initClass();


   	MainWindow mw;
    mw.show();

	return a.exec();
}
