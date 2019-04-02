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
 
#ifndef TTRACKERFACTORY_H_
#define TTRACKERFACTORY_H_

#include <QtPlugin>

class QString;
class QIcon;
class TTracker;

class TTrackerFactory
{
public:
    virtual ~TTrackerFactory() {}
    virtual QString TTrackerName() const  = 0;
    virtual QIcon TTrackerIcon() const = 0;
    virtual TTracker* CreateTTracker( ) const = 0;
};

Q_DECLARE_INTERFACE( TTrackerFactory, "tonatiuh.TTrackerFactory")


#endif /*TTRACKERFACTORY_H_*/
