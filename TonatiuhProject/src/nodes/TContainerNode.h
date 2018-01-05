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

#ifndef TCONTAINERNODE_H_
#define TCONTAINERNODE_H_

#include <map>
#include <string>

#include "NodeLibrary.h"
#include "TNode.h"

//!  TContainerNode class is the base class for all container nodes.
/*!
  TContainerNode class defines all the basic methods for the nodes that collects different parts for its definition.
*/

class NODE_API TContainerNode : public TNode
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	TContainerNode(const TContainerNode&) = delete;
	TContainerNode& operator=(const TContainerNode&) = delete;

public:
	static void Init();

	TContainerNode();

	virtual TContainerNode* Copy() const = 0;


	bool Contains(const std::string & name) const;
	virtual std::string GetIcon() const;
	TNode* GetPart( const std::string name ) const;
	std::vector<std::string> GetPartNames() const;
	virtual TNodeType GetType() const;

	int NumberOfParts() const;
	virtual bool SetPart( const std::string name, TNode* node  );


protected:
	virtual ~TContainerNode();
	//virtual TNode* CreateCopy() const = 0;

	void AppendPart( const std::string name, TNodeType type, TNode* defaultNode );
	void RevomePart( const std::string name );

	std::map< std::string, TNodeType > m_partsTypeList;
	std::map< std::string, TNode* > m_partsList;

private:
	static TNodeType m_nodeType;
};


#endif /* TCONTAINERNODE_H_ */
