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

#include "TransmissivityATMParameters.h"


SO_NODE_SOURCE( TransmissivityATMParameters );


void TransmissivityATMParameters::initClass()
{
	SO_NODE_INIT_CLASS( TransmissivityATMParameters, TTransmissivity, "TTransmissivity" );
}

TransmissivityATMParameters::TransmissivityATMParameters( )
{
	SO_NODE_CONSTRUCTOR( TransmissivityATMParameters );
	SO_NODE_ADD_FIELD( atm1, ( 0.29544 ) );
	SO_NODE_ADD_FIELD( atm2, ( 15.22128 ) );
	SO_NODE_ADD_FIELD( atm3, ( -1.8598 ) );
	SO_NODE_ADD_FIELD( atm4, ( 0.15182 ) );
}

TransmissivityATMParameters::~TransmissivityATMParameters()
{

}

bool TransmissivityATMParameters::IsTransmitted( double distance, RandomDeviate& rand ) const
{


	double dKM = ( distance / 1000 );

	double attenuation = atm1.getValue() + atm2.getValue() * dKM + atm3.getValue()* dKM * dKM + atm4.getValue() * dKM * dKM * dKM;

	double t = 1 - ( attenuation / 100 );

	if( rand.RandomDouble() < t  )	return ( true );

	return ( false );
}
