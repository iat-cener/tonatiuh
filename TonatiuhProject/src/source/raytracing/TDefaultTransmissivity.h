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

#ifndef TDEFAULTTRANSMISSIVITY_H_
#define TDEFAULTTRANSMISSIVITY_H_

#include "trt.h"
#include "TTransmissivity.h"


class TDefaultTransmissivity : public TTransmissivity
{
	SO_NODE_HEADER( TransmissivityDefualt );

public:
    static void initClass();
    TDefaultTransmissivity();

	bool IsTransmitted( double distance, RandomDeviate& rand ) const;

	trt::TONATIUH_REAL constant;

protected:
    virtual ~TDefaultTransmissivity();

};

#endif /* TDEFAULTTRANSMISSIVITY_H_ */
