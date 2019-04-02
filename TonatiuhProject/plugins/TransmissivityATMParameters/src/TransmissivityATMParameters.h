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

#ifndef TRANSMISSIVITYFATMPARAMETERS_H_
#define TRANSMISSIVITYFATMPARAMETERS_H_

#include "trt.h"
#include "TTransmissivity.h"


class TransmissivityATMParameters : public TTransmissivity
{
	SO_NODE_HEADER( TransmissivityATMParameters );

public:
    static void initClass();
    TransmissivityATMParameters();

	bool IsTransmitted( double distance, RandomDeviate& rand ) const;

	//trt::TONATIUH_BOOL ClearDay;
	trt::TONATIUH_REAL atm1;
	trt::TONATIUH_REAL atm2;
	trt::TONATIUH_REAL atm3;
	trt::TONATIUH_REAL atm4;

protected:
    virtual ~TransmissivityATMParameters();

};

#endif /* TRANSMISSIVITYFATMPARAMETERS_H_ */
