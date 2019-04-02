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

#include "TransmissivityMirval.h"


SO_NODE_SOURCE( TransmissivityMirval );


void TransmissivityMirval::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivityMirval, TTransmissivity, "TTransmissivity" );
}

TransmissivityMirval::TransmissivityMirval( )
{
	SO_NODE_CONSTRUCTOR( TransmissivityMirval );
}

TransmissivityMirval::~TransmissivityMirval()
{

}

bool TransmissivityMirval::IsTransmitted( double distance, RandomDeviate& rand ) const
{
	double t;
	if( distance/1000 <= 1.0 )
		t = ( 0.99321 - 0.1176 *( distance / 1000 ) +0.0197 * ( distance / 1000 ) * ( distance / 1000 ));
	else
		t= exp (-0.1106 * distance/1000);

	if( rand.RandomDouble() < t  )	return true;
	return false;
}
