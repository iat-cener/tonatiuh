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
QSettings
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "PhotonMapExportFile.h"
#include "PhotonMapExportFileWidget.h"

/*!
 * Creates a widget for the plugin parameters.
 */
PhotonMapExportFileWidget::PhotonMapExportFileWidget( QWidget* parent )
:PhotonMapExportParametersWidget( parent)
{
	setupUi( this );
	SetupTriggers();

}

/*!
 * Destroys widget object.
 */
PhotonMapExportFileWidget::~PhotonMapExportFileWidget()
{

}

/*!
 * Returns the plugin parameters names.
 */
QStringList PhotonMapExportFileWidget::GetParameterNames() const
{
	return PhotonMapExportFile::GetParameterNames();
}

/*!
 * Return the value of
 */
QString PhotonMapExportFileWidget::GetParameterValue( QString parameter ) const
{
	QStringList parametersName = GetParameterNames();

	//Directory name
	if( parameter == parametersName[0] )
		return saveDirectoryLine->text();

	//File name.
	else if( parameter == parametersName[1] )
	{
		return filenameLine->text();
	}

	//Maximum number of photons that a file can store.
	else if( parameter == parametersName[2] )
	{
		if( !photonsPerFileCheck->isChecked() )	return QString::number( -1 );
		else	return QString::number( nOfPhotonsSpin->value() );
	}

	return QString();
}

/*!
 * Select existing directory to save the data exported from the photon.
 */
void PhotonMapExportFileWidget::SelectSaveDirectory()
{
	QSettings settings( QLatin1String( "NREL UTB CENER" ), QLatin1String( "Tonatiuh" ) );
	QString lastUsedDirectory = settings.value( QLatin1String( "PhotonMapExportFileWidget.directoryToExport" ),
			QLatin1String( "." ) ).toString();


	QString directoryToExport = QFileDialog::getExistingDirectory ( this, tr( "Save Direcotry" ), lastUsedDirectory );
	if( directoryToExport.isEmpty() )	return;


	QDir dirToExport( directoryToExport );
	if( !dirToExport.exists() )
	{
		QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Selected directory is not valid." ), 1 );
		return;

	}

	settings.setValue( QLatin1String( "PhotonMapExportFileWidget.directoryToExport" ), directoryToExport );
	saveDirectoryLine->setText( directoryToExport );

}

/*!
 * Setups triggers for the buttons.
 */
void PhotonMapExportFileWidget::SetupTriggers()
{
	connect( selectDirectoryButton, SIGNAL( clicked() ), this, SLOT( SelectSaveDirectory() ) );
}
