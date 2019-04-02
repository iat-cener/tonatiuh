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

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "gc.h"
#include "gf.h"


void gf::SevereError( std::string errorMessage )
{
	std::cerr << errorMessage << std::endl;
	exit(-1);
}

void gf::Warning( std::string warningMessage )
{
	std::cerr << warningMessage << std::endl;
}

bool gf::IsOdd( int number )
{
	bool answer = number & 1;
	return answer;
}

bool gf::Quadratic( double A, double B, double C, double *t0, double *t1)
{
	// Find discriminant
	double discrim = B*B - 4.0*A*C;
	if (discrim < 0.) return false;

	// Compute quadratic root values
	double q = -0.5;
	if ( B < 0 ) q *= B - sqrt( discrim );
	else q *= B + sqrt( discrim ) ;
	*t0 = q / A;
	*t1 = C / q;
	if(*t0 > *t1) std::swap( *t0, *t1 );
	return true;
}
