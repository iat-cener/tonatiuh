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

#include <QItemSelectionModel>
#include <QMessageBox>

#include "InstanceNode.h"
#include "SceneModel.h"
#include "SceneModelView.h"
#include "SelectSurfaceDialog.h"
#include "TShapeKit.h"

/*!
 *Creates a dialog object to select a surface from the \a currentSceneModel.
 */
SelectSurfaceDialog::SelectSurfaceDialog( SceneModel& currentSceneModel, QWidget* parent )
:QDialog( parent ),
 m_pCurrentSceneModel( &currentSceneModel ),
 m_pSelectionModel( 0 )
{
	setupUi( this );


	m_pSelectionModel = new QItemSelectionModel( m_pCurrentSceneModel );

	sceneModelView->setModel( m_pCurrentSceneModel );
	QModelIndex viewRootNodeIndex = currentSceneModel.IndexFromNodeUrl( QString( "//SunNode" ) );
	sceneModelView->setRootIndex( viewRootNodeIndex );
	sceneModelView->setSelectionModel( m_pSelectionModel );
	sceneModelView->setSelectionMode( QAbstractItemView::SingleSelection );
	sceneModelView->setSelectionBehavior( QAbstractItemView::SelectRows );
	sceneModelView->setRootIsDecorated( true );
}

/*!
 * Destroys dialog object.
 */
SelectSurfaceDialog::~SelectSurfaceDialog()
{

}

/*!
 * Checks if the selected node is valid node before close dialog.
 */
void SelectSurfaceDialog::accept()
{
	if( sceneRadio->isChecked() && m_pSelectionModel->hasSelection() )
	{
		InstanceNode* selectedNode = m_pCurrentSceneModel->NodeFromIndex( m_pSelectionModel->currentIndex() );
		if( !selectedNode->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
		{
			QMessageBox::information( this, QLatin1String( "Tonatiuh" ), tr( "Selected node is not surface node" ), 1 );
			return;
		}

	}

	QDialog::accept();
}

/*!
 * Returns selected surface url.
 * If the selected node is not a surface node return an empty url.
 */
QString SelectSurfaceDialog::GetSelectedSurfaceURL() const
{
	if( lightRadio->isChecked() )
		return QLatin1String( "//Light" );
	QModelIndex currentIndex = m_pSelectionModel->currentIndex();
	if( !currentIndex.isValid() )	return QString();

	InstanceNode* currentNode = m_pCurrentSceneModel->NodeFromIndex( currentIndex );
	if( !currentNode )	return QString();

	return currentNode->GetNodeURL();
}


