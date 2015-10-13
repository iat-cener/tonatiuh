/*
 * NodesFilterModel.cpp
 *
 *  Created on: 09/10/2015
 *      Author: amutuberria
 */

#include "InstanceNode.h"
#include "NodesFilterModel.h"
#include "SceneModel.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"



/*!
 * Creates model object
 */
NodesFilterModel::NodesFilterModel(QObject *parent )
:QSortFilterProxyModel( parent )
 {

 }

/*!
 * Adds new node type to de filter.
 */
void NodesFilterModel::AddNodeType( QString nodeType )
{
	if( !m_nodetypeList.contains( nodeType ) )
		m_nodetypeList.push_back( nodeType );
}


/*!
 * Returns true if the \a m_nodetypeList contains the type of the element defined with \a sourceRow and \a sourceParent
 */
bool NodesFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{

	SceneModel* sceneModel = dynamic_cast< SceneModel* > ( sourceModel() );
	QModelIndex index = sceneModel->index(sourceRow, 0, sourceParent);

	InstanceNode* nodeInstance = sceneModel->NodeFromIndex( index );
	if( !nodeInstance )		return false;

	SoNode* node = nodeInstance->GetNode();
	if( !node )		return false;


	if( m_nodetypeList.contains( node->getTypeId().getName().getString() ) )
	{
		return ( true );
	}


	return ( false );
}
