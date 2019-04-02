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

#include <QItemSelectionModel>
#include <QMessageBox>

#include "InstanceNode.h"
#include "NodesFilterModel.h"
#include "SceneModel.h"
#include "SceneModelView.h"
#include "SelectSurfaceDialog.h"
#include "TShapeKit.h"

/*!
 *Creates a dialog object to select a surface from the \a currentSceneModel.
 */
SelectSurfaceDialog::SelectSurfaceDialog( SceneModel& currentSceneModel, bool enableLight , QWidget* parent )
:QDialog( parent ),
 m_isLightVisible( enableLight ),
 m_pCurrentSceneModel( &currentSceneModel ),
 m_pNodeFilterModel( 0 )
{
	setupUi( this );

	if( !enableLight  )
	{
		lightRadio->setVisible( false );
		sceneRadio->setVisible( false );
	}

	m_pNodeFilterModel = new NodesFilterModel( this );
	m_pNodeFilterModel->setSourceModel(m_pCurrentSceneModel);
    sceneModelView->setModel(m_pNodeFilterModel);


	QModelIndex viewRootNodeIndex = currentSceneModel.IndexFromNodeUrl( QString( "//SunNode" ) );
	sceneModelView->setSelectionBehavior(QAbstractItemView::SelectRows);
	sceneModelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	sceneModelView->setSelectionMode(QAbstractItemView::SingleSelection);
	sceneModelView->setRootIsDecorated( true );
	sceneModelView->setRootIndex( viewRootNodeIndex );



}

/*!
 * Destroys dialog object.
 */
SelectSurfaceDialog::~SelectSurfaceDialog()
{
	delete m_pNodeFilterModel;
}


void SelectSurfaceDialog::SetShapeTypeFilters( QVector< QString >  shapeTypeFilters )
{
	m_pNodeFilterModel->SetShapeTypeFilters( shapeTypeFilters );
}


/*!
 * Checks if the selected node is valid node before close dialog.
 */
void SelectSurfaceDialog::accept()
{

	QItemSelectionModel* selectionModel = sceneModelView->selectionModel();
	if( sceneRadio->isChecked() && selectionModel->hasSelection() )
	{

		QModelIndex selectedIndex = selectionModel->currentIndex();
		QModelIndex currentIndex = m_pNodeFilterModel->mapToSource(selectedIndex);

		InstanceNode* selectedNode = m_pCurrentSceneModel->NodeFromIndex( currentIndex );
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
	if(  m_isLightVisible  && lightRadio->isChecked() )
		return QLatin1String( "//Light" );

	QItemSelectionModel* selectionModel = sceneModelView->selectionModel();

	QModelIndex selectedIndex = selectionModel->currentIndex();
	QModelIndex currentIndex = m_pNodeFilterModel->mapToSource(selectedIndex);
	if( !currentIndex.isValid() )	return QString();

	InstanceNode* currentNode = m_pCurrentSceneModel->NodeFromIndex( currentIndex );
	if( !currentNode )	return QString();

	return ( currentNode->GetNodeURL() ) ;
}


