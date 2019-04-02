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

#include "TDefaultTransmissivity.h"

SO_NODE_SOURCE( TDefaultTransmissivity );


void TDefaultTransmissivity::initClass()
{
	SO_NODE_INIT_CLASS( TDefaultTransmissivity, TTransmissivity, "TTransmissivity" );
}

TDefaultTransmissivity::TDefaultTransmissivity( )
{
	SO_NODE_CONSTRUCTOR( TDefaultTransmissivity );
	SO_NODE_ADD_FIELD( constant, ( 0.001 ) );
}

TDefaultTransmissivity::~TDefaultTransmissivity()
{

}

bool TDefaultTransmissivity::IsTransmitted( double /*distance*/, RandomDeviate& /*rand*/ ) const
{
	return true;
}
