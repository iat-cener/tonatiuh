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

#include "InstanceNode.h"
#include "NodesFilterModel.h"
#include "SceneModel.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"
#include "TShape.h"



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
void NodesFilterModel::AddShapeTypeFilter( QString shapeType )
{
	if( !m_shapeTypeList.contains( shapeType ) )
		m_shapeTypeList.push_back( shapeType );
}

/*!
 * Sets the shape filters to \a  shapeTypeFilters. Previously defined shape filters will be removed.
 */
void NodesFilterModel::SetShapeTypeFilters( QVector< QString >  shapeTypeFilters )
{
	m_shapeTypeList.clear();
	m_shapeTypeList = shapeTypeFilters;
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


	if( node->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )	return ( true );
	if( node->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
	{
		if( m_shapeTypeList.count() < 1 )	return ( true );

		TShapeKit* shapeKit = static_cast< TShapeKit* >( node );
		if(!shapeKit)	return ( false );
		TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );

		if( shape  &&  m_shapeTypeList.contains( shape->getTypeId().getName().getString() ) )
			return ( true );
	}


	return ( false );
}
