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

#ifndef TRANSMISSIVITYMIRVAL_H_
#define TRANSMISSIVITYMIRVAL_H_

#include "trt.h"
#include "TTransmissivity.h"


class TransmissivityMirval : public TTransmissivity
{
	SO_NODE_HEADER( TransmissivityMirval );

public:
    static void initClass();
    TransmissivityMirval();

	bool IsTransmitted( double distance, RandomDeviate& rand ) const;


protected:
    virtual ~TransmissivityMirval();

};

#endif /* TRANSMISSIVITYMIRVAL_H_ */
