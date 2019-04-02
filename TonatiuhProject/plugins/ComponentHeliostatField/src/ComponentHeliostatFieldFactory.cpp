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
#include <QMessageBox>
#include <QVector>
#include <QVariant>
#include <iostream>


#include "ComponentHeliostatFieldFactory.h"

QString ComponentHeliostatFieldFactory::TComponentName() const
{
	return QString("Heliostat_Field_Component");
}

QIcon ComponentHeliostatFieldFactory::TComponentIcon() const
{
	return QIcon(":/icons/ComponentHeliostatField.png");
}

TSeparatorKit* ComponentHeliostatFieldFactory::CreateTComponent( PluginManager* pPluginManager ) const
{
	ComponentHeliostatField component( pPluginManager );
	return component.CreateField();
}


TSeparatorKit* ComponentHeliostatFieldFactory::CreateTComponent( PluginManager* pPluginManager, int numberofParameters, QVector< QVariant > parametersList  ) const
{
	if( numberofParameters != 15 )
	{

        QMessageBox::warning( 0, QString( "Heliostat Field Layout" ),
        		QString( "The number of arguments is not valid:\n"
        				"\t- Heliostat coordinates filename\n"
        				"\t- Heliostat component filename (leave it empty for a new design)\n"
        				"\t- Shape (Flat_Rectangle or Spherical_rectangle)\n"
        				"\t- Heliostat width [m]\n"
        				"\t- Heliostat height [m]\n"
        				"\t- Heliostat radius [m] (use -1 to calculate the radius)\n"
        				"\t- Heliostat reflectivity [%]\n"
        				"\t- Heliostat optical error [mrad]\n"
        				"\t- Aiming strategy:\n"
        				"\t\t1-File\n"
        				"\t\t2-One-Dimensional\n"
        				"\t\t3-Point\n"
        				"\t- Aiming points filename\n"
        				"\t- Receiver elevation [m]\n"
        				"\t- Receiver radius [m]\n"
        				"\t- x [m]\n"
        				"\t- y [m]\n"
        				"\t- z [m]\n") );

		return 0;
	}
	ComponentHeliostatField component( pPluginManager );
	return component.CreateField( parametersList );
}


#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(ComponentHeliostatField, ComponentHeliostatFieldFactory)
#endif
