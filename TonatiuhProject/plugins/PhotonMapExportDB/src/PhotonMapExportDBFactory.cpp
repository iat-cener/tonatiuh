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
#include <QString>

#include "PhotonMapExportDBFactory.h"

/*!
 * Returns the export type name.
 */
QString PhotonMapExportDBFactory::GetName() const
{
	return QString( "SQL_Database" );
}

QIcon PhotonMapExportDBFactory::GetIcon() const
{
	return QIcon(":/icons/PhotonMapExportDB.png");
}

/*!
 * Returns new ExportPhotonMap class object.
 */
PhotonMapExportDB* PhotonMapExportDBFactory::GetExportPhotonMapMode( ) const
{
	return new PhotonMapExportDB();
}

/*!
 * Returns a widget to define the parameters to export to a database.
 */
PhotonMapExportDBParametersWidget* PhotonMapExportDBFactory::GetExportPhotonMapModeWidget() const
{
	return new PhotonMapExportDBParametersWidget();
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2( PhotonMapExportDB, PhotonMapExportDBFactory )
#endif


