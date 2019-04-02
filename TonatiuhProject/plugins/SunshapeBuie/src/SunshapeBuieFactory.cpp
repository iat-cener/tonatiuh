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
#include "SunshapeBuieFactory.h"


QString SunshapeBuieFactory::TSunShapeName() const
{
	return QString( "Buie_Sunshape" );
}

QIcon SunshapeBuieFactory::TSunShapeIcon() const
{
	return QIcon( ":/icons/SunshapeBuie.png" );
}

SunshapeBuie* SunshapeBuieFactory::CreateTSunShape( ) const
{
	static bool firstTimeSunShape = true;
	if ( firstTimeSunShape )
	{
	    SunshapeBuie::initClass();
	    firstTimeSunShape = false;
	}

	SunshapeBuie* pill = new SunshapeBuie;
	return pill;
}
#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(SunshapeBuie, SunshapeBuieFactory)
#endif

