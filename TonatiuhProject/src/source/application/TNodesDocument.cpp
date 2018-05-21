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
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMap>

#include "TContainerNode.h"
#include "TNode.h"
#include "TNodesDocument.h"
#include "TNodesList.h"
#include "TParameter.h"
#include "TParameterList.h"

/*!
 * Document constructor.
 */
TNodesDocument::TNodesDocument()
:m_pRootNode( 0 )
{

}

/*!
 * Destroys document object.
 */
TNodesDocument::~TNodesDocument()
{

}

/*!
 *	Returns the root node in the document.
 */
std::shared_ptr< TNode >  TNodesDocument::GetRootNode() const
{
	return ( m_pRootNode );
}

/*!
 * Reads the content of the file \a filename and set to the document.
 * Returns true if successful; otherwise returns false.
 */
bool TNodesDocument::Read( std::string filename )
{
	QFile tonatiuhFile( filename.c_str() );
	if( !tonatiuhFile.open( QIODevice::ReadOnly ) )
	{
		std::cerr<< "TNodesDocument: Cannot find " << filename << "\n" <<std::endl;
		return (false);
	}


	int errLine;
	QString errMsg;
	QDomDocument xmlDocument;
	if( xmlDocument.setContent(&tonatiuhFile, &errMsg, &errLine) == false)
	{
		std::cerr<< "TNodesDocument: Error in line " << errLine << "\n" << errMsg.toStdString()<<std::endl;
		return ( false );
	}

	QDomElement docummentRootElement = xmlDocument.documentElement();
	if( docummentRootElement.tagName() !=  QLatin1String( "Tonatiuh" ) )
	{
		std::cerr<< "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );
	}

	if( !docummentRootElement.hasChildNodes() )
	{
		std::cerr<< "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );

	}

	QDomNodeList docChilds = docummentRootElement.childNodes();
	if( docChilds.size() != 1 )
	{
		std::cerr<< "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );
	}

	QDomElement rootElement = docChilds.at(0).toElement();
	std::string rootErrMsg;
	m_pRootNode = CreateNodeObject( rootElement, &rootErrMsg );
	if( m_pRootNode == nullptr || !rootErrMsg.empty() )
	{
		std::cerr<< "TNodesDocument: error creating root node object. ";
		std::cerr<< rootErrMsg;
	    return ( false );
	}
	//m_pRootNode->IncreaseReference();

	std::string readErrMsg;
	QDomNodeList rootChilds = rootElement.childNodes();
	if( rootChilds.size() < 1 )		return ( false );


	QMap< int, std::shared_ptr< TNode > > readNodes;
	for( int c = 0; c < rootChilds.size(); c++ )
	{
		QDomElement childNode = rootChilds.at(c).toElement();

		 if( !ReadNode( childNode, m_pRootNode, &readNodes, &readErrMsg ) )
		{
			std::cerr<< readErrMsg;
			return ( false );
		}
	}
	return ( true );
}

/*!
 *	Sets document root node to \a node.
 */
void TNodesDocument::SetRootNode( const std::shared_ptr< TNode >& node )
{
	m_pRootNode = node;
}

/*!
 * Writes to the file \a filename the information related to the node defined in the document.
 * Returns true if successful; otherwise returns false.
 */
bool TNodesDocument::Write( std::string filename ) const
{

	QFile tonatiuhFile( filename.c_str() );
	if( !tonatiuhFile.open( QIODevice::WriteOnly ) )	return (false);

	QDomDocument xmlDocument;


	QDomProcessingInstruction xmlDeclaration = xmlDocument.createProcessingInstruction(QLatin1String("xml"), QLatin1String("version=\"1.0\" encoding=\"UTF-8\""));
	xmlDocument.appendChild(xmlDeclaration);

	if( !m_pRootNode || m_pRootNode == 0 )
	{
		tonatiuhFile.close();
		return (false);
	}

	QDomElement docummentRootElement = xmlDocument.createElement(QLatin1String( "Tonatiuh") );
	xmlDocument.appendChild(docummentRootElement);

	QList< int > writtenNodes;
	bool ok = false;
	if( const TContainerNode* container = m_pRootNode->as<TContainerNode>() )
		ok =  WriteConatinerNode(docummentRootElement, *container, &writtenNodes );
	else
		ok = WriteNode(docummentRootElement, *m_pRootNode, &writtenNodes );

	if( !ok )
	{
		tonatiuhFile.close();
		return (false);
	}



	QByteArray arr = xmlDocument.toByteArray(2);
	if( tonatiuhFile.write(arr) != arr.size() )
		return (false);


	return (true);

}

/*!
 * Reads the content of the node \a node and sets inside of the \a parentNode.
 */
std::shared_ptr<TNode > TNodesDocument::CreateNodeObject( QDomElement node, std::string* errMsg )
{

	TNodeType nodeType = TNodeType::FromName( node.attribute( "type" ).toStdString() );
	std::shared_ptr<TNode> objectNode = nodeType.NodeFromType();
	if( !objectNode || objectNode == 0 )
	{
		*errMsg = std::string( "TNodesDocument::CreateNodeObject. Error creating '" ) +  node.tagName().toStdString() +
				std::string(  "' node object." );
	    return ( 0 );
	}

	objectNode->SetName( node.tagName().toStdString() );
	QDomNamedNodeMap nodeAttributes = node.attributes();
	for( int att = 0; att < nodeAttributes.length(); att++ )
	{
		std::string attributeName = nodeAttributes.item( att ).nodeName().toStdString();
		if( ( attributeName != "type" ) && ( attributeName != "name" )  && ( attributeName != "id" ))
		{
			std::string parameterValue = node.attribute( attributeName.c_str() ).toStdString();
			if( !objectNode->SetParameterFormString( attributeName, parameterValue ) )
			{
				*errMsg= std::string("TNodesDocument::CreateNodeObject. Error defining the attribute '" ) + node.tagName().toStdString() +
						std::string( "' of node '" ) + attributeName +
						std::string( "' and value '") + parameterValue + std::string("'." );

			    return ( 0 );
			}
		}
	}

    return ( objectNode );
}


bool TNodesDocument::ReadNode( QDomElement node, std::shared_ptr< TNode > parentNode, QMap< int, std::shared_ptr< TNode > >* readNodes, std::string* errMsg )
{
	if( node.hasAttribute( QLatin1String( "type" ) ) &&  node.hasAttribute( QLatin1String( "id" ) ) )  //node
	{
		int nodeID  = node.attribute( QLatin1String( "id" ) ).toInt();
		if( readNodes->contains( nodeID ) )
		{
			std::shared_ptr< TNode > nodeObject = readNodes->value( nodeID );
			if( !nodeObject || nodeObject == nullptr )	return ( false );
			return ( true );
		}
		else
		{

			std::shared_ptr< TNode > nodeObject = CreateNodeObject( node, errMsg );
			if( !nodeObject || nodeObject == nullptr )	return ( false );

			QDomNodeList children = node.childNodes();
			if( children.size() < 1 ) return ( true );

			for( int c = 0; c < children.size(); c++ )
			{
				QDomElement childNode = children.at(c).toElement();
				if( !ReadNode( childNode, nodeObject, readNodes, errMsg ) )	return ( false );

			}
			readNodes->insert( nodeID, nodeObject );

			return (true);
		}

	}
	else if( !node.hasAttributes() )	//part name. This node has not attributes
	{
		std::string partName = node.tagName().toStdString();
		//TContainerNode* parentContainer = parentNode->as<TContainerNode>();
		std::shared_ptr< TContainerNode > parentContainer = std::dynamic_pointer_cast< TContainerNode > ( parentNode );

		if( !parentContainer || parentContainer == 0  || parentContainer == nullptr )
		{
			*errMsg = "TNodesDocument::ReadNode error converting node in a container node.";
			return ( false );
		}
		QDomNodeList nodeChilds = node.childNodes();
		if( nodeChilds.size() < 1 )		return ( true );
		if( nodeChilds.size() > 1 )
		{
			*errMsg = std::string("TNodesDocument::ReadNode. More than one node defined for the container ") + partName +
					std::string( " node." );
			return ( false );
		}
		QDomElement partNode = nodeChilds.at(0).toElement();
		TNodeType partType = TNodeType::FromName( partNode.attribute( "type" ).toStdString() );
		if( partType.IsDerivedFrom( TNodeType::FromName( "NodesList" ) ) )
		{
			//TNodesList* listNode = parentContainer->GetPart( partName )->as<TNodesList>();
			std::shared_ptr< TNodesList > listNode = std::dynamic_pointer_cast<TNodesList>(  parentContainer->GetPart( partName ) );
			if( !listNode  || !ReadNodeList( partNode, listNode, readNodes, errMsg ) )	return (false);

		}
		else
		{
			int nodeID  = partNode.attribute( QLatin1String( "id" ) ).toInt();
			if( readNodes->contains( nodeID ) )
			{
				std::shared_ptr< TNode > nodeObject = readNodes->value( nodeID );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				parentContainer->SetPart( partName, nodeObject );
				//nodeObject->IncreaseReference();

				return ( true );
			}
			else
			{
				std::shared_ptr< TNode > nodeObject = CreateNodeObject( partNode, errMsg );
				if( !nodeObject || nodeObject == 0 )	return ( false );
				readNodes->insert( nodeID, nodeObject );
				parentContainer->SetPart( partName, nodeObject );
				//nodeObject->IncreaseReference();

				QDomNodeList children = partNode.childNodes();
				if( children.size() < 1 ) return ( true );

				for( int c = 0; c < children.size(); c++ )
				{
					QDomElement childNode = children.at(c).toElement();
					if( !ReadNode( childNode, nodeObject, readNodes, errMsg ) )	return ( false );

				}
			}
		}

		return (true);
	}

	return (false);

}


/*!
 * Reads the children of the list \a node and insert into \a listNode.
 * Returns true if there is not errors during read process, otherwise returns false and a message in \a errMsg.
 */

bool TNodesDocument::ReadNodeList( QDomElement node, std::shared_ptr< TNodesList > listNode, QMap< int, std::shared_ptr< TNode > >* readNodes, std::string* errMsg  )
{

	QDomNodeList partChilds = node.childNodes();
	if( partChilds.size() < 1 )		return ( true );
	for( int c = 0; c < partChilds.size(); c++ )
	{

		QDomElement itemNode = partChilds.at(c).toElement();
		if( itemNode.hasAttribute( QLatin1String( "type" ) ) ) //node
		{

			int nodeID  = itemNode.attribute( QLatin1String( "id" ) ).toInt();
			if( readNodes->contains( nodeID ) )
			{

				std::shared_ptr< TNode > nodeObject = readNodes->value( nodeID );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				bool ok = listNode->InsertItem( nodeObject );
				//nodeObject->IncreaseReference();
				if( !ok )
				{
					*errMsg = std::string( "TNodesDocument::ReadNodeList. The node ") + itemNode.tagName().toStdString() +
							std::string( " cannot be added as a child of " ) + listNode->GetName() +
							std::string( "." ) ;

					return ( false );
				}
				//return ( true );
			}
			else
			{
				std::shared_ptr< TNode > nodeObject = CreateNodeObject( itemNode, errMsg );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				readNodes->insert( nodeID, nodeObject );
				bool ok = listNode->InsertItem( nodeObject );

				//nodeObject->IncreaseReference();
				if( !ok )
				{
					*errMsg = std::string( "TNodesDocument::ReadNodeList. The node ") + itemNode.tagName().toStdString() +
							std::string( " cannot be added as a child of " ) + listNode->GetName() +
							std::string( "." ) ;

					return ( false );
				}
				QDomNodeList children = itemNode.childNodes();
				if( children.size() < 1 ) return ( true );

				for( int c = 0; c < children.size(); c++ )
				{
					QDomElement childNode = children.at(c).toElement();
					if( !ReadNode( childNode, nodeObject, readNodes, errMsg ) )	return ( false );

				}
			}
		}
	}
	return (true);
}

/*!
 * Adds the \a node parameters as a \a parent child
 */
bool TNodesDocument::WriteNode( QDomElement parent, const TNode& node, QList< int >* writtenNodes ) const
{
	int nodeID = node.GetID();

	QDomElement el = parent.ownerDocument().createElement( node.GetName().c_str() );
	el.setAttribute( "type", node.GetType().GetName().c_str() );
	el.setAttribute(  "id", nodeID );
	parent.appendChild(el);

	if( writtenNodes->contains( nodeID ) )	return ( true );


	std::vector< std::string > parametersList = node.GetVisibleParametersName();
	for( unsigned int p = 0; p < parametersList.size(); p++ )
	{
		std::string parameterName = parametersList[p];
		std::string parameterValue = node.GetParameterToString( parameterName );
		el.setAttribute( parameterName.c_str(), parameterValue.c_str() );
	}
	writtenNodes->append( nodeID );

	if( const TNodesList* listNode = node.as<TNodesList>() )
	{
		int numberOfChildren = listNode->Count();
		for( int l = 0; l < numberOfChildren; l++ )
		{
			std::shared_ptr< TNode > childNode = listNode->Item( l );
			if( const TContainerNode* childNodeContainer = childNode->as<TContainerNode>() )
			{
				if( !WriteConatinerNode(el, *childNodeContainer, writtenNodes ) ) 	return ( false );
			}
			else
				if( !WriteNode(el, *childNode, writtenNodes) )	return ( false );
		}
	}
	return ( true );
}

/*!
 * Adds the as \a parent child the \a containerNode parameters and all the parts nodes
 */
bool TNodesDocument::WriteConatinerNode( QDomElement parent, const TContainerNode& containerNode, QList< int >* writtenNodes ) const
{
	int nodeID = containerNode.GetID();

	QDomElement el = parent.ownerDocument().createElement(containerNode.GetName().c_str() );
	el.setAttribute( "type", containerNode.GetType().GetName().c_str() );
	el.setAttribute( "id", nodeID );
	parent.appendChild(el);

	if( writtenNodes->contains( nodeID ) )	return ( true );

	writtenNodes->append( nodeID );
	std::vector<std::string> parametersList = containerNode.GetVisibleParametersName();

	for( unsigned int p = 0; p < parametersList.size(); p++ )
	{
		std::string parameterName = parametersList[p];
		std::string parameterValue = containerNode.GetParameterToString( parameterName );
		el.setAttribute( parameterName.c_str(), parameterValue.c_str() );
	}

	int numberOfParts = containerNode.NumberOfParts();
	std::vector<std::string> partNamesList = containerNode.GetPartNames();
	for( int p = 0; p < numberOfParts; p++ )
	{
		std::shared_ptr< TNode > partNode = containerNode.GetPart( partNamesList[p] );
		if( partNode )
		{
			QDomElement partElement = el.ownerDocument().createElement( partNamesList[p].c_str() );
			el.appendChild(partElement);

			if( const TContainerNode* partContainer = partNode->as<TContainerNode>() )
			{
				if( !WriteConatinerNode(partElement, *partContainer, writtenNodes ) ) return ( false );
			}
			else
				if( !WriteNode(partElement, *partNode, writtenNodes ) ) return ( false );

		}

	}
	return ( true );
}
