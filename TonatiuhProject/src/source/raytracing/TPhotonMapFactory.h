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

#ifndef TPHOTONMAPFACTORY_H_
#define TPHOTONMAPFACTORY_H_

#include <QtPlugin>

class QString;
class QIcon;
class TPhotonMap;

class TPhotonMapFactory
{
public:
	virtual ~TPhotonMapFactory() {}
    virtual QString TPhotonMapName() const  = 0;
    virtual QIcon TPhotonMapIcon() const = 0;
    virtual TPhotonMap* CreateTPhotonMap( ) const = 0;
};

Q_DECLARE_INTERFACE( TPhotonMapFactory, "tonatiuh.TPhotonMapFactory")

#endif /* TPHOTONMAPFACTORY_H_ */
