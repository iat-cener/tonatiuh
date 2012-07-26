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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <sstream>

#include <QItemSelectionModel>
#include <QMessageBox>
#include <QFileDialog>

#include "gc.h"

#include "ExportDialog.h"
#include "RunOptionsDialog.h"
#include "SceneModel.h"

/**
 * Creates a new dialog for the light definition.
 *
 * The dialog present a list given by \a sunshapeFactoryList to define light sunshape, \a shapeFactoryList for the light input
 * aperture and shows the the light parameters defined in the light \a currentLightKit.
 */

RunOptionsDialog::RunOptionsDialog(SceneModel& sceneModel, QWidget* parent )
:QDialog( parent ),
 m_exportSceneModel( &sceneModel )
{
	setupUi( this );

	connect( selectSurfaceButton, SIGNAL( clicked() ), this, SLOT( SelectSurface() ) );

	connect( selectFileSave, SIGNAL( clicked() ), this, SLOT( SelectFileSave() ) );
	connect( SQLselectFile, SIGNAL( clicked() ), this, SLOT( SQLSelectFile() ) );

	SQLWidget->setVisible(false);

}

/*!
 * Destroys the RunOptionsDialog object.
 */
RunOptionsDialog::~RunOptionsDialog()
{

}

/*!
 *Returns the file name for the file or files to export.
 */
QString RunOptionsDialog::GetExportFileName() const
{
	return fileNameEdit->text();
}

/*!
 * Returns the number of photons that can be saved into a file.
 */
int RunOptionsDialog::GeNumberOfPhotonsPerFile() const
{
	return raysPerFileSpinBox->value();
}

/*!
 * Returns the export surface node name.
 */
QString RunOptionsDialog::GetSurfaceNode() const
{
	return m_surfaceNodeName;
}

void RunOptionsDialog::SelectFileSave()
{
	QString currentDir = ".";

	QFileInfo currentFile( fileNameEdit->text() );
	QDir fileDir = currentFile.absoluteDir();
	if( fileDir.exists() ) currentDir = fileDir.absolutePath();


	QString fileName = QFileDialog::getSaveFileName( this, tr("Export PhotonMap"),
				currentDir,
	            tr( "Binary data files (*.dat)" ) );

	if( !fileName.isEmpty() )
	{
		QStringList list = fileName.split(".");
		if( list.size() < 2 )	fileName.append(".dat");
		fileNameEdit->setText( fileName );

	}
}

/*!
 * Opens a ExportDialog to select a surface.
 */
void RunOptionsDialog::SelectSurface()
{
	ExportDialog exportDialog( *m_exportSceneModel, this );

	if( exportDialog.exec() )
	{

		m_globalCoordinates = exportDialog.ExportPhotonsInGlobal();
		surfacePathLineEdit->setText( exportDialog.GetSelectedSurface() );
    }
}


void RunOptionsDialog::SQLSelectFile()
{
	QString currentDir = ".";

	QFileInfo currentFile( SQLNameEdit->text() );
	QDir fileDir = currentFile.absoluteDir();
	if( fileDir.exists() ) currentDir = fileDir.absolutePath();


	QString fileName = QFileDialog::getSaveFileName( this, tr("Export PhotonMap"),
				currentDir,
	            tr( "Binary data files (*.db)" ) );

	if( !fileName.isEmpty() )	{
			QStringList list = fileName.split(".");
			if(list.size()<2){
				fileName.append(".db");
			}
			SQLNameEdit->setText( fileName );

		}

	//PhotonMapDB photonMap(filename);
}

bool RunOptionsDialog::isLaunchActive(){
	return m_launchRays;
}

void RunOptionsDialog::accept()
{

	m_surfaceNodeName=surfacePathLineEdit->text();


	if( fileNameEdit->text().isEmpty() &&
			SQLNameEdit->text().isEmpty() &&
			m_surfaceExportFileName == "" )
	{
		QMessageBox::information( this, "Tonatiuh ",  "No file defined to save photon map", 1);
		return;

	}

	if( exportSurfaceRadioButton->isChecked() && m_surfaceNodeName == "" )
	{
		QMessageBox::information( this, "Tonatiuh ",  "No surface stored", 1);
		return;
	}

	m_launchRays = true;

	QDialog::accept();

}

void RunOptionsDialog::reject()
{
	m_launchRays = false;
	QDialog::reject();

}


/*!
 * Returns the file name for database.
 */
QString RunOptionsDialog::GetSQLExportFileName() const
{
	return SQLNameEdit->text();
}

/*!
 * Returns true file export must be done into one file.
 */
bool RunOptionsDialog::InOneFile()
{
	return oneFileRadio->isChecked();
}

/*!
 * Returns true if export to binary file is selected. Otherwise, return false.
 */
bool RunOptionsDialog::IsDatFileChecked() const
{
	return datFileRadio->isChecked();
}

/*!
 * Returns true if all photons must be exported to binary file. Otherwise, return false.
 */
bool RunOptionsDialog::IsExportAllPhotonsEnabled() const
{
	return exportAllPhotonsRadio->isChecked();
}

/*!
 *
 */
bool RunOptionsDialog::IsSavePowerPerPhotonEnabled() const
{
	return powerPerPhotonCheck->isChecked();
}

/*!
 * Returns true if the photons coordinates must be exported.
 */
bool RunOptionsDialog::IsSaveCoordinateEnabled() const
{
	return coordBox->isChecked();
}

/*!
 * Returns true if the photons side must be exported.
 */
bool RunOptionsDialog::IsSaveSideEnabled() const
{
	return sideBox->isChecked();
}

void RunOptionsDialog::ChangeSaveOptions()
{

}

bool RunOptionsDialog::ExportPhotonsInGlobal(){
	return m_globalCoordinates;
}
