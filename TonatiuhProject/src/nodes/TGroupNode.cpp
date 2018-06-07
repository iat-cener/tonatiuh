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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/


#include "gc.h"
#include "Vector3D.h"

#include "nf.h"
#include "TGroupNode.h"
#include "TNodesList.h"
#include "TParameterList.h"
#include "Transform.h"

/******************************
 * TGroupNode
 ******************************/

TNodeType TGroupNode::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > TGroupNode::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateTGroupNode : public TGroupNode { using TGroupNode::TGroupNode; };
	return ( std::make_unique<EnableCreateTGroupNode>() );
}

/*!
 * Initializes TGroupNode type.
 */
void TGroupNode::Init()
{
	m_nodeType = TNodeType::CreateType( TNodeType::FromName( "ContainerNode" ), "GroupNode", &TGroupNode::CreateInstance );
}

/*!
 * TGroupNode : public TNode
 */
TGroupNode::TGroupNode()
:TContainerNode(),
 m_rotationName( "rotation"),
 m_scaleFactorName( "scaleFactor" ),
 m_translationName( "translation" )
{
	SetName(GetType().GetName() );

	//Parts
	//const std::shared_ptr< TNode >& childrenListNode = std::move( TNodesList::CreateInstance( ) );

	auto childrenListNode =  std::dynamic_pointer_cast<TNodesList>( std::move( TNodesList::CreateInstance( ) )  );


	childrenListNode->AddValidNodeType( TNodeType::FromName( "GroupNode") ) ;
	childrenListNode->AddValidNodeType( TNodeType::FromName( "SurfaceNode") ) ;
	childrenListNode->AddValidNodeType( TNodeType::FromName( "TrackerNode") ) ;
	AppendPart( "childrenList", TNodeType::FromName( "NodesList" ) , std::move( childrenListNode ) );



	//Transformation
	m_pParametersList->Append<std::string>( m_translationName, "0 0 0", true );
	m_pParametersList->Append<std::string>( m_rotationName, "0 0 1 0", true );
	m_pParametersList->Append<std::string>( m_scaleFactorName, "1 1 1", true );
}

/*!
 * Destructor.
 */
TGroupNode::~TGroupNode()
{
	SetPart( "childrenList", 0 );
}

/*!
 * Creates a copy of group node.
 */
std::shared_ptr< TNode > TGroupNode::Copy() const
 {
	//smart pointer->public constructor
	struct EnableCreateTGroupNode : public TGroupNode { using TGroupNode::TGroupNode; };
	auto groupNode = std::make_unique<EnableCreateTGroupNode>();

	 //TGroupNode* groupNode = new TGroupNode;
	 if( groupNode == 0 )	return ( 0  );

	 for (std::map<std::string, TNodeType>::iterator it = groupNode->m_partsTypeList.begin(); it!=groupNode->m_partsTypeList.end(); ++it)
	 {
		 std::string partName = it->first;
		 TNodeType partType = it->second;
		 std::shared_ptr< TNode > partNode = groupNode->m_partsList[partName];

		 groupNode->AppendPart( partName, partType, partNode->Copy() );
	 }


	 //Coping the parameters.
	 groupNode->m_pParametersList->SetValue( m_translationName, GetParameterValue<std::string>( m_translationName ) );
	 groupNode->m_pParametersList->SetValue( m_rotationName, GetParameterValue<std::string>( m_rotationName ) );
	 groupNode->m_pParametersList->SetValue( m_scaleFactorName, GetParameterValue<std::string>( m_scaleFactorName ) );

	 return ( groupNode );
 }

/*!
 * Returns icon file name
 */
std::string TGroupNode::GetIcon() const
{
	return ( ":/icons/tgroupnode.png" );
}

/*!
 * Returns the object to world transformation for the parameters defined for this node.
 */
Transform TGroupNode::GetTrasformation() const
{
	std::vector<std::string> parametersList = GetVisibleParametersName();
	if( parametersList.empty() )	return ( Transform() );


	std::vector< std::string > translationValues = nf::StringSplit( GetParameterValue<std::string>( m_translationName ) , "\\s+" );
	if( translationValues.size() != 3 ) 	return ( Transform() );
	Transform translation = Translate( stod( translationValues[0] ), stod( translationValues[1] ), stod( translationValues[2] )  );

	std::vector< std::string >  rotationValues = nf::StringSplit( GetParameterValue<std::string>( m_rotationName ), "\\s+" );
	if( rotationValues.size() != 4 ) return ( Transform() );
	Vector3D rotationAxis( stod( rotationValues[0] ), stod( rotationValues[1] ), stod( rotationValues[2] ) );
	if( fabs( 1.0 - rotationAxis.length() ) > gc::Epsilon  )	return ( Transform() );
	Transform rotation = Rotate( stod( rotationValues[3] ), rotationAxis   );

	std::vector< std::string >  scaleValues = nf::StringSplit( GetParameterValue<std::string>( m_scaleFactorName), "\\s+" );
	if( scaleValues.size() != 3 ) return ( Transform() );
	Transform scale = Scale( stod( scaleValues[0] ), stod( scaleValues[1] ), stod( scaleValues[2] ) );

	//First scaled, the rotated and finally is translated
	Transform transformOTW = translation*rotation*scale;

	return ( transformOTW );
}

/*!
 * Returns the type of node.
 */
TNodeType TGroupNode::GetType() const
{
	return ( TGroupNode::m_nodeType );
}
