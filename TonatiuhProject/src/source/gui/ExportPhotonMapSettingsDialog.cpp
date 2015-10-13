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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
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
	std::cout<<"currentSurfaceIndex: "<<currentSurfaceIndex<<std::endl;
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
