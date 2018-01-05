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

#ifndef TNODE_H_
#define TNODE_H_

#include <memory>
#include <string>

#include "NodeLibrary.h"
#include "TNodeType.h"
#include "TParameterList.h"


//!  TNode class is the base class for all Tonatiuh nodes.
/*!
  TNode class defines all the basic parameters and methods for the nodes to define a concentration system.
*/

/** Base class of all Tonatiuh nodes: TNode */
class NODE_API TNode
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	TNode(const TNode& node) = delete;
	TNode& operator=(const TNode&) = delete;

public:
	virtual ~TNode() noexcept;
	static void Init();

	template<class T> const T* as() const;
	template<class T> T* as();

	//TNode* CopyNode() const;
	virtual TNode* Copy() const = 0;

	int GetID() const;
	std::string GetName() const;
	virtual std::string GetIcon() const;
	template<class T> T GetParameterValue( std::string name ) const;
	std::string GetParameterToString( std::string name ) const;
	int GetReferences() const;
	virtual TNodeType GetType() const;
	std::vector<std::string> GetVisibleParametersName() const;

	void IncreaseReference();
	void RemoveReference();

	void SetName( std::string name );
	template<class T> bool SetParameterValue( const std::string& name, const T& value );
	bool SetParameterFormString( const std::string& name, const std::string& value );


protected:
	//virtual TNode* CreateCopy() const = 0;
	std::unique_ptr<TParameterList> m_pParametersList;

	TNode();

private:

	int m_id;
	std::string m_name;
	int m_referenceCount;
	static TNodeType m_nodeType;

};


/*!
 * Returns the node as a node of type T
 */
template<class T>
const T* TNode::as() const
{
  const T* t = dynamic_cast<const T*>(this);
  return t;
}

/*!
 * Returns the node as a node of type T
 */
template<class T>
T* TNode::as()
{
  T* t = dynamic_cast<T*>(this);
  return t;
}

/*!
 * Returns the value of the parameter \a name.
 */
template<class T> T TNode::GetParameterValue( std::string name ) const
{
	//if(!m_parametersList )	return( std::any() );
	if( !m_pParametersList->Contains( name ) || !m_pParametersList->GetVisibility( name ) )
		return( T{} );
	return ( std::get<T>( m_pParametersList->GetValue( name ).value() ) );

}

/*!
 * Sets to the parameter \a name the \a value.
 */
template<class T>  bool TNode::SetParameterValue( const std::string& name, const T& value )
{
	if( !m_pParametersList )	return ( false );
	if( !m_pParametersList->Contains( name ) || !m_pParametersList->GetVisibility( name ) )
		return ( false );

	if( !m_pParametersList->SetValue( name, value ) ) 	return ( false );

	return (true);
}


#endif /* TNODE_H_ */
