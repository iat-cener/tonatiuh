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


#include <cassert>
#include <string>
#include <vector>

#include "TNodeType.h"




/*******************************
 * TTypeData
 *******************************/
/*
 * Internal structure to save type parameters
 */
struct TTypeData
{
	TTypeData( const std::string theName,
			const TNodeType theType = TNodeType::CreateEmptyType(),
			const TNodeType theParent = TNodeType::CreateEmptyType(),
			const TNodeType::ConstructorMethod theConstructorMethod = 0  )
	: name(theName),
	  type(theType),
	  parentType(theParent),
	  constructorMethod( theConstructorMethod )
	{

	};

	std::string name;
	TNodeType type;
	TNodeType parentType;
	TNodeType::ConstructorMethod constructorMethod;
};


/*******************************
 * TNodeType
 *******************************/

//std::vector< std::unique_ptr< TTypeData > >  TNodeType::m_typeList( 0 );
std::vector< TTypeData >  TNodeType::m_typeList;

/*!
 * Returns true if a type with \a name exists. Otherwise returns false.
 */
bool TNodeType::Contains(const std::string& name)
{

	for( unsigned int i = 0; i < TNodeType::m_typeList.size(); i++ )
	{
		//if( TNodeType::m_typeList[i]->name == name )
		if( TNodeType::m_typeList[i].name == name )
				return (true );
	}
	return (false);

}

/*!
 * Creates an empty type, for not valid types.
 */
const TNodeType TNodeType::CreateEmptyType()
{
	TNodeType emptyType;
	emptyType.m_index = -1;
	return (emptyType);
}

/*!
 * Creates a TNodeType object.
 */
const TNodeType TNodeType::CreateType(const TNodeType parent, const std::string name,
        const ConstructorMethod method )
{

	std::string msg = std::string( "CreateType: The node name ") + name + std::string ( " already exists in the database." );
	assert(!TNodeType::Contains( name ) && msg.c_str() );


	TNodeType newType;
	newType.m_index = TNodeType::m_typeList.size();
	std::unique_ptr< TTypeData > typeData = std::make_unique<TTypeData> (name, newType, parent, method);
	//TNodeType::m_typeList.push_back(std::move( typeData) );
	TNodeType::m_typeList.push_back( *typeData );

	return ( newType );

}

/*!
 * Returns the node type with already defined with the \a name.
 * If there is not a type with \a name, am empty type will be returned.
 */
TNodeType TNodeType::FromName( const std::string name )
{

	for( unsigned int i = 0; i < TNodeType::m_typeList.size(); i++ )
	{
		//if( TNodeType::m_typeList[i]->name == name )
		//	return ( TNodeType::m_typeList[i]->type );
		if( TNodeType::m_typeList[i].name == name )
			return ( TNodeType::m_typeList[i].type );
	}

	return ( CreateEmptyType() );

}

/*!
 * Initializes types system.
 */
/*
void TNodeType::Init()
{
	assert(TNodeType::m_typeList.size() < 0 && "Error initializing node types list" );

	//TNodeType::m_typeList = std::vector< std::unique_ptr< TTypeData> >  (CreateEmptyType());
	//TNodeType::m_typeList.resize( 10 );
}
*/

/*!
 * Name of the type.
 */
std::string TNodeType::GetName() const
{
	//return ( TNodeType::m_typeList[m_index]->name );
	return ( TNodeType::m_typeList[m_index].name );
}

/*!
 * Returns the type of the parent
 */
const TNodeType TNodeType::GetParent() const
{
	//return ( TNodeType::m_typeList[m_index]->parentType );
	return ( TNodeType::m_typeList[m_index].parentType );
}


/*!
 * Returns if the type of the object is derived of \a type.
 */
bool TNodeType::IsDerivedFrom(const TNodeType type) const
{
	int parentIndex = m_index;
	while( parentIndex > 0 && type.m_index != parentIndex )
	{
		//TNodeType parentType = TNodeType::m_typeList[parentIndex]->parentType;
		TNodeType parentType = TNodeType::m_typeList[parentIndex].parentType;
		parentIndex = parentType.m_index;
	}

	if( type.m_index == parentIndex )	return ( true );
	return ( false );
}

/*!
 * Checks if the type is a valid. Returns true if it is a valid type.
 */
bool TNodeType::IsValid() const
{
	if( m_index >= 0 )	return ( true );
	return ( false );
}

/*!
 * Creates a node object from the current type.
 *
 */
std::shared_ptr< TNode > TNodeType::NodeFromType() const
{
	//TNodeType::ConstructorMethod method = TNodeType::m_typeList[m_index]->constructorMethod;
	TNodeType::ConstructorMethod method = TNodeType::m_typeList[m_index].constructorMethod;
	return ( method() );
}

/*!
 * Compares if the types are equal.
 */
bool TNodeType::operator==( const TNodeType type )
{
	if(m_index == type.m_index)	return ( true );

	return (false);
}
