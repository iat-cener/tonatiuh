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

#ifndef SUNSHAPEBUIEFACTORY_H_
#define SUNSHAPEBUIEFACTORY_H_

#include <QObject>

#include "SunshapeBuie.h"
#include "TSunShapeFactory.h"


class SunshapeBuieFactory: public QObject, public TSunShapeFactory
{
    Q_OBJECT
    Q_INTERFACES(TSunShapeFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TSunShapeFactory")
#endif

    
public:
   	QString TSunShapeName() const;
   	QIcon TSunShapeIcon() const;
   	SunshapeBuie* CreateTSunShape( ) const;
};

#endif /*SUNSHAPEBUIEFACTORY_H_*/
