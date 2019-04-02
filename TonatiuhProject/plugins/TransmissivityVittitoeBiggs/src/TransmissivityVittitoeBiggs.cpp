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

#include "TransmissivityVittitoeBiggs.h"


SO_NODE_SOURCE( TransmissivityVittitoeBiggs );


void TransmissivityVittitoeBiggs::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivityVittitoeBiggs, TTransmissivity, "TTransmissivity" );
}

TransmissivityVittitoeBiggs::TransmissivityVittitoeBiggs( )
{
	SO_NODE_CONSTRUCTOR( TransmissivityVittitoeBiggs );
	SO_NODE_ADD_FIELD( ClearDay , (TRUE));
}

TransmissivityVittitoeBiggs::~TransmissivityVittitoeBiggs()
{

}

bool TransmissivityVittitoeBiggs::IsTransmitted( double distance, RandomDeviate& rand ) const
{
	double t;
    if( ClearDay.getValue() )
    	t = ( 0.99326 - 0.1046 *( distance / 1000 ) + 0.017 * ( distance / 1000 ) * ( distance / 1000 )
			- 0.002845 * ( distance / 1000 ) * ( distance / 1000 ) * ( distance / 1000 ) );
	else
		t = ( 0.98707 - 0.2748 *( distance / 1000 ) + 0.03394 * ( distance / 1000 ) * ( distance / 1000 ) );

    if( rand.RandomDouble() < t )	return true;
    return false;
}
