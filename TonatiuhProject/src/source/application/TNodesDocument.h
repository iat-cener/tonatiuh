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

#ifndef TNODESDOCUMENT_H_
#define TNODESDOCUMENT_H_

class TContainerNode;
class TNode;
class TNodesList;
class QDomElement;

//!  TNodesDocument class saves and reads a nodes tree in/from a file.
/*!
*  TNodesDocument class manages the read and write actions of a nodes structure. This class includes all the methods to save and read all the nodes and their parameters.
* 	- width: in the x axis
* 	- height: in the y axis
* 	- depth: in the z axis
*/

class TNodesDocument
{

public:
	TNodesDocument();
	~TNodesDocument();

	TNode*  GetRootNode() const;
	bool Read( std::string filename );
	void SetRootNode( TNode* node ) ;
	bool Write( std::string filename ) const;

protected:
	TNode* CreateNodeObject( QDomElement node, std::string* errMsg );
	bool ReadNode( QDomElement node, TNode* parentNode, QMap< int, TNode* >* readNodes, std::string* errMsg );
	bool ReadNodeList( QDomElement node, TNodesList* listNode, QMap< int, TNode* >* readNodes, std::string* errMsg  );
	bool WriteNode( QDomElement parent, const TNode* node, QList< int >* writtenNodes ) const;
	bool WriteConatinerNode( QDomElement parent, const TContainerNode* containerNode, QList< int >* writtenNodes ) const;

private:
	TNode* m_pRootNode;

};


#endif /* TNODESDOCUMENT_H_ */
