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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <Inventor/nodes/SoNode.h>

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "InstanceNode.h"
#include "Ray.h"
#include "TMaterial.h"
#include "Transform.h"
#include "TShape.h"
#include "TShapeKit.h"


InstanceNode::InstanceNode( SoNode* node )
: m_coinNode( node ), m_parent( 0 )
{
}

InstanceNode::InstanceNode( const InstanceNode* node )
{
	m_coinNode = node->m_coinNode;
	node->m_coinNode->ref();
	m_parent = node->m_parent;

	for( int index = 0; index < node->children.count(); ++index )
    {
    	InstanceNode* child = new InstanceNode( *(node->children[index]) );
    	children.append(child);
    }
}

InstanceNode::~InstanceNode()
{
	qDeleteAll( children );
}

/**
 * Returns node URL.
 */
QString InstanceNode::GetNodeURL() const
{
	QString url;
	if( GetParent() ) url = GetParent()->GetNodeURL();
	url.append( "/" + QString( m_coinNode->getName().getString() ) );
	return url;
}

void InstanceNode::Print( int level ) const
{
	for( int i = 0; i < level; ++i ) std::cout << " ";
    std::cout << m_coinNode->getTypeId().getName().getString()
    		  << " has " << children.size()
    		  << " children "<< std::endl;
    for( int index = 0; index < children.count(); ++index )
    	children[index]->Print( level++ );
}

/**
 * Appends new \a child node to the end of the child list.
**/
void InstanceNode::AddChild( InstanceNode* child )
{
    children.push_back( child );
    child->SetParent( this );
}
/**
 * Inserts the \a instanceChild node as child number \a row.
**/
void InstanceNode::InsertChild( int row, InstanceNode* instanceChild)
{
	if( row > children.size() ) row = children.size();
 	children.insert( row, instanceChild);
 	instanceChild->SetParent(this);
}

bool InstanceNode::Intersect( const Ray& ray,
            		        RandomDeviate& rand,
            		        double* tHit,
    						InstanceNode** modelNode,
            		        bool* isFront,
            		        Ray* outRay )
{


	//Check if the ray intersects with the BoundingBox
	double t0 = ray.mint;
	double t1 = ray.maxt;

	for( int i = 0; i < 3; ++i )
	{
		double invRayDir = 1.0 / ray.direction[i];
		double tNear = ( m_bbox.pMin[i] - ray.origin[i] ) * invRayDir;
		double tFar = ( m_bbox.pMax[i] - ray.origin[i] ) * invRayDir;
		if( tNear > tFar ) std::swap( tNear, tFar );
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if( t0 > t1 ) return false;
	}

	if( !GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
	{
		Ray reflected;
		bool isOutputRay = false;

		///Check if the ray intersects with the BoundingBox
		for( int index = 0; index < children.size(); ++index )
		{
			InstanceNode* intersectedChild = 0;
			bool isChildFront = false;
			Ray childOutputRay;
			double childHit = *tHit;

			bool isChildOutputRay = children[index]->Intersect( ray, rand, &childHit, &intersectedChild, &isChildFront, &childOutputRay );

			if( childHit < *tHit )
			{
				isOutputRay = false;
				*tHit = childHit;
				*modelNode = intersectedChild;
				*isFront = isChildFront;

				if( isChildOutputRay )
				{
					reflected = childOutputRay;
					isOutputRay = true;
				}
				else	reflected = Ray();
			}


		}
		if( isOutputRay )
		{
			*outRay = reflected;
			return true;
		}

	}
	else
	{
		TShapeKit* shapeKit  = static_cast< TShapeKit* > ( GetNode() );

		Ray childCoordinatesRay( m_transformWTO( ray ) );
		childCoordinatesRay.maxt = ray.maxt;

		bool isShapeFront = false;

		InstanceNode* shapeInstance = 0;
		if( children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
			shapeInstance =  children[0];
		else if(  children.count() > 1 )	shapeInstance =  children[1];

		if( shapeInstance != 0 )
		{
			TShape* tshape = static_cast< TShape* >( shapeInstance->GetNode() );

			double thit = 0.0;
			DifferentialGeometry dg;
			bool intersect = tshape->Intersect( childCoordinatesRay, &thit, &dg );
			isShapeFront = dg.shapeFrontSide;
			if( intersect )
			{
				childCoordinatesRay.maxt = thit;
				*tHit = childCoordinatesRay.maxt;

				InstanceNode* materialInstance = 0;
				if( children[0]->GetNode()->getTypeId().isDerivedFrom( TMaterial::getClassTypeId() ) )
					materialInstance =  children[0];
				else if(  children.count() > 1 )	materialInstance =  children[1];

				if( materialInstance )
				{
					TMaterial* tmaterial = static_cast< TMaterial* > ( materialInstance->GetNode() );

					Ray surfaceOutputRay;
					bool isOutputRay = tmaterial->OutputRay( childCoordinatesRay, &dg, rand, &surfaceOutputRay );

					*modelNode = this;
					*isFront = isShapeFront;
					if( isOutputRay )
					{
						*outRay = m_transformOTW( surfaceOutputRay );
						return true;
					}

				}
			}
		}

	}
	return false;
}

BBox InstanceNode::GetIntersectionBBox()
{
	return m_bbox;
}

Transform InstanceNode::SetIntersectionTransform()
{
	return m_transformWTO;
}

void InstanceNode::SetIntersectionBBox( BBox nodeBBox )
{
	m_bbox = nodeBBox;
}

void InstanceNode::SetIntersectionTransform( Transform nodeTransform )
{

	m_transformWTO = nodeTransform;
	m_transformOTW = m_transformWTO.GetInverse();
}

QDataStream& operator<< ( QDataStream & s, const InstanceNode& node )
{
	s << node.GetNode();
	return s;
}

QDataStream& operator>> ( QDataStream & s, const InstanceNode& node )
{
	s >> node;
	return s;
}

bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
	return ( (thisNode.GetNode() == otherNode.GetNode()) &&
			 (thisNode.GetParent()->GetNode() == otherNode.GetParent()->GetNode()) );
}
