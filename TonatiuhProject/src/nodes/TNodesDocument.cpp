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


#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QVariant>

#include "TContainerNode.h"
#include "TNode.h"
#include "TNodesDocument.h"
#include "TNodesList.h"
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
TNode* TNodesDocument::GetRootNode() const
{
	return ( m_pRootNode );
}

/*!
 * Reads the content of the file \a filename and set to the document.
 * Returns true if successful; otherwise returns false.
 */
bool TNodesDocument::Read( QString filename )
{
	QFile tonatiuhFile( filename );
	if( !tonatiuhFile.open( QIODevice::ReadOnly ) )	return (false);


	int errLine;
	QString errMsg;
	QDomDocument xmlDocument;
	if( xmlDocument.setContent(&tonatiuhFile, &errMsg, &errLine) == false)
	{
		qWarning() << "TNodesDocument: Error in line " << errLine << "\n" << errMsg;
	    return ( false );
	  }

	QDomElement docummentRootElement = xmlDocument.documentElement();
	if( docummentRootElement.tagName() !=  QLatin1String( "Tonatiuh" ) )
	{
		qWarning() << "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );
	}

	if( !docummentRootElement.hasChildNodes() )
	{
		qWarning() << "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );

	}

	QDomNodeList docChilds = docummentRootElement.childNodes();
	if( docChilds.size() != 1 )
	{
		qWarning() << "TNodesDocument: The file is not compatible with this version of Tonatiuh\n";
	    return ( false );
	}

	QDomElement rootElement = docChilds.at(0).toElement();


	QString rootErrMsg;
	m_pRootNode = CreateNodeObject( rootElement, &rootErrMsg );
	if( !m_pRootNode || m_pRootNode == 0 || !rootErrMsg.isEmpty() )
	{
		qWarning() << "TNodesDocument: error creating root node object. ";
		qWarning() << rootErrMsg;
	    return ( false );
	}
	m_pRootNode->IncreaseReference();


	QString readErrMsg;
	QDomNodeList rootChilds = rootElement.childNodes();
	if( rootChilds.size() < 1 )		return ( false );


	QMap< int, TNode* > readNodes;
	for( int c = 0; c < rootChilds.size(); c++ )
	{
		QDomElement childNode = rootChilds.at(c).toElement();
		if( !ReadNode( childNode, m_pRootNode, &readNodes, &readErrMsg ) )
		{
			qWarning() << readErrMsg;
			return ( false );
		}

	}


	return ( true );
}

/*!
 *	Sets document root node to \a node.
 */
void TNodesDocument::SetRootNode( TNode* node )
{
	m_pRootNode = node;
}

/*!
 * Writes to the file \a filename the information related to the node defined in the document.
 * Returns true if successful; otherwise returns false.
 */
bool TNodesDocument::Write( QString filename ) const
{
	QFile tonatiuhFile( filename );
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
		ok =  WriteConatinerNode(docummentRootElement, container, &writtenNodes );
	else
		ok = WriteNode(docummentRootElement, m_pRootNode, &writtenNodes );

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
TNode* TNodesDocument::CreateNodeObject( QDomElement node, QString* errMsg )
{
	TNodeType nodeType = TNodeType::FromName( node.attribute( QLatin1String( "type" ) ) );

	TNode* objectNode = (TNode*) nodeType.NodeFromType();
	if( !objectNode || objectNode == 0 )
	{
		*errMsg = QString( "TNodesDocument: error creating %1 node object." ).arg( node.tagName() );
	    return ( 0 );
	}


	objectNode->SetName( node.tagName() );
	QDomNamedNodeMap nodeAttributes = node.attributes();
	for( int att = 0; att < nodeAttributes.length(); att++ )
	{
		QString attributeName = nodeAttributes.item( att ).nodeName();
		if( ( attributeName != QLatin1String( "type" ) ) && ( attributeName != QLatin1String( "name" ) ) )
			objectNode->SetParameterValue( attributeName, node.attribute( attributeName ) );
	}

    return ( objectNode );
}

bool TNodesDocument::ReadNode( QDomElement node, TNode* parentNode, QMap< int, TNode* >* readNodes, QString* errMsg )
{


	if( node.hasAttribute( QLatin1String( "type" ) ) &&  node.hasAttribute( QLatin1String( "id" ) ) )  //node
	{
		int nodeID  = node.attribute( QLatin1String( "id" ) ).toInt();
		if( readNodes->contains( nodeID ) )
		{
			TNode* nodeObject = readNodes->value( nodeID );
			if( !nodeObject || nodeObject == 0 )	return ( false );

			return ( true );
		}
		else
		{

			TNode* nodeObject = CreateNodeObject( node, errMsg );
			if( !nodeObject || nodeObject == 0 )	return ( false );

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
		QString partName = node.tagName();
		TContainerNode* parentContainer = parentNode->as<TContainerNode>();
		if( !parentContainer || parentContainer == 0 )
		{
			*errMsg = "TNodesDocument::ReadNode error converting node in a container node.";
			return ( false );
		}
		QDomNodeList nodeChilds = node.childNodes();
		if( nodeChilds.size() < 1 )		return ( true );
		if( nodeChilds.size() > 1 )
		{
			*errMsg = QString( "TNodesDocument::ReadNode. More than one node defined for the container %1 node.").arg( partName );
			return ( false );
		}
		QDomElement partNode = nodeChilds.at(0).toElement();
		TNodeType partType = TNodeType::FromName( partNode.attribute( QLatin1String( "type" ) ) );
		if( partType.IsDerivedFrom( TNodeType::FromName( QLatin1String( "NodesList" ) ) ) )
		{
			TNodesList* listNode = parentContainer->GetPart( partName )->as<TNodesList>();
			if( !ReadNodeList( partNode, listNode, readNodes, errMsg ) )	return (false);

		}
		else
		{

			int nodeID  = partNode.attribute( QLatin1String( "id" ) ).toInt();
			if( readNodes->contains( nodeID ) )
			{
				TNode* nodeObject = readNodes->value( nodeID );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				parentContainer->SetPart( partName, nodeObject );
				//nodeObject->IncreaseReference();

				return ( true );
			}
			else
			{

				TNode* nodeObject = CreateNodeObject( partNode, errMsg );
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

bool TNodesDocument::ReadNodeList( QDomElement node, TNodesList* listNode, QMap< int, TNode* >* readNodes, QString* errMsg  )
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
				TNode* nodeObject = readNodes->value( nodeID );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				bool ok = listNode->InsertItem( nodeObject );
				//nodeObject->IncreaseReference();
				if( !ok )
				{
					*errMsg = QString( "The node %1 cannot be added as a child of %2." ).arg( itemNode.tagName(), listNode->GetName() );
					return ( false );
				}
				return ( true );
			}
			else
			{
				TNode* nodeObject = CreateNodeObject( itemNode, errMsg );
				if( !nodeObject || nodeObject == 0 )	return ( false );

				readNodes->insert( nodeID, nodeObject );


				bool ok = listNode->InsertItem( nodeObject );
				//nodeObject->IncreaseReference();
				if( !ok )
				{
					*errMsg = QString( "The node %1 cannot be added as a child of %2." ).arg( itemNode.tagName(), listNode->GetName() );
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
bool TNodesDocument::WriteNode( QDomElement parent, const TNode* node, QList< int >* writtenNodes ) const
{
	if( !node )	return (false);

	int nodeID = node->GetID();

	QDomElement el = parent.ownerDocument().createElement(node->GetName());
	el.setAttribute( QLatin1String( "type" ), node->GetType().GetName() );
	el.setAttribute( QLatin1String( "id" ), nodeID);
	parent.appendChild(el);

	if( writtenNodes->contains( nodeID ) )	return ( true );


	QStringList parametersList = node->GetVisibleParametersName();
	for( int p = 0; p < parametersList.count(); p++ )
	{

		QString parameterName = parametersList[p];
		QVariant parameterValue = node->GetParameterValue( parameterName);
		el.setAttribute( parameterName, parameterValue.toString() );
	}
	writtenNodes->append( nodeID );

	if( const TNodesList* listNode = node->as<TNodesList>() )
	{
		int numberOfChildren = listNode->Count();
		for( int l = 0; l < numberOfChildren; l++ )
		{
			TNode* childNode = listNode->Item( l );
			if( const TContainerNode* childNodeContainer = childNode->as<TContainerNode>() )
			{
				if( !WriteConatinerNode(el, childNodeContainer, writtenNodes ) ) 	return ( false );
			}
			else
				if( !WriteNode(el, childNode, writtenNodes) )	return ( false );
		}
	}
	return ( true );
}

/*!
 * Adds the as \a parent child the \a containerNode parameters and all the parts nodes
 */
bool TNodesDocument::WriteConatinerNode( QDomElement parent, const TContainerNode* containerNode, QList< int >* writtenNodes ) const
{
	if( !containerNode )	return (false);

	int nodeID = containerNode->GetID();

	QDomElement el = parent.ownerDocument().createElement(containerNode->GetName());
	el.setAttribute( QLatin1String( "type" ), containerNode->GetType().GetName() );
	el.setAttribute( QLatin1String( "id" ), nodeID );
	parent.appendChild(el);

	if( writtenNodes->contains( nodeID ) )	return ( true );

	QStringList parametersList = containerNode->GetVisibleParametersName();
	for( int p = 0; p < parametersList.count(); p++ )
	{

		QString parameterName = parametersList[p];
		QVariant parameterValue = containerNode->GetParameterValue( parameterName);
		el.setAttribute( parameterName, parameterValue.toString() );
	}
	writtenNodes->append( nodeID );

	int numberOfParts = containerNode->NumberOfParts();
	QStringList partNamesList = containerNode->GetPartNames();
	for( int p = 0; p < numberOfParts; p++ )
	{

		TNode* partNode = containerNode->GetPart( partNamesList[p] );
		if( partNode )
		{
			QDomElement partElement = el.ownerDocument().createElement(partNamesList[p]);
			el.appendChild(partElement);

			if( const TContainerNode* partContainer = partNode->as<TContainerNode>() )
			{
				if( !WriteConatinerNode(partElement, partContainer, writtenNodes ) ) return ( false );
			}
			else
				if( !WriteNode(partElement, partNode, writtenNodes ) ) return ( false );

		}

	}

	return ( true );
}
