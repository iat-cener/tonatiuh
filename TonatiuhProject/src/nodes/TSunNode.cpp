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

#include "nf.h"
#include "TParameterList.h"
#include "TSunNode.h"

/******************************
 * TSunNode
 ******************************/

TNodeType TSunNode::m_nodeType = TNodeType::CreateEmptyType();


/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > TSunNode::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateTSunNode : public TSunNode { using TSunNode::TSunNode; };
	return ( std::make_unique<EnableCreateTSunNode>() );
}


/*!
 * Initializes TGroupNode type.
 */
void TSunNode::Init()
{
	m_nodeType = TNodeType::CreateType( TNodeType::FromName( "ContainerNode" ), "SunNode", &TSunNode::CreateInstance );
}

/*!
 * TGroupNode : public TNode
 */
TSunNode::TSunNode()
:TContainerNode(),
 m_azimuthLabel( "azimuth" ),
 m_disabledNodesLabel( "disabledNodes" ),
 m_zenithLabel( "zenith" )
{
	SetName( GetType().GetName() );

	//Parts
	AppendPart( "sunshape", TNodeType::FromName( "Sunshape" ) , 0 );


	//Sun coordinates
	m_pParametersList->Append<double>( m_azimuthLabel, 0.0, true );
	m_pParametersList->Append<double>( m_zenithLabel, 0.0, true );
	m_pParametersList->Append<std::string>( m_disabledNodesLabel, std::string( "" ), true );
}

/*!
 * Destructor.
 */
TSunNode::~TSunNode()
{
	SetPart( "sunshape", 0 );
}

/*!
 * Creates a copy of sun node.
 */
std::shared_ptr< TNode > TSunNode::Copy() const
{
	//shared_prt needs a public constructor
	struct EnableCreateTSunNode : public TSunNode { using TSunNode::TSunNode; };
	auto sunNode = std::make_unique<EnableCreateTSunNode>();

	 if( sunNode == 0 )	return ( 0  );

	 for (std::map<std::string, TNodeType>::iterator it = sunNode->m_partsTypeList.begin(); it!=sunNode->m_partsTypeList.end(); ++it)
	 {
		 std::string partName = it->first;
		 TNodeType partType = it->second;
		 std::shared_ptr<TNode> partNode = sunNode->m_partsList[partName];


		 sunNode->AppendPart( partName, partType, partNode->Copy() );
	 }


	 //Coping the parameters.
	 sunNode->m_pParametersList->SetValue( m_azimuthLabel, GetParameterValue<double>( m_azimuthLabel ) );
	 sunNode->m_pParametersList->SetValue( m_zenithLabel, GetParameterValue<double>( m_zenithLabel ) );

	 return ( sunNode );

 }


/*
 * Changes the sun node position to the coordinates defined by the angles \a azimuth and zenith. These angles are in radians.
 */
void TSunNode::ChangeSunPosition( double azimuth, double zenith )
{
	m_pParametersList->SetValue( m_azimuthLabel, azimuth );
	m_pParametersList->SetValue( m_zenithLabel, zenith );

	//emit SunpositonChanged( azimuth, zenith );

}

/*!
 * Returns azimuth value in radians.
 */
double TSunNode::GetAzimuth() const
{
	return ( GetParameterValue<double>( m_azimuthLabel ) );
}

/*!
 * Returns the url of the nodes that are not defined as first stage nodes.
 */
std::vector<std::string> TSunNode::GetDisabledNodes() const
{
	std::vector< std::string > disabledNodesList;

	std::string disabledNodes = GetParameterValue<std::string>( m_disabledNodesLabel );
	if( disabledNodes.empty() )	return ( disabledNodesList );

	disabledNodesList = nf::StringSplit( disabledNodes, ";" );
	return ( disabledNodesList );
}

/*!
 * Returns icon file name
 */
std::string TSunNode::GetIcon() const
{
	return ( ":/icons/tsunnode.png" );
}

/*!
 * Returns zenith angle value in radians.
 */
double TSunNode::GetZenith() const
{
	return( GetParameterValue<double>( m_zenithLabel ) );
}


/*!
 * Returns the type of node.
 */
TNodeType TSunNode::GetType() const
{
	return ( TSunNode::m_nodeType );
}

