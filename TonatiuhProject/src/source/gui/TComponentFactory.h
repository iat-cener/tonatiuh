/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef TCOMPONENTFACTORY_H_
#define TCOMPONENTFACTORY_H_

#include <QtPlugin>

class PluginManager;
class QString;
class QIcon;
class TSeparatorKit;

class TComponentFactory
{
public:
    virtual ~TComponentFactory() {}
    virtual QString TComponentName() const  = 0;
    virtual QIcon TComponentIcon() const = 0;
    virtual TSeparatorKit* CreateTComponent( PluginManager* pPluginManager ) const = 0;
    virtual TSeparatorKit* CreateTComponent( PluginManager* pPluginManager, int numberofParameters, QVector< QVariant > parametersList ) const = 0;
};

Q_DECLARE_INTERFACE( TComponentFactory, "tonatiuh.TComponentFactory")


#endif /* TCOMPONENTFACTORY_H_ */
