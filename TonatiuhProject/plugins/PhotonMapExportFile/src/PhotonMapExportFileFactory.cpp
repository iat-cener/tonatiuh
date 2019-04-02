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

#include "PhotonMapExportFileFactory.h"

QString PhotonMapExportFileFactory::GetName() const
{
	return QString("Binary_file");
}

QIcon PhotonMapExportFileFactory::GetIcon() const
{
	return QIcon(":/icons/PhotonMapExportFile.png");
}

/*!
 * Returns new ExportPhotonMap class object.
 */
PhotonMapExportFile* PhotonMapExportFileFactory::GetExportPhotonMapMode( ) const
{
	return new PhotonMapExportFile();
}

/*!
 * Returns a widget to define the plugin parameters.
 */
PhotonMapExportFileWidget* PhotonMapExportFileFactory::GetExportPhotonMapModeWidget() const
{
	return new PhotonMapExportFileWidget();
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2( PhotonMapExportFile, PhotonMapExportFileFactory )
#endif





