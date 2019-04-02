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
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "PhotonMapExportDBParametersWidget.h"
#include "PhotonMapExportDB.h"


PhotonMapExportDBParametersWidget::PhotonMapExportDBParametersWidget( QWidget* parent )
:PhotonMapExportParametersWidget( parent )
{
	setupUi( this );
	SetupTriggers();
}

PhotonMapExportDBParametersWidget::~PhotonMapExportDBParametersWidget()
{

}

QStringList PhotonMapExportDBParametersWidget::GetParameterNames() const
{

	return PhotonMapExportDB::GetParameterNames();
}

QString PhotonMapExportDBParametersWidget::GetParameterValue( QString parameter ) const
{
	QStringList parametersName = GetParameterNames();

		//Directory name
		if( parameter == parametersName[0] )
			return saveDirectoryDBLine->text();

		//File name.
		else if( parameter == parametersName[1] )
		{
			return filenameDBLine->text();
		}

	return QString();
}

void PhotonMapExportDBParametersWidget::SelectSaveDirectory()
{

	QSettings settings( QLatin1String( "NREL UTB CENER" ), QLatin1String( "Tonatiuh" ) );
	QString lastUsedDirectory = settings.value( QLatin1String( "PhotonMapExportDBParametersWidget.directoryToExport" ),
			QLatin1String( "." ) ).toString();


	QString directoryToExport = QFileDialog::getExistingDirectory ( this, tr( "Save Direcotry" ), lastUsedDirectory );
	if( directoryToExport.isEmpty() )	return;


	QDir dirToExport( directoryToExport );
	if( !dirToExport.exists() )
	{
		QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Selected directory is not is not  valid." ), 1 );
		return;

	}

	settings.setValue( QLatin1String( "PhotonMapExportDBParametersWidget.directoryToExport" ), directoryToExport );
	saveDirectoryDBLine->setText( directoryToExport );
}

void PhotonMapExportDBParametersWidget::SetupTriggers()
{
	connect( selectDirectoryDBButton, SIGNAL( clicked() ), this, SLOT( SelectSaveDirectory() ) );
}
