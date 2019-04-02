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

#include <QIcon>

#include "TransmissivityATMParametersFactory.h"
#include "TransmissivityATMParameters.h"


QString TransmissivityATMParametersFactory::TTransmissivityName() const
{
	return QString( "ATM Parameters Transmissivity" );
}

QIcon TransmissivityATMParametersFactory::TTransmissivityIcon() const
{
	return QIcon(":/icons/TransmissivityATMParameters.png");
}

TransmissivityATMParameters* TransmissivityATMParametersFactory::CreateTTransmissivity( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
	    // Initialize the new node classes
		TransmissivityATMParameters::initClass();
	    firstTime = false;
	}
	return new TransmissivityATMParameters;

}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2( TransmissivityATMParameters, TransmissivityATMParametersFactory )
#endif
