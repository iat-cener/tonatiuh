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

#include <iostream>
#include <QMessageBox>

#include "ExportPhotonMapSettingsDialog.h"
#include "PhotonMapExportParametersWidget.h"
#include "PhotonMapExportSettings.h"
#include "SelectSurfaceDialog.h"

/*!
 * Creates a new dialog object to define the export settings. The available export mode types are listed into \a typeList.
 */
ExportPhotonMapSettingsDialog::ExportPhotonMapSettingsDialog( SceneModel& currentSceneModel, QVector< PhotonMapExportFactory* > typeList, QWidget* parent )
:QDialog( parent ),
 m_pCurrentSceneModel( &currentSceneModel )
{
	setupUi( this );

	for( int index = 0; index < typeList.size(); index++ )
	{
		storeTypeCombo->addItem( typeList[index]->GetIcon(), typeList[index]->GetName() );
		PhotonMapExportParametersWidget* widget = typeList[index]->GetExportPhotonMapModeWidget();
		if( widget )
		{
			parametersWidgetList.push_back( widget );
			parametersWidget->addWidget( widget );
		}
		else
		{

			parametersWidgetList.push_back( 0 );
			parametersWidget->addWidget( new QWidget() );
		}

	}
	ChangeCurrentStoreTypeParameters();
	SetupTriggers();
}

/*!
 * Destroys dialog object.
 */
ExportPhotonMapSettingsDialog::~ExportPhotonMapSettingsDialog()
{

}

/*!
 *	Returns defined settings into settings class object.
 */
PhotonMapExportSettings ExportPhotonMapSettingsDialog::GetExportPhotonMapSettings()  const
{
	PhotonMapExportSettings settings;
	settings.modeTypeName = storeTypeCombo->currentText();
	//settings.exportAllPhotonMap = ( exportAllPhotonsRadio->isEnabled() && exportAllPhotonsRadio->isChecked() );
	settings.exportCoordinates = ( coordCheck->isEnabled() && coordCheck->isChecked() );
	settings.exportIntersectionSurfaceSide =  ( sideBox->isEnabled() && sideBox->isChecked() );
	settings.exportInGlobalCoordinates =  ( globalCoordinatesRadio->isEnabled() && globalCoordinatesRadio->isChecked() );
	settings.exportPreviousNextPhotonID = ( nextPreviousCheck->isEnabled() && nextPreviousCheck->isChecked() );
	settings.exportSurfaceID =  ( surfaceIdenfierCheck->isEnabled() && surfaceIdenfierCheck->isChecked() );
	if( exportAllPhotonsRadio->isEnabled() && exportAllPhotonsRadio->isChecked() )
		settings.exportSurfaceNodeList.clear();
	else
		settings.exportSurfaceNodeList = m_exportSurfaceNodeList;

	PhotonMapExportParametersWidget* exportTypeWidget = parametersWidgetList[storeTypeCombo->currentIndex()];
	if( !exportTypeWidget )	return settings;

	QStringList exportTypeParametersName = exportTypeWidget->GetParameterNames();
	for( int p = 0; p< exportTypeParametersName.size(); p++ )
	{
		QString parameterName = exportTypeParametersName[p];
		settings.AddParameter( parameterName, exportTypeWidget->GetParameterValue( parameterName ) );
	}
	return settings;
}

/*!
 * Adds a surface to export surface list.
 */
void ExportPhotonMapSettingsDialog::AddSurfaceToExportList()
{
	SelectSurfaceDialog selectSurfaceDialog( *m_pCurrentSceneModel, true );
	if( !selectSurfaceDialog.exec( ) )	return;

	QString selectedSurfaceURL = selectSurfaceDialog.GetSelectedSurfaceURL();
	if( selectedSurfaceURL.isEmpty() )	return;

	if( m_exportSurfaceNodeList.contains( selectedSurfaceURL) )
	{
		QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Selected node has already been added." ), 1 );
		return;
	}

	m_exportSurfaceNodeList<<selectedSurfaceURL;
	surfacesListWidget->addItem( selectedSurfaceURL );

}

/*!
 * Changes the widget of the store type settings to current selected store widget.
 */
void ExportPhotonMapSettingsDialog::ChangeCurrentStoreTypeParameters()
{
	parametersWidget->setCurrentIndex( storeTypeCombo->currentIndex() );
}

/*!
 *Deletes current selected surface from export surface list.
 */
void ExportPhotonMapSettingsDialog::DeleteSurfaceFromExportList()
{
	if( !surfacesListWidget->currentItem() )	return;
	int currentSurfaceIndex = m_exportSurfaceNodeList.indexOf( surfacesListWidget->currentItem()->text() );
	if( currentSurfaceIndex < 0 ) 	return;
	m_exportSurfaceNodeList.removeAt( currentSurfaceIndex );
	delete surfacesListWidget->item( currentSurfaceIndex );
}

/*!
 * Setups triggers for the dialog buttons.
 */
void ExportPhotonMapSettingsDialog::SetupTriggers()
{
	connect( storeTypeCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeCurrentStoreTypeParameters() ) );
	connect( addSurfaceButton, SIGNAL( clicked() ), this, SLOT( AddSurfaceToExportList() ) );
	connect( deleteSurfaceButton, SIGNAL( clicked() ), this, SLOT( DeleteSurfaceFromExportList() ) );
}
