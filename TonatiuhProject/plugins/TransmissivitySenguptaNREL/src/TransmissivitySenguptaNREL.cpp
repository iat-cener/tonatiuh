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

#include "TransmissivitySenguptaNREL.h"
#include <cmath>

SO_NODE_SOURCE( TransmissivitySenguptaNREL );


void TransmissivitySenguptaNREL::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivitySenguptaNREL, TTransmissivity, "TTransmissivity" );
}

TransmissivitySenguptaNREL::TransmissivitySenguptaNREL( )
{
	SO_NODE_CONSTRUCTOR( TransmissivitySenguptaNREL );
	SO_NODE_ADD_FIELD( beta, ( 0.155996 ) );
}

TransmissivitySenguptaNREL::~TransmissivitySenguptaNREL()
{

}

bool TransmissivitySenguptaNREL::IsTransmitted( double distance, RandomDeviate& rand ) const
{
	double t = exp( -( 0.2299* beta.getValue() + 0.002674 )* distance /250 );
	if( rand.RandomDouble() < t  )	return true;
	return false;
}
