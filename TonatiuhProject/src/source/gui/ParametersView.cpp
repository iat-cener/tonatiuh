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

#include <QString>

#include <Inventor/nodekits/SoBaseKit.h>

#include "FieldContainerWidget.h"
#include "ParametersView.h"

/**
 * Creates a new ParametersView with parent \a parent.
 */
ParametersView::ParametersView( QWidget* parent )
: QTabWidget( parent ),
  m_actualCoinNode( 0 ), m_isPart(false)
{
	addTab( new FieldContainerWidget( 0, "" ), tr("Transform") );
	addTab( new FieldContainerWidget( 0, "" ), tr("Shape") );

}
/*!
 * Destroys the parameters view widget.
 */
ParametersView::~ParametersView()
{

}


/*!
 * Changes the parameters view to show \a coinNode \a parts parameters.
 */
void ParametersView::SelectionChangedToPart( SoNode* coinPart )
{
	clear();

	if (coinPart->getTypeId().isDerivedFrom(SoNodeKitListPart::getClassTypeId()))
	{
		/*SoNodeKitListPart* parentGroup = static_cast< SoNodeKitListPart* >( coinPart );
		if( parentGroup )
		{
			int child=0;
			while( child < parentGroup->getNumChildren() && child <10)
			{
				SoNode* element = (SoNode*)  parentGroup->getChild(child);
				if( element )	AddTab( element, "" );
				child++;
			}
		}*/
	}
	else
	{
		m_actualCoinNode = coinPart;
		m_isPart =true;
		AddTab( coinPart, "" );
	}
}

/*!
 * Changes the parameters view to show \a coinNode \a parts parameters.
 */
void ParametersView::SelectionChangedToKit( SoBaseKit* coinNode/*, QStringList parts*/ )
{
	clear();

	QStringList	parts = ContainerNodeParts( coinNode );

	m_actualCoinNode = coinNode;
	m_isPart =false;
	for( int i = 0; i< parts.size(); ++i )
	{
		QString partName = parts[i];

		if (partName == QString( "" )) AddTab( coinNode, "" );
		else if (partName[partName.length()-1]=='*')
		{
			QString partName2 = partName.left(partName.length()-1);
			SoNodeKitListPart* parentGroup = static_cast< SoNodeKitListPart* >( coinNode->getPart(partName2.toStdString().c_str(), false ) );
			if( parentGroup )
			{
				int child=0;
				while( child < parentGroup->getNumChildren() && child <10)
				{
					SoNode* element = (SoNode*)  parentGroup->getChild(child);
					if( element )	AddTab( element, "" );
					child++;
				}
			}
		}
		else
		{
			SoNode* coinPart = coinNode->getPart(partName.toStdString().c_str(), false );
			if( coinPart != 0 ) AddTab( coinPart, partName );
		}
	}

}

/*!
 * Updates selected node parametes views.
 */
void ParametersView::UpdateView()
{
	if (m_isPart) SelectionChangedToPart (m_actualCoinNode);
	else SelectionChangedToKit(  (SoBaseKit*)m_actualCoinNode );
}

/*!
 * Emits a valueModificated signal with \a node as the actual node, \a paramenterName and \a newValue.
 */
void ParametersView::SetValue( SoNode* node, QString paramenterName, QString newValue )
{
	emit valueModificated( node, paramenterName, newValue );
}

/*!
 * Adds a new tab to the view with \a coinNode \a partName parameters.
 */
void ParametersView::AddTab( SoNode* coinNode, QString partName )
{
	QString type = coinNode->getName().getString();
	if (type.length()<=0) type = coinNode->getTypeId().getName().getString();

	FieldContainerWidget* nodeContainer = new FieldContainerWidget( coinNode, partName, this );
	addTab( nodeContainer, type );
	connect( nodeContainer, SIGNAL( valueModificated( SoNode*, QString, QString ) ), this, SLOT( SetValue( SoNode*, QString, QString ) ) );
}

/*!
 * Returns the names of the parts of the \a coinNode that the view shows.
 *
 * If the \a coinNode is not a container node, return a empty list.
 */
QStringList ParametersView::ContainerNodeParts( SoBaseKit* coinNode )
{
	QStringList parts;
	if( !coinNode && ! coinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )	return parts;

	SoBaseKit* nodeKit = static_cast< SoBaseKit* >( coinNode );
	QString type = nodeKit->getTypeId().getName().getString();

	if ( type == "TLightKit" )	parts<<QString( "transform" )<<QString( "icon" )<<QString( "tsunshape" );
	else if( type == "TShapeKit" )	parts<<QString( "shape" )<<QString( "appearance.material" );
	else if( type == "TAnalyzerKit" )	parts<<QString( "parameter" )<<QString( "result" )<<QString( "levelList*" )<<QString( "transform" );
	else if( type == "TAnalyzerResultKit" )	parts<<QString( "result" );
	else	parts<<QString( "transform" );

	return parts;
}
