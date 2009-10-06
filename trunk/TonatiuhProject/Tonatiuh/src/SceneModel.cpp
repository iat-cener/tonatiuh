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

#include <QIcon>
#include <QMessageBox>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSceneKit.h>

#include "InstanceNode.h"
#include "PathWrapper.h"
#include "SceneModel.h"
#include "tgf.h"
#include "TLightKit.h"
#include "TMaterial.h"
#include "Trace.h"
#include "TSeparatorKit.h"
#include "TShapeKit.h"
#include "TTracker.h"

/*!
 * Creates an empty model.
 */
SceneModel::SceneModel( QObject* parent)
:QAbstractItemModel( parent ), m_coinRoot( 0 ), m_coinScene(0), m_instanceRoot( 0 )
{
	Trace trace( "SceneModel::SceneModel", false );
}

/*!
 * Destroys the model.
 */
SceneModel::~SceneModel()
{
	Trace trace( "SceneModel::~SceneModel", false  );
}

/*!
 * Sets \a coinRoot as the model root.
 */
void SceneModel::SetCoinRoot( SoSelection& coinRoot )
{
	Trace trace( "SceneModel::SetCoinRoot", false );
	m_coinRoot = &coinRoot;
}

/*!
 * Sets the model scene to the given \a coinScene.
 *
 * Creates nodes for the model to the scene subnodes.
 */
void SceneModel::SetCoinScene( SoSceneKit& coinScene )
{
	Trace trace( "SceneModel::SetCoinScene", false );

    m_coinScene = &coinScene;
    delete m_instanceRoot;
    m_instanceRoot = new InstanceNode( m_coinScene );

    QList< InstanceNode* > instanceRootNodeList;
    m_mapCoinQt.insert( std::make_pair( &coinScene, instanceRootNodeList ) );
    m_mapCoinQt[&coinScene].append( m_instanceRoot );

    if ( m_coinScene->getPart("lightList[0]", false ) )
    {
    	TLightKit* coinLight = static_cast< TLightKit* >( m_coinScene->getPart("lightList[0]", false ) );
    	InsertLightNode( *coinLight );
    }

    SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( m_coinScene->getPart( "childList", true ) );

    if ( coinPartList && coinPartList->getNumChildren() == 0 )
    {
    	TSeparatorKit* separatorKit = new TSeparatorKit();
		separatorKit->ref();

		coinPartList->addChild( separatorKit );
    	separatorKit->setSearchingChildren( true );

    	InstanceNode* instanceSeparator = new InstanceNode( separatorKit );
    	m_instanceRoot->AddChild( instanceSeparator );

    	QList< InstanceNode* > instanceNodeList;
    	m_mapCoinQt.insert( std::make_pair( separatorKit, instanceNodeList ) );
    	m_mapCoinQt[separatorKit].append( instanceSeparator );

    }
    else
    {
		//Is TSeparatorKit node
		TSeparatorKit* coinChild = static_cast< TSeparatorKit* >( coinPartList->getChild( 0 ) );
		if ( !coinChild ) return;

        InstanceNode* instanceChild = new InstanceNode( coinChild );
	    m_instanceRoot->AddChild( instanceChild );

	    QList< InstanceNode* > instanceNodeList;
	    m_mapCoinQt.insert( std::make_pair( coinChild, instanceNodeList ) );
	    m_mapCoinQt[coinChild].append( instanceChild );

	    GenerateInstanceTree( *instanceChild );
    }
    reset();
}

void SceneModel::GenerateInstanceTree( InstanceNode& instanceParent )
{
	Trace trace( "SceneModel::GenerateInstanceTree", false );

	SoNode* parentNode = instanceParent.GetNode();

	if (parentNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		SoBaseKit* parentKit = static_cast< SoBaseKit* >( parentNode );
		if (parentNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
		{
			//Is TShapeKit node
			TShapeKit* parentKit = static_cast< TShapeKit* >( parentNode );

			SoNode* shape = parentKit->getPart("shape", false);
			if( shape )
			{
				InstanceNode* shapeChild = new InstanceNode( shape );
			   	instanceParent.AddChild( shapeChild );

			    QList< InstanceNode* > shapeNodeList;
				m_mapCoinQt.insert( std::make_pair( shape, shapeNodeList ) );
				m_mapCoinQt[shape].append( shapeChild );
			}

			SoNode* material = parentKit->getPart("appearance.material", false);
			if( material )
			{
				InstanceNode* materialChild = new InstanceNode( material );
			   	instanceParent.AddChild( materialChild );

			    QList< InstanceNode* > instanceNodeList;
				m_mapCoinQt.insert( std::make_pair( material, instanceNodeList ) );
				m_mapCoinQt[material].append( materialChild );
			}
		}
		else
		{

			//Is TSeparatorKit node
			SoNode* tracker = parentKit->getPart( "tracker", false );
			if( tracker )
			{
				TTracker* trackerNode = dynamic_cast< TTracker* >( tracker );
				trackerNode->SetSceneKit( m_coinScene );

				InstanceNode* trackerChild = new InstanceNode( tracker );
				instanceParent.AddChild( trackerChild );

				QList< InstanceNode* > trackerNodeList;
				m_mapCoinQt.insert( std::make_pair( tracker, trackerNodeList ) );
				m_mapCoinQt[tracker].append( trackerChild );
			}

			SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( parentKit->getPart( "childList", false ) );
   			if ( !coinPartList ) return;

   			for( int index = 0; index < coinPartList->getNumChildren(); index++ )
    		{
	            SoBaseKit* coinChild = static_cast< SoBaseKit* >( coinPartList->getChild( index ) );
	            InstanceNode* instanceChild = new InstanceNode( coinChild );
	    	    instanceParent.AddChild( instanceChild );

	    	    QList< InstanceNode* > instanceNodeList;
	    	    m_mapCoinQt.insert( std::make_pair( coinChild, instanceNodeList ) );
	    	    m_mapCoinQt[coinChild].append( instanceChild );

	    	    GenerateInstanceTree( *instanceChild );
	    	}
	    }
	}
}

QModelIndex SceneModel::index( int row, int column, const QModelIndex& parentModelIndex ) const
{
	Trace trace( "SceneModel::index", false );

	if ( !m_instanceRoot ) return QModelIndex();
	InstanceNode* instanceParent = NodeFromIndex( parentModelIndex );
    return createIndex( row, column, instanceParent->children[ row ] );
}

InstanceNode* SceneModel::NodeFromIndex( const QModelIndex& modelIndex ) const
{
	Trace trace( "SceneModel::NodeFromIndex", false );

	if ( modelIndex.isValid() ) return static_cast< InstanceNode* >( modelIndex.internalPointer() );
    else return m_instanceRoot;
}

int SceneModel::rowCount( const QModelIndex& parentModelIndex ) const
{
	Trace trace( "SceneModel::rowCount", false );

	InstanceNode* instanceParent = NodeFromIndex( parentModelIndex );
    return ( instanceParent ) ? ( instanceParent->children.count() ) : 0;
}

int SceneModel::columnCount( const QModelIndex& ) const
{
	Trace trace( "SceneModel::columnCount", false );

	return 1;
}

QModelIndex SceneModel::parent( const QModelIndex& childModelIndex ) const
{
	Trace trace( "SceneModel::parent", false);

    InstanceNode* instanceChild = NodeFromIndex( childModelIndex );
    if ( !instanceChild ) return QModelIndex();

    InstanceNode* instanceParent = instanceChild->GetParent();
    if ( !instanceParent ) return QModelIndex();

    InstanceNode* instanceGrandparent = instanceParent->GetParent();
    if ( !instanceGrandparent ) return QModelIndex();

    int row = instanceGrandparent->children.indexOf( instanceParent );
    return createIndex( row, childModelIndex.column(), instanceParent );
}

QVariant SceneModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	Trace trace( "SceneModel::headerData", false );

	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if ( section == 0 ) return tr( "Node" );
    }
    return QVariant();
}

QVariant SceneModel::data( const QModelIndex& modelIndex, int role ) const
{
    Trace trace( "SceneModel::data", false );

	if ( role != Qt::DisplayRole && role != Qt::UserRole && role != Qt::DecorationRole ) return QVariant();

    InstanceNode* instanceNode = NodeFromIndex( modelIndex );
    if ( !instanceNode ) return QVariant();


    if ( modelIndex.column() == 0 )
    {
    	SoNode* coinNode = instanceNode->GetNode();
    	if ( !coinNode ) return QVariant();

    	if ( role == Qt::DisplayRole )
    	{
    	    if ( coinNode )
    	    {
    	    	QString nodeName;
    	    	if ( coinNode->getName() == SbName() )
    	     		nodeName = QString( coinNode->getTypeId().getName().getString() );
    	    	else
    	    		nodeName = QString( coinNode->getName().getString() );


				SoSearchAction* coinSearch = new SoSearchAction();
				coinSearch->setNode( coinNode );
				coinSearch->setInterest( SoSearchAction::ALL);
				coinSearch->apply( m_coinRoot );

				int numReferences = coinSearch->getPaths( ).getLength();

				QString references ( " ( " );
				references += QString::number( numReferences, 10);
				references += " )  " ;

				delete coinSearch;

    	    	return references + nodeName;
    	    }
    	    else return QVariant();
    	}

        if ( role == Qt::UserRole )
        {
        	SoNodeKitPath* coinKitPath = PathFromIndex(modelIndex);
        	if ( coinKitPath )
        	{
		        PathWrapper pathWrapper( coinKitPath );
	            return QVariant::fromValue( pathWrapper );

        	}
        	return  QVariant();
        }
        if (role == Qt::DecorationRole)
        {
			if( coinNode->getTypeId().isDerivedFrom( TLightKit::getClassTypeId() ) )
			{
				return QIcon(":/icons/lightKit.png");
			}
			else if( coinNode->getTypeId().isDerivedFrom(TSeparatorKit::getClassTypeId() ) )
			{
				TSeparatorKit* separatorKit = static_cast<TSeparatorKit*>( coinNode );
				return QIcon( separatorKit->getIcon() );

			}
			else if( coinNode->getTypeId().isDerivedFrom(TShapeKit::getClassTypeId() ) )
			{
				SoBaseKit* nodeKit = static_cast< SoBaseKit* >( coinNode );
				TShape* kit = static_cast<TShape*>( nodeKit->getPart( "shape", false ) );
				if( kit ) return QIcon( kit->getIcon() );
				return QIcon( ":/icons/shapeKit.png" );
			}
			else if( coinNode->getTypeId().isDerivedFrom(SoShape::getClassTypeId() ) )
			{
				TShape* shape = static_cast<TShape*>( coinNode );
				return QIcon(shape->getIcon());
			}
			else if( coinNode->getTypeId().isDerivedFrom(TMaterial::getClassTypeId() ) )
			{
				TMaterial* material = static_cast<TMaterial*>( coinNode );
				return QIcon(material->getIcon());
			}
			else if( coinNode->getTypeId().isDerivedFrom(TTracker::getClassTypeId() ) )
			{
				TTracker* tracer = static_cast<TTracker*>( coinNode );
				return QIcon(tracer->getIcon());
			}
     	}
    }
    return QVariant();
}

int SceneModel::InsertCoinNode( SoNode& coinChild, SoBaseKit& coinParent )
{
	Trace trace( "SceneModel::InsertCoinNode", false );

	int row = -1;
	if (coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		if( coinChild.getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )
		{
			coinParent.setPart( "tracker", &coinChild );
			row = 0;
		}
		else
		{

			SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", true ) );
			if ( !coinPartList ) return -1;

			row = coinPartList->getNumChildren();

			if( coinChild.getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
			{
				SoBaseKit* childKit = static_cast< SoBaseKit* >( &coinChild );
				coinPartList->addChild( childKit );
				childKit->setSearchingChildren( true );

				if( coinParent.getPart( "tracker", false ) ) row++;
			}
		}

	}
    else
    {
    	TShapeKit* shapeKit = static_cast< TShapeKit* > ( &coinParent );
    	if( shapeKit->getPart( "shape", false ) ) row++;
    	if( shapeKit->getPart( "appearance.material", false ) ) row++;
    }

	QList<InstanceNode*>& instanceListParent = m_mapCoinQt[ &coinParent ];
	for ( int index = 0; index < instanceListParent.count(); index++ )
	{
	    InstanceNode* instanceParent = instanceListParent[index];
		InstanceNode* instanceChild = new InstanceNode( &coinChild );
		instanceParent->InsertChild( row, instanceChild );

		//Inserting InstanceNode in the map
		QList< InstanceNode* > instanceNodeList;
  	  	m_mapCoinQt.insert( std::make_pair( &coinChild, instanceNodeList ) );
    	m_mapCoinQt[ &coinChild ].append( instanceChild );

    	GenerateInstanceTree( *instanceChild );
	}
	emit layoutChanged();

	return row;
}


/*!
 * Insert a light node to the model. If the model has an other light node, the previous node
 * will be deleted.
 */
void SceneModel::InsertLightNode( TLightKit& coinLight )
{
	Trace trace( "SceneModel::InsertLightNode", false );

	SoNodeKitListPart* lightList = static_cast< SoNodeKitListPart* > ( m_coinScene->getPart("lightList", true ) ) ;
	if ( lightList->getNumChildren() > 0 )	m_instanceRoot->children.removeAt( 0 );

	m_coinScene->setPart( "lightList[0]", &coinLight );

	InstanceNode* instanceLight = new InstanceNode( &coinLight );
	m_instanceRoot->InsertChild( 0, instanceLight );


	emit LightNodeStateChanged( 1 );
	emit layoutChanged();
}


void SceneModel::RemoveCoinNode( int row, SoBaseKit& coinParent )
{
	Trace trace( "SceneModel::RemoveCoinNode", false );

	if (coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		if( ( row == 0 ) && coinParent.getPart( "tracker", false ) )
		{
			coinParent.setPart( "tracker", NULL );
		}
		else
		{
			SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", false ) );
			if ( coinPartList )
			{
				if( coinParent.getPart( "tracker", false ) )	coinPartList->removeChild( row - 1 );
				else coinPartList->removeChild( row );
			}

		}

	}

    QList<InstanceNode*> instanceListParent = m_mapCoinQt[ &coinParent ];

	for( int index = 0; index< instanceListParent.size(); index++ )
	{
	    InstanceNode* instanceParent = instanceListParent[index];
	    InstanceNode* instanceNode = instanceParent->children[row];
	    instanceParent->children.removeAt(row);

	    QList<InstanceNode*>& instanceList = m_mapCoinQt[ instanceNode->GetNode()];
		instanceList.removeAt( instanceList.indexOf( instanceNode ) );
	}
	emit layoutChanged();
}

void SceneModel::RemoveLightNode( TLightKit& coinLight )
{
	Trace trace( "SceneModel::RemoveLightNode", false );

	SoNodeKitListPart* lightList = static_cast< SoNodeKitListPart* >( m_coinScene->getPart( "lightList", true ) );
    if ( lightList ) lightList->removeChild( &coinLight );

    m_instanceRoot->children.removeAt( 0 );

	emit LightNodeStateChanged( 0 );
    emit layoutChanged();

}

Qt::ItemFlags SceneModel::flags( const QModelIndex& modelIndex ) const
{
	Trace trace( "SceneModel::flags", false );

	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags( modelIndex );

	if ( modelIndex.isValid() )
	{
		if ( modelIndex.column() > 0 ) return defaultFlags;
		else return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
	}
	else return defaultFlags;
}

Qt::DropActions SceneModel::supportedDropActions() const
{
	Trace trace( "SceneModel::supportedDropActions", false );

	return  Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions SceneModel::supportedDragActions() const
{
	Trace trace( "SceneModel::supportedDragActions", false );

	return Qt::CopyAction | Qt::MoveAction;
}

void SceneModel::Cut( SoBaseKit& coinParent, int row )
{
	Trace trace( "SceneModel::Cut", false );


	if( row < 0 ) tgf::SevereError( "SceneModel::Cut invalid row number" );

	QList<InstanceNode*> instanceListParent = m_mapCoinQt[ &coinParent ];
	InstanceNode* instanceParent = instanceListParent[0];

	SoNode* coinChild = instanceParent->children[row]->GetNode();
	if( !coinChild->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		SbString partName = coinParent.getPartString( coinChild );
		if( partName.getLength() == 0 ) partName = "material";

		coinParent.setPart( partName, NULL );
	}
	else
	{
		if( coinParent.getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
		{
			SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", false ) );
			if( !coinPartList ) tgf::SevereError( "SceneModel::Cut Null coinPartList pointer" );


			if( coinParent.getPart( "tracker", false ) )
			{
				coinChild = coinPartList->getChild( row-1 );
			    coinPartList->removeChild( row -1 );
			}
			else
			{
				coinChild = coinPartList->getChild( row );
			    coinPartList->removeChild( row );
			}

			if( !coinChild ) tgf::SevereError( "SceneModel::Cut Null coinChild pointer" );

		}
	}

	QList<InstanceNode*> instanceList = m_mapCoinQt[ coinChild ];

	// Test if the node is shared
	if( instanceList.size() == 1 )
	{
	    InstanceNode* instanceNode = instanceList[0];
		DeleteInstanceTree( *instanceNode );
	}
	else
	{
		QList<InstanceNode*> instanceListParent = m_mapCoinQt[ &coinParent ];
		if( instanceListParent.size() == 1 )
		{
		    InstanceNode* instanceParent = instanceListParent[0];
		    InstanceNode* instanceNode = instanceParent->children[row];
		    DeleteInstanceTree( *instanceNode );
		}
		else
		{
			for( int index = 0; index< instanceListParent.size(); index++ )
			{
			    InstanceNode* instanceParent = instanceListParent[index];
			    InstanceNode* instanceNode = instanceParent->children[row];
			    DeleteInstanceTree( *instanceNode );
			}
		}
	}
	emit layoutChanged();
}

void SceneModel::Paste( tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinNode, int row )
{
	Trace trace( "SceneModel::Paste", false );

	SoNode* coinChild = 0;
	SoNode* pCoinParent = 0;
	pCoinParent = &coinParent;

	if ( type == tgc::Shared ) coinChild = &coinNode;


	switch ( type )
	{
		case tgc::Copied :
			coinChild = static_cast< SoNode* >( coinNode.copy( true ) );
			break;

		case tgc::Shared :
		default :
			coinChild = &coinNode;
		    break;
	}
	if( !coinChild->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		if( coinChild->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )
		{
			TSeparatorKit* separatorKit = static_cast< TSeparatorKit* >( pCoinParent );
			TTracker* tracker = static_cast< TTracker* >( separatorKit->getPart( "tracker", false ) );
			if (tracker)
			{
				QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TSeparatorKit already contains a tracker" ) );
				return;
			}
			coinParent.setPart( "tracker", coinChild );

		}

		if( coinChild->getTypeId().isDerivedFrom( SoShape::getClassTypeId() ) )
		{
			TShapeKit* shapeKit = static_cast< TShapeKit* >( pCoinParent );
			if(!shapeKit)	return;
			TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );

    		if (shape)
    		{
    			QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TShapeKit already contains a shape" ) );
    			return;
    		}
			coinParent.setPart("shape", coinChild );
		}
		if( coinChild->getTypeId().isDerivedFrom( SoMaterial::getClassTypeId() ) )
		{
			TShapeKit* shapeKit = static_cast< TShapeKit* >( pCoinParent );
			if(!shapeKit)	return;
			TMaterial* material = static_cast< TMaterial* >( shapeKit->getPart( "material", false ) );
			if (material)
    		{
    			QMessageBox::warning( 0, tr( "Tonatiuh warning" ), tr( "This TShapeKit already contains a material" ) );
    			return;
    		}
			coinParent.setPart("material", coinChild );
		}

	}
	else
	{
		SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent.getPart( "childList", true ) );
		if( !coinPartList ) tgf::SevereError( "SceneModel::Paste Null coinPartList pointer" );
		if( coinParent.getPart( "tracker", false ) ) coinPartList->insertChild( coinChild, row -1 );
		else coinPartList->insertChild( coinChild, row );
	}

	QList<InstanceNode*>& instanceListParent = m_mapCoinQt[ &coinParent ];
	for ( int index = 0; index < instanceListParent.count(); index++ )
	{
	    InstanceNode* instanceParent = instanceListParent[index];
		InstanceNode* instanceChild = new InstanceNode( coinChild );
		instanceParent->InsertChild( row, instanceChild );

		//Inserting InstanceNode in the map
		QList< InstanceNode* > instanceNodeList;
  	  	m_mapCoinQt.insert( std::make_pair( coinChild, instanceNodeList ) );
    	m_mapCoinQt[ coinChild ].append( instanceChild );
		GenerateInstanceTree( *instanceChild );
	}

	emit layoutChanged();
}

void SceneModel::DeleteInstanceTree( InstanceNode& instanceNode )
{
	Trace trace( "SceneModel::DeleteInstanceTree", false );

	for ( int index = 0; index < instanceNode.children.count(); index++)
	{
		DeleteInstanceTree( *instanceNode.children[index] );
	}

	QList<InstanceNode*>& instanceList = m_mapCoinQt[ instanceNode.GetNode()];
	instanceList.removeAt( instanceList.indexOf( &instanceNode ) );

	InstanceNode* instanceParent = instanceNode.GetParent();
	int row = instanceParent->children.indexOf( &instanceNode );

	instanceParent->children.removeAt( row );
}

SoNodeKitPath* SceneModel::PathFromIndex( const QModelIndex& modelIndex ) const
{
	Trace trace( "SceneModel::PathFromIndex", false );

	SoNode* coinNode = NodeFromIndex( modelIndex )->GetNode();

	if (!coinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		QModelIndex parentIndex = parent( modelIndex );
		SoNodeKitPath* parentPath = PathFromIndex( parentIndex );
		return parentPath;

	}
	else
	{

		SoSearchAction* coinSearch = new SoSearchAction();
		coinSearch->setNode( m_coinScene );
		coinSearch->setInterest( SoSearchAction::FIRST);
		coinSearch->apply( m_coinRoot );

		SoPath* coinScenePath = coinSearch->getPath( );
		if( !coinScenePath ) tgf::SevereError( "PathFromIndex Null coinScenePath." );

		SoNodeKitPath* nodePath = static_cast< SoNodeKitPath* > ( coinScenePath );
		if( !nodePath ) tgf::SevereError( "PathFromIndex Null nodePath." );

		InstanceNode* instanceNode = NodeFromIndex( modelIndex );

		SoNodeList nodeList;
		while (instanceNode->GetParent() )
		{
			nodeList.append( instanceNode->GetNode() );
			instanceNode = instanceNode->GetParent();
		}

		for( int i = nodeList.getLength(); i > 0; i--){
			SoBaseKit* coinNode = static_cast< SoBaseKit* >( nodeList[i-1] );
			if ( coinNode )
				nodePath->append( coinNode );
		}
		return nodePath;
	}
}

QModelIndex SceneModel::IndexFromPath( const SoNodeKitPath& coinNodePath ) const
{
	Trace trace( "SceneModel::IndexFromPath", false );

	SoBaseKit* coinNode = static_cast< SoBaseKit* >( coinNodePath.getTail() );
	if( !coinNode ) tgf::SevereError( "IndexFromPath Null coinNode." );

	if( coinNode->getTypeId().getName().getString() == SbName("TLightKit") ) return index( 0, 0 );

	if ( coinNodePath.getLength() > 1 )
	{
		SoBaseKit* coinParent = static_cast< SoBaseKit* >( coinNodePath.getNodeFromTail(1) );
		if( !coinParent ) tgf::SevereError( "IndexFromPath Null coinParent." );

		if( coinParent->getTypeId().getName() == SbName( "TLightKit") ) return index( 0, 0 );

		SoNodeKitListPart* coinPartList = static_cast< SoNodeKitListPart* >( coinParent->getPart( "childList", true ) );
    	if( !coinPartList ) tgf::SevereError( "IndexFromPath Null coinPartList." );

    	int row = coinPartList->findChild(coinNode);
		if(coinNodePath.getNodeFromTail(1) == m_coinScene )
		{
			int child = 0;
			while( child < m_instanceRoot->children.count() )
			{
				if ( m_instanceRoot->children[child]->GetNode() == coinNode )
					break;
				child++;
			}
			return index(child, 0 );
		}else{
			SoNodeKitPath* parentPath = static_cast< SoNodeKitPath* > ( coinNodePath.copy( 0, 0 ) );
			parentPath->truncate(parentPath->getLength()-1 );
			QModelIndex parentIndex = IndexFromPath( *parentPath );

			return index (row, 0, parentIndex );
		}
	}
	else
		return QModelIndex();

}
