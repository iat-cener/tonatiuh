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

#ifndef TRACKERHELIOSTATFACTORY_H_
#define TRACKERHELIOSTATFACTORY_H_

#include "TrackerHeliostat.h"
#include "TTrackerFactory.h"

class TrackerHeliostatFactory : public QObject, public TTrackerFactory
{
    Q_OBJECT
    Q_INTERFACES(TTrackerFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TTrackerFactory")
#endif
    
public:
   	QString TTrackerName() const;
   	QIcon TTrackerIcon() const;
   	TrackerHeliostat* CreateTTracker( ) const;
};

#endif /*TRACKERHELIOSTATFACTORY_H_*/
