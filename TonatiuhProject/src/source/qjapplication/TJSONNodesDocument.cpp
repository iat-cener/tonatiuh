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

#include <QDataStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

#include "TContainerNode.h"
#include "TNode.h"
#include "TJSONNodesDocument.h"
#include "TNodesList.h"
#include "TParameter.h"
#include "TParameterList.h"

/*!
 * Document constructor.
 */
TJSONNodesDocument::TJSONNodesDocument()
:m_pRootNode( 0 )
{

}

/*!
 * Destroys document object.
 */
TJSONNodesDocument::~TJSONNodesDocument()
{

}

/*!
 *	Returns the root node in the document.
 */
std::shared_ptr< TNode >  TJSONNodesDocument::GetRootNode() const
{
	return ( m_pRootNode );
}

/*!
 * Reads the content of the file \a filename and set to the document.
 * Returns true if successful; otherwise returns false.
 */
bool TJSONNodesDocument::Read( std::string filename )
{
	QFile tonatiuhFile( filename.c_str() );
	if( !tonatiuhFile.open( QIODevice::ReadOnly ) )
	{
		std::cerr<< "TJSONNodesDocument: Cannot find " << filename << "\n" <<std::endl;
		return (false);
	}


    QByteArray fileData = tonatiuhFile.readAll();
    QJsonDocument jsonDocument( QJsonDocument::fromJson(fileData) );
    QJsonObject documentRootObject = jsonDocument.object();

	if( documentRootObject.isEmpty() )
	{
		std::cerr<< "TJSONNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );
	}

	std::string readErrMsg;


	m_pRootNode = CreateNodeObject( documentRootObject, &readErrMsg );
	if( m_pRootNode == nullptr || !readErrMsg.empty() )
	{
		std::cerr<< "TJSONNodesDocument: error creating root node object. ";
		std::cerr<< readErrMsg;
	    return ( false );
	}



	std::shared_ptr< TContainerNode > container = std::dynamic_pointer_cast< TContainerNode > ( m_pRootNode );
	if( container == 0  || container == nullptr )	return ( true );


	QMap< int, std::shared_ptr< TNode > > readNodes;
	if( !ReadContainerNode( documentRootObject, container, &readNodes, &readErrMsg ) )
	{
		std::cerr<< readErrMsg;
		return ( false );
	}

   /* read(loadDoc.object());

    QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using "
                        << (saveFormat != Json ? "binary " : "") << "JSON...\n";
                        */
    return ( true );

}

/*!
 *	Sets document root node to \a node.
 */
void TJSONNodesDocument::SetRootNode( const std::shared_ptr< TNode >& node )
{
	m_pRootNode = node;
}

/*!
 * Writes to the file \a filename the information related to the node defined in the document.
 * Returns true if successful; otherwise returns false.
 */
bool TJSONNodesDocument::Write( std::string filename ) const
{

	QFile tonatiuhFile( filename.c_str() );
	if( !tonatiuhFile.open( QIODevice::WriteOnly ) )	return (false);


	if( !m_pRootNode || m_pRootNode == 0 )
	{
		tonatiuhFile.close();
		return (false);
	}

    QList< int > writtenNodes;
    QJsonObject documentRootObject;
	if( const TContainerNode* container = m_pRootNode->as<TContainerNode>() )
		documentRootObject =  WriteConatinerNode( *container, &writtenNodes ).toObject();
	else
		documentRootObject = WriteNode( *m_pRootNode, &writtenNodes ).toObject();

	if( documentRootObject.empty() )
	{
		tonatiuhFile.close();
		return (false);
	}

    QJsonDocument jsonDocument( documentRootObject );
    tonatiuhFile.write( jsonDocument.toJson() );

	return (true);

}

/*!
 * Reads the content of the node \a node and sets inside of the \a parentNode.
 */

std::shared_ptr<TNode > TJSONNodesDocument::CreateNodeObject( QJsonObject jsonObject, std::string* errMsg )
{
	TNodeType nodeType = TNodeType::FromName( jsonObject[ m_typeLabel ].toString().toStdString() );
	std::string nodeName = jsonObject[ m_nameLabel ].toString().toStdString();
	std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
	if( !objectNode || objectNode == nullptr )
	{

		*errMsg = std::string( "TJSONNodesDocument::CreateNodeObject. Error creating '" ) + nodeName +
				std::string(  "' node object." );
	    return ( 0 );
	}
	objectNode->SetName( nodeName );
	//std::cout<<"\t\t CreateNodeObject: "<<nodeName<<" type: "<<jsonObject[ m_typeLabel ].toString().toStdString()<<std::endl;

	std::vector< std::string > partNames;
	std::shared_ptr< TContainerNode > container = std::dynamic_pointer_cast< TContainerNode > ( objectNode );
	if( container != 0  && container != nullptr )
		partNames = container->GetPartNames();

	//std::cout<<"\t\t partNames "<<partNames.size()<<std::endl;
	QStringList nodeAttributes =  jsonObject.keys();
	for( int att = 0; att < nodeAttributes.count(); att++ )
	{
		std::string attributeName = nodeAttributes[ att ].toStdString();
		if( ( attributeName != m_typeLabel ) && ( attributeName != m_nameLabel )  && ( attributeName != m_idLabel ) && ( attributeName != m_nodeListLabel ) &&
				std::count( partNames.begin(), partNames.end(), attributeName ) < 1 )
		{
			std::string attributeValue = jsonObject[ attributeName.c_str() ].toString().toStdString();

			//std::cout<<"\t\t\t"<<attributeName<<" "<<attributeValue<<std::endl;
			if( !objectNode->SetParameterFormString( attributeName, attributeValue ) )
			{
				*errMsg= std::string("TJSONNodesDocument::CreateNodeObject. Error defining the attribute '" ) + nodeName +
					std::string( "' of node '" ) + attributeName +
					std::string( "' and value '") + attributeValue + std::string("'." );
			    return ( 0 );
			}

		}

	}

    return ( objectNode );
}
/*!
 * Reads
 */
bool TJSONNodesDocument::ReadContainerNode( QJsonObject jsonContainerNode, std::shared_ptr< TContainerNode > parentTNode, QMap< int, std::shared_ptr< TNode > >* readNodes, std::string* errMsg )
{
	if( !parentTNode || parentTNode == 0  || parentTNode == nullptr )
	{
		*errMsg = "TJSONNodesDocument::ReadContainerNode cannot read null node.";
		return ( false );
	}

	std::vector< std::string > partNames = parentTNode->GetPartNames();
	for( unsigned int c = 0; c < partNames.size(); c++ )
	{
		if( jsonContainerNode.contains( partNames[c].c_str() ) )
		{
			QJsonObject partJSONObject = jsonContainerNode[ partNames[c].c_str() ].toObject();
			int nodeID  = partJSONObject[ m_idLabel ].toInt();
			if( readNodes->contains( nodeID ) )
			{
				std::shared_ptr< TNode > nodeObject = readNodes->value( nodeID );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				bool ok = parentTNode->SetPart( partNames[c], nodeObject );
				if( !ok )
				{
					*errMsg = std::string( "TJSONNodesDocument::ReadContainerNode. The node ") + nodeObject->GetName() +
							std::string( " cannot be added as a part of " ) + partNames[c] +
							std::string( "." ) ;

					return ( false );
				}
			}
			else
			{

				if( partJSONObject.contains( m_nodeListLabel ) )
				{

					std::shared_ptr< TNodesList > listNode = std::dynamic_pointer_cast<TNodesList>( parentTNode->GetPart( partNames[c] ) );
					if( !listNode  || !ReadNodeList( partJSONObject[m_nodeListLabel].toArray(), listNode, readNodes, errMsg ) )
					{
						//The error is defined in ReadNodeList function.  *errMsg = "TJSONNodesDocument::ReadContainerNode m_nodeListLabel.";
						return ( false );
					}

				}
				else
				{

					std::shared_ptr< TNode > nodeObject = CreateNodeObject( partJSONObject, errMsg );
					if( !nodeObject || nodeObject == 0 )	return ( false );
					readNodes->insert( nodeID, nodeObject );
					parentTNode->SetPart( partNames[c], nodeObject );

					std::shared_ptr< TContainerNode > containerChildNode = std::dynamic_pointer_cast< TContainerNode > ( nodeObject );
					if( containerChildNode != 0  && containerChildNode != nullptr &&
							!ReadContainerNode( partJSONObject, containerChildNode, readNodes, errMsg ) )
						return ( false );
				}

			}
		}

	}

	return ( true );
}


/*!
 * Reads the children of the list \a node and insert into \a listNode.
 * Returns true if there is not errors during read process, otherwise returns false and a message in \a errMsg.
 */
bool TJSONNodesDocument::ReadNodeList( QJsonArray jsonArray, std::shared_ptr< TNodesList > listNode, QMap< int, std::shared_ptr< TNode > >* readNodes, std::string* errMsg  )
{

	if( jsonArray.count() < 1 )		return ( true );
	for( int c = 0; c < jsonArray.count(); c++ )
	{
		QJsonObject jsonNode = jsonArray[c].toObject();
		if( jsonNode.isEmpty() )	return ( false );
		int nodeID  = jsonNode[ m_idLabel ].toInt();

		if( readNodes->contains( nodeID ) )
		{
			std::shared_ptr< TNode > nodeObject = readNodes->value( nodeID );
			if( !nodeObject || nodeObject == nullptr )	return ( false );

			bool ok = listNode->InsertItem( nodeObject );
			if( !ok )
			{
				*errMsg = std::string( "TJSONNodesDocument::ReadNodeList. The node ") +nodeObject->GetName() +
						std::string( " cannot be added as a child of " ) + listNode->GetName() +
						std::string( "." ) ;

				return ( false );
			}
		}
		else
		{
			std::shared_ptr< TNode > nodeObject = CreateNodeObject( jsonNode, errMsg );
			if( !nodeObject || nodeObject == 0 )	return ( false );

			std::shared_ptr< TContainerNode > containerChildNode = std::dynamic_pointer_cast< TContainerNode > ( nodeObject );
			if( containerChildNode != 0  && containerChildNode != nullptr &&
					!ReadContainerNode( jsonNode, containerChildNode, readNodes, errMsg ) )
				return ( false );


			readNodes->insert( nodeID, nodeObject );
			bool ok = listNode->InsertItem( nodeObject );
			if( !ok )
			{
				*errMsg = std::string( "TJSONNodesDocument::ReadNodeList. The node ") +nodeObject->GetName() +
						std::string( " cannot be added as a child of " ) + listNode->GetName() +
						std::string( "." ) ;
				return ( false );
			}
		}
	}

	return (true);

}

/*!
 * Adds the \a node parameters as a \a parent child
 */
QJsonValue TJSONNodesDocument::WriteNode( const TNode& node, QList< int >* writtenNodes ) const
{

	int nodeID = node.GetID();

	QJsonObject jsonObject;
	jsonObject[ m_typeLabel ] = node.GetType().GetName().c_str();
	jsonObject[ m_idLabel ] = nodeID;
	jsonObject[ m_nameLabel ] = node.GetName().c_str();

	if( writtenNodes->contains( nodeID ) )	return ( QJsonValue::Undefined );

	writtenNodes->append( nodeID );

	//Write parameters
	std::vector< std::string > parametersList = node.GetVisibleParametersName();
	for( unsigned int p = 0; p < parametersList.size(); p++ )
	{
		std::string parameterName = parametersList[p];
		std::string parameterValue = node.GetParameterToString( parameterName );
		jsonObject[ parameterName.c_str() ] = parameterValue.c_str();
	}

	if( const TNodesList* listNode = node.as<TNodesList>() )
	{
		int numberOfChildren = listNode->Count();
		QJsonArray jsonArray;
		for( int l = 0; l < numberOfChildren; l++ )
		{
			std::shared_ptr< TNode > childNode = listNode->Item( l );
			QJsonValue listObject;
			if( const TContainerNode* childNodeContainer = childNode->as<TContainerNode>() )
				listObject = WriteConatinerNode( *childNodeContainer, writtenNodes );

			else
				listObject = WriteNode(*childNode, writtenNodes );

			if( !listObject.isUndefined() )
				jsonArray.append( listObject.toObject() );
			else
				return ( QJsonValue::Undefined );

		}
		jsonObject[ m_nodeListLabel ] = jsonArray;
	}

	return ( jsonObject );
}

/*!
 * Adds the as \a parent child the \a containerNode parameters and all the parts nodes
 */
QJsonValue TJSONNodesDocument::WriteConatinerNode( const TContainerNode& containerNode, QList< int >* writtenNodes ) const
{
	int nodeID = containerNode.GetID();

	QJsonObject jsonObject;
	jsonObject[ m_typeLabel ] = containerNode.GetType().GetName().c_str();
	jsonObject[ m_idLabel ] = nodeID;
	jsonObject[ m_nameLabel ] = containerNode.GetName().c_str();

	if( writtenNodes->contains( nodeID ) )	return ( jsonObject);
	writtenNodes->append( nodeID );

	std::vector<std::string> parametersList = containerNode.GetVisibleParametersName();
	for( unsigned int p = 0; p < parametersList.size(); p++ )
	{
		std::string parameterName = parametersList[p];
		std::string parameterValue = containerNode.GetParameterToString( parameterName );
		jsonObject[ parameterName.c_str() ] = parameterValue.c_str();
	}

	int numberOfParts = containerNode.NumberOfParts();
	std::vector<std::string> partNamesList = containerNode.GetPartNames();
	for( int p = 0; p < numberOfParts; p++ )
	{

		std::shared_ptr< TNode > partNode = containerNode.GetPart( partNamesList[p] );
		if( partNode )
		{

			QJsonValue listObject = QJsonValue::Undefined;
			if( const TContainerNode* partContainer = partNode->as<TContainerNode>() )
				listObject = WriteConatinerNode( *partContainer, writtenNodes );
			else
				listObject = WriteNode( *partNode, writtenNodes );

			if( !listObject.isUndefined() )
				jsonObject[ partNamesList[p].c_str()  ] = listObject ;
			else
				return ( QJsonValue::Undefined );

		}

	}

	return ( jsonObject );
}
