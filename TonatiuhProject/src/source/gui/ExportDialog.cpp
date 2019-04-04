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

#include <QFileDialog>
#include <QMessageBox>

#include "ExportDialog.h"
#include "InstanceNode.h"
#include "TShapeKit.h"

/**
 * Creates a dialog to define export parameters.
 * The dialog need \a sceneModel to allow user to select a surface to export.
 * Sets export mode to export all photos if \a previousSurfaceUrl is empty. Otherwise, sets to surface export mode.
 * Sets global coordinate system if \a previusInGlobal is \a true. Otherwise, sets to local coordinate system.
 * The current export file value takes the \a previousFile name. The file \a previousFile is the last file uses to export a photon map.
 */
ExportDialog::ExportDialog( SceneModel& sceneModel, QString previousSurfaceUrl, bool previusInGlobal, QString previousFile, QWidget* parent )
:QDialog( parent ),
 m_exportSceneModel( &sceneModel ),
 m_exportSelectionModel( 0 )
{
	setupUi( this );
	connect( allMapRadio, SIGNAL( toggled( bool ) ), this, SLOT( SetExportAllPhotons( bool ) ) );
	connect( surfaceMapRadio, SIGNAL( toggled( bool ) ), this, SLOT( SetExportSurfacePhotons( bool ) ) );
	connect( selectFile, SIGNAL( clicked() ), this, SLOT( SelectFile() ) );


	m_exportSelectionModel = new QItemSelectionModel( m_exportSceneModel );

	modelView->setModel( m_exportSceneModel );
	modelView->setSelectionModel( m_exportSelectionModel );
	modelView->setSelectionMode(QAbstractItemView::SingleSelection);
	modelView->setSelectionBehavior(QAbstractItemView::SelectRows);
	modelView->setRootIsDecorated(true);

	if( !previousSurfaceUrl.isEmpty() )
	{
		SetExportSurfacePhotons( true );
		QModelIndex selectedNodeIndex = m_exportSceneModel->IndexFromNodeUrl( previousSurfaceUrl );
		m_exportSelectionModel->select( selectedNodeIndex, QItemSelectionModel::ClearAndSelect );

		QModelIndex parentIndex = selectedNodeIndex.parent();
		while( parentIndex != QModelIndex() )
		{
			modelView->expand( parentIndex );
			parentIndex = parentIndex.parent();
		}
	}

	if( previusInGlobal )	globalCoordinatesRadio->setChecked( true );
	else	localCoordinates->setChecked( true );

	if( !previousFile.isEmpty() )	fileNameEdit->setText( previousFile );
}

ExportDialog::ExportDialog( SceneModel& sceneModel, QWidget* parent )
:QDialog( parent ),
 m_exportSceneModel( &sceneModel ),
 m_exportSelectionModel( 0 )
{
	setupUi( this );
	//connect( allMapRadio, SIGNAL( toggled( bool ) ), this, SLOT( SetExportAllPhotons( bool ) ) );
	connect( surfaceMapRadio, SIGNAL( toggled( bool ) ), this, SLOT( SetExportSurfacePhotons( bool ) ) );
	connect( selectFile, SIGNAL( clicked() ), this, SLOT( SelectFile() ) );

	m_exportSelectionModel = new QItemSelectionModel( m_exportSceneModel );

	modelView->setModel( m_exportSceneModel );
	modelView->setSelectionModel( m_exportSelectionModel );
	modelView->setSelectionMode(QAbstractItemView::SingleSelection);
	modelView->setSelectionBehavior(QAbstractItemView::SelectRows);
	modelView->setRootIsDecorated(true);

	allMapRadio->setVisible(false);
	surfaceMapRadio->setChecked(true);
	fileWidget->setVisible(false);
}
/*!
 * Destroys the file dialog.
 */
ExportDialog::~ExportDialog()
{
	delete m_exportSelectionModel;
}

/*!
 * Returns \a true if the option export all photon map option is selected. Otherwise, returns \a false.
 */
bool ExportDialog::ExportAllPhotonMap() const
{
	if( allMapRadio->isChecked() ) return true;
	else return false;
}

/*!
 * Return \a true if the global coordinates system is selected. Otherwise, returns \a  false.
 */
bool ExportDialog::ExportPhotonsInGlobal() const
{
	if( globalCoordinatesRadio->isChecked() ) return true;
	else return false;
}

/*!
 * Returns id to indicate selected photon map photons.
 *
 * If all photon map is selected, returns \a 0. To surface photon map returns \a 1.
 *
 * \sa GetSelectedSurface
 */
QString ExportDialog::GetExportFileName() const
{
	return fileNameEdit->text();
}

/*!
 * Return selected surface node url.
 *
 * Returns an empty string if there is not selection or selected node is not surface node.
 */
QString ExportDialog::GetSelectedSurface() const
{
	if( !surfaceMapRadio->isChecked () ) return 0;
	if( !m_exportSelectionModel->hasSelection() ) return 0;

	InstanceNode* selectedNode = m_exportSceneModel->NodeFromIndex( m_exportSelectionModel->currentIndex() );
	if( !selectedNode->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) return 0;

	//return m_exportSceneModel->PathFromIndex( m_exportSelectionModel->currentIndex() );
	return selectedNode->GetNodeURL();
}

/*!
 * Hides the modal dialog and sets the result code to Accepted.
 */
void ExportDialog::accept()
{
	if(allMapRadio->isVisible()){
	if( fileNameEdit->text().isEmpty() )
		{
			QMessageBox::information( this, "Tonatiuh ",  "No file defined to save photon map", 1);
			    return;
		}
	}
	if( surfaceMapRadio->isChecked () )
	{
		if( m_exportSelectionModel->hasSelection() )
		{
			InstanceNode* selectedNode = m_exportSceneModel->NodeFromIndex( m_exportSelectionModel->currentIndex() );
			if( !selectedNode->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
			{
				QMessageBox::information( this, "Tonatiuh",	"Selected node is not surface node", 1);
				return;
			}

		}
	}
	QDialog::accept();

}

/*!
 * If \a allPhotos is true set export mode to export all photon map.
 * Otherwise \a sets to export all photons if surface photons is not checked.
 */
void ExportDialog::SetExportAllPhotons( bool allPhotos )
{
	if( allPhotos )	allMapRadio->setChecked( true );
	else if( ( !allPhotos ) && ( !surfaceMapRadio->isChecked() ) )
		allMapRadio->setChecked( true );
}

/*!
 * If \a allPhotos is true set export mode to export surface photons.
 * Otherwise \a sets to export surface photons if all photons is not checked.
 */
void ExportDialog::SetExportSurfacePhotons( bool surfacePhotos )
{
	if( surfacePhotos )	surfaceMapRadio->setChecked( true );
	else if( ( !surfacePhotos ) && ( !allMapRadio->isChecked() ) )
		surfaceMapRadio->setChecked( true );
}

/*!
 * Opens a dialog to select a file as the the file to save the export information.
 *
 * \a fileNameEdit line will show the selected file path.
 */
void ExportDialog::SelectFile()
{
	QString currentDir = ".";

	QFileInfo currentFile( fileNameEdit->text() );
	QDir fileDir = currentFile.absoluteDir();
	if( fileDir.exists() ) currentDir = fileDir.absolutePath();


	QString fileName = QFileDialog::getSaveFileName( this, tr("Export PhotonMap"),
				currentDir,
	            tr( "Binary data files (*.dat)" ) );

	if( !fileName.isEmpty() )	{
				QStringList list = fileName.split(".");
				if(list.size()<2){
					fileName.append(".dat");
				}
				fileNameEdit->setText( fileName );

			}

}


