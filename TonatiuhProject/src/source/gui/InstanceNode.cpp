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

#include <algorithm>

#include <QString>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

#include "InstanceNode.h"
#include "Ray.h"
#include "TMaterial.h"
#include "TShape.h"
#include "TShapeKit.h"


/*!
* Creates an object for a \a node.
*/
InstanceNode::InstanceNode( SoNode* node )
: m_coinNode( node ), m_parent( 0 )
{
}

/*!
* Destroys the object.
*/
InstanceNode::~InstanceNode()
{
	// Delete all objects
    for( auto child : m_children ) 
        delete child;

    // Clear vector to remove dangling pointers
    m_children.clear();
}

/*!
 * @brief Appends a new child node to the end of the child list.
 *
 * @param child Pointer to the node to be added as a child.
 */
void InstanceNode::AddChild( InstanceNode* child )
{
    m_children.push_back( child );
    child->SetParent( this );
}

/*!
 * @brief Returns the index of the given child node.
 *
 * Searches the children of the current node and returns the position of the specified child node.
 *
 * @param child Pointer to the child node.
 * @return The index of the child node if found; otherwise, -1.
 */
int InstanceNode::GetChildIndex( InstanceNode* child ) const
{
	auto it = std::find( m_children.begin(), m_children.end(), child );
    if( it != m_children.end() )
	{
        return ( std::distance( m_children.begin(), it) );
	}
	return -1;
}

/*!
 * @brief Returns the bounding box of the instance node.
 *
 * @return The bounding box of the object.
 */
BBox InstanceNode::GetIntersectionBBox()
{
	return m_bbox;
}

/*!
 * @brief Returns the transformation of the node.
 *
 * This transformation converts coordinates from world space to the object's
 * local space.
 *
 * @return The transformation of the node.
 */
Transform InstanceNode::GetIntersectionTransform()
{
	return m_transformWTO;
}

/*!
 * @brief Returns the URL of the node.
 *
 * @return A string representing the node's URL.
 */
QString InstanceNode::GetNodeURL() const
{
   QString url;
   if( GetParent() ) url = GetParent()->GetNodeURL();
   url.append( QLatin1String( "/" ) );
   const char* nodeName = m_coinNode->getName().getString();
   url.append( QLatin1String( nodeName  ) );
   return url;
}

/*!
 * @brief Inserts a child node at a specific position.
 *
 * Inserts the given \a instanceChild node as child number \a row.
 *
 * @param instanceChild Pointer to the child node to insert.
 * @param row The position (index) at which to insert the child node.
 */
void InstanceNode::InsertChild( int row, InstanceNode* instanceChild)
{
	if( row > int( m_children.size()  ) )
   		m_children.push_back( instanceChild );
	else
		m_children.insert( m_children.begin() + row, instanceChild);
   instanceChild->SetParent(this);
}

/*!
 * @brief Executes the intersection routine of a ray with the node.
 *
 * Determines whether the given ray intersects with this node.
 *
 * @param ray The input ray to test for intersection.
 * @param rand Random number generator.
 * @param isShapeFront Set to indicate the surface side if an intersection occurs.
 * @param modelNode The node where the intersection occrus, if any.
 * @param outputRay The resulting ray after intersection, if any.
 * @return true if the ray intersects with the node; false otherwise.
 */
bool InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, bool* isShapeFront, InstanceNode** modelNode, Ray* outputRay )
{
	int nChildren = int( m_children.size() );

	//Check if the ray intersects with the BoundingBox
	if( !m_bbox.IntersectP(ray) ) return false;
	if( !GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
	{
		bool isOutputRay = false;
		double t = ray.maxt;
		for( int index = 0; index < nChildren; ++index )
		{
			InstanceNode* intersectedChild = 0;
			Ray childOutputRay;
			bool childShapreFront = true;
			bool isChildOutputRay = m_children[index]->Intersect( ray, rand, &childShapreFront, &intersectedChild, &childOutputRay );
			
			if( ray.maxt < t )
			{
				t = ray.maxt;
				*modelNode = intersectedChild;
				*isShapeFront = childShapreFront;
				*outputRay = childOutputRay;
				isOutputRay = isChildOutputRay;
			}
		}
		return isOutputRay;
	}
	else
	{
		Ray childCoordinatesRay( m_transformWTO( ray ) );

		TShape* tshape = 0;
		TMaterial* tmaterial = 0;
		if( m_children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
		{
			tshape = static_cast< TShape* >( m_children[0]->GetNode() );
			if( m_children.size() > 1 )	tmaterial = static_cast< TMaterial* > ( m_children[1]->GetNode() );
		}
		else if( nChildren > 1 )
		{
			tmaterial = static_cast< TMaterial* > ( m_children[0]->GetNode() );
			tshape = static_cast< TShape* >( m_children[1]->GetNode() );
		}

		if( tshape )
		{
			 double thit = 0.0;
			 DifferentialGeometry dg;
			 if( !tshape->Intersect( childCoordinatesRay, &thit, &dg ) ) return false;

			 ray.maxt = thit;
			 *modelNode = this;

			 *isShapeFront = dg.shapeFrontSide;

			 if( tmaterial )
			 {
				 Ray surfaceOutputRay;
				 if( tmaterial->OutputRay( childCoordinatesRay, &dg, rand, &surfaceOutputRay ) )
				 {
					 *outputRay = m_transformOTW( surfaceOutputRay );
					 return true;
				 }
			}
      }
   }
	return false;
}

/*!
 * @brief Removes the child node at a given index from the children list.
 *
 * @param row The index of the child node to remove.
 * @return true if the child node was successfully removed; false if the index is out of bounds.
 */
bool InstanceNode::RemoveChild( int row )
{
	if( row >= int( m_children.size() ) )	return false;
	m_children.erase( m_children.begin() + row );
	return true;
}

/*!
 * @brief Sets the bounding box of the object.
 *
 * @param nodeBBox The bounding box to assign to the object.
 */
void InstanceNode::SetIntersectionBBox( BBox nodeBBox )
{
	m_bbox = nodeBBox;
}

/*!
 * @brief Sets the world-to-object transformation of the node.
 *
 * @param nodeTransform The transformation to assign to the node.
 */
void InstanceNode::SetIntersectionTransform( Transform nodeTransform )
{
	m_transformWTO = nodeTransform;
	m_transformOTW = m_transformWTO.GetInverse();
}

/*!
 * @brief Checks whether two instance nodes are the same.
 *
 * Two instances are considered identical if both their stored node
 * and their parent node are the same.
 *
 * @param other The instance node to compare with.
 * @return true if the instances are identical; false otherwise.
 */
bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
	return ( (thisNode.GetNode() == otherNode.GetNode()) &&
			 (thisNode.GetParent()->GetNode() == otherNode.GetParent()->GetNode()) );
}
