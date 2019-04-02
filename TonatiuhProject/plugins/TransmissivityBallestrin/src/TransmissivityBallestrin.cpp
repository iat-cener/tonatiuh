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

#include "TransmissivityBallestrin.h"


SO_NODE_SOURCE( TransmissivityBallestrin );


void TransmissivityBallestrin::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivityBallestrin, TTransmissivity, "TTransmissivity" );
}

TransmissivityBallestrin::TransmissivityBallestrin( )
{
	SO_NODE_CONSTRUCTOR( TransmissivityBallestrin );
	SO_NODE_ADD_FIELD( ClearDay, ( TRUE ) );
}

TransmissivityBallestrin::~TransmissivityBallestrin()
{

}

bool TransmissivityBallestrin::IsTransmitted( double distance, RandomDeviate& rand ) const
{
	double t;
	if( ClearDay.getValue() )
	{
		t = ( 0.9970456  + -0.1522128 *( distance / 1000 ) + 0.018598 * ( distance / 1000 ) * ( distance / 1000 )
				-0.0015182 * ( distance / 1000 ) * ( distance / 1000 ) * ( distance / 1000 ) );
	}
	else
	{
		t = ( 0.9922059 -0.5549083 *( distance / 1000 ) + 0.147887 * ( distance / 1000 ) * ( distance / 1000 )
				-0.0153718 * ( distance / 1000 ) * ( distance / 1000 ) * ( distance / 1000 ) );
	}

	if( rand.RandomDouble() < t  )	return true;

	return false;
}
