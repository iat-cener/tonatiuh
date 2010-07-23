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

#include <QFileDialog>
#include <QMessageBox>

#include "ExportDialog.h"
#include "InstanceNode.h"
#include "TShapeKit.h"

ExportDialog::ExportDialog( SceneModel& sceneModel, QWidget* parent )
:QDialog( parent ),m_exportSceneModel( &sceneModel ),  m_exportSelectionModel( 0 )

{
	setupUi( this );

	m_exportSelectionModel = new QItemSelectionModel( m_exportSceneModel );

	modelView->setModel( m_exportSceneModel );
	modelView->setSelectionModel( m_exportSelectionModel );
	modelView->setSelectionMode(QAbstractItemView::SingleSelection);
	modelView->setSelectionBehavior(QAbstractItemView::SelectRows);
	modelView->setRootIsDecorated(true);
}

ExportDialog::~ExportDialog()
{
	delete m_exportSelectionModel;
}

/*!
 * Return selected coordinate system id.
 *
 * If global coordinate system is selected, returns \a 0. To local coordinate system returns \a 1.
 */
int ExportDialog::GetCoordinateSystem() const
{
	if( globalCoordinatesRadio->isChecked() ) return 0;
	else return 1;
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


int ExportDialog::GetSelectedPhotons() const
{
	if( allMapRadio->isChecked() ) return 0;
	else return 1;
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

void ExportDialog::accept()
{
	if( fileNameEdit->text().isEmpty() )
	{
		QMessageBox::information( this, "Tonatiuh Action",
			                          "No file defined to save Photon Map", 1);
			    return;
	}

	if( surfaceMapRadio->isChecked () )
	{
		if( m_exportSelectionModel->hasSelection() )
		{
			InstanceNode* selectedNode = m_exportSceneModel->NodeFromIndex( m_exportSelectionModel->currentIndex() );
			if( !selectedNode->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
			{
				QMessageBox::information( this, "Tonatiuh Action",
					                          "Selected node is not surface node", 1);
				return;
			}

		}
	}
	QDialog::accept();

}

void ExportDialog::on_openButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName( this, tr("Export PhotonMap"),
				tr( "." ),
	            tr( "Binary data files (*.dat)" ) );

	if( !fileName.isEmpty() )	fileNameEdit->setText( fileName );
}
