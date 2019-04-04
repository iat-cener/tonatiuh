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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>

#include "TransmissivityVantHull.h"

SO_NODE_SOURCE( TransmissivityVantHull );


void TransmissivityVantHull::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivityVantHull, TTransmissivity, "TTransmissivity" );
}

TransmissivityVantHull::TransmissivityVantHull( )
{
	SO_NODE_CONSTRUCTOR( TransmissivityVantHull );

	SO_NODE_ADD_FIELD( Visibility, ( 23000 ) );
	SO_NODE_ADD_FIELD( Site_Elevation, ( 0 ) );
	SO_NODE_ADD_FIELD( Vapor_Density, ( 5.9 ) );
	SO_NODE_ADD_FIELD( Tower_Heigth, ( 100 ) );

}

TransmissivityVantHull::~TransmissivityVantHull()
{

}

bool TransmissivityVantHull::IsTransmitted( double distance, RandomDeviate& rand ) const
{

	if( distance == HUGE_VAL )	return false;

	double R = distance/ 1000;
	double beta = 3.912 / ( Visibility.getValue() / 1000 );
	double h = Site_Elevation.getValue()/1000;
	double ro =  Vapor_Density.getValue();

	double A0 = 0.0112 * h + 0.0822;
	double S0 = 0.00101 * ro + 0.0507;
	double C0 = 0.0105 * ro + 0.724;

	double A = A0 * log( ( beta + 0.0003 * ro ) / 0.00455 );


	double S = 1 - ( S0 * pow( beta + 0.0091, -0.5 ) );
	double C = C0 * pow( beta - 0.0037, S );

	double e = C * exp( - A * ( Tower_Heigth.getValue() / 1000 ) );
	if( pow( R, S ) == HUGE_VAL )	return true;
	double t = exp( - e * pow( R, S ) );


	if( rand.RandomDouble() <  t  )	return true;

	return false;

}
