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

#ifndef INSTANCENODE_H_
#define INSTANCENODE_H_

#include <vector>

#include "BBox.h"
#include "Transform.h"

class RandomDeviate;
class Ray;
class SoNode;
class QString;

/*!
 * @class InstanceNode
 * @brief Represents an instance of a node in the scene.
 *
 * In a scene, a node can be shared by more than one parent. Each shared instance
 * is represented as an InstanceNode object.
 *
 * Any change made within a shared node is reflected in all corresponding
 * InstanceNode objects.
 */
class InstanceNode
{
public:
    InstanceNode( SoNode* node );
    ~InstanceNode();
    
    void AddChild( InstanceNode* child );

    InstanceNode* GetChild( int childIndex ) ;
    int GetChildIndex( InstanceNode* child ) const;
    BBox GetIntersectionBBox();
    Transform GetIntersectionTransform();
    SoNode* GetNode() const;
    QString GetNodeURL() const;
    InstanceNode* GetParent() const;

    void InsertChild( int row, InstanceNode* instanceChild);
    bool Intersect( const Ray& ray, RandomDeviate& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay );
    
    int NumberOfChildren( ) const;
    bool RemoveChild( int row );

    void SetIntersectionBBox( BBox nodeBBox );
    void SetIntersectionTransform( Transform nodeTransform );
    void SetNode( SoNode* node );
    void SetParent( InstanceNode* parent );

protected:
    std::vector< InstanceNode* > m_children;

private:
    SoNode* m_coinNode;
    InstanceNode* m_parent;
    BBox m_bbox;
    Transform m_transformWTO;
    Transform m_transformOTW;
};

bool operator==( const InstanceNode& thisNode,const InstanceNode& otherNode );

/*!
 * @brief Sets the parent of the current object.
 *
 * @param parent Pointer to the node to assign as the current object's parent.
 */
inline void InstanceNode::SetParent( InstanceNode* parent )
{
	m_parent = parent;
}

/*!
 * @brief Sets the current object's node.
 *
 * @param node Pointer to the node to assign to the current object.
 */
inline void InstanceNode::SetNode( SoNode* node )
{
	m_coinNode = node;
}

/*!
 * @brief Returns the child node at a given index.
 *
 * @param index The position of the child node to retrieve.
 * @return Pointer to the child node at the specified index, or nullptr if the index is out of bounds.
 */
inline InstanceNode* InstanceNode::GetChild( int childIndex )
{
    if( int( m_children.size() ) <= childIndex ) return nullptr;
    
    return ( m_children[childIndex] );
}

/*!
 * @brief Returns the number of children of the node.
 *
 * @return The number of child nodes.
 */
inline int InstanceNode::NumberOfChildren( ) const
{
    return int( m_children.size() );
}

inline SoNode* InstanceNode::GetNode() const
{
	return m_coinNode;
}

/*!
 * @brief Returns the parent instance of the current node.
 *
 * @return Pointer to the parent instance.
 */
inline InstanceNode* InstanceNode::GetParent() const
{
	return m_parent;
}

#endif /*INSTANCENODE_H_*/
