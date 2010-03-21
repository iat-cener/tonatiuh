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

#include <iostream>
#include <QMap>
#include <Inventor/nodes/SoNode.h>

#include "InstanceNode.h"
#include "Ray.h"
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
    children.append( child );
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

Ray* InstanceNode::Intersect( const Ray& ray,
		                      RandomDeviate& rand,
		                      QMap< InstanceNode*,QPair< SbBox3f, Transform* > >* sceneMap,
		                      InstanceNode** modelNode,
		                      bool* isFront )
{
	QPair< SbBox3f, Transform* > instanceData =  sceneMap->value( this );

	//Transform ray to InstaceNode coordinates
	Transform* worldToObject =  instanceData.second;

	Ray objectRay( (*worldToObject)( ray ) );

	//Check if the ray intersects with the BoundingBox
	SbBox3f box = instanceData.first;


	double t0 = objectRay.mint;
	double t1 = objectRay.maxt;

	for( int i = 0; i < 3; ++i )
	{
		double invRayDir = 1.0 / objectRay.direction[i];
		double tNear = ( box.getMin()[i] - objectRay.origin[i] ) * invRayDir;
		double tFar = ( box.getMax()[i] - objectRay.origin[i] ) * invRayDir;
		if( tNear > tFar ) std::swap( tNear, tFar );
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if( t0 > t1 ) return 0;
	}


	Ray* result = 0;

	if( !GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) )
	{
		Ray* reflected = 0;
		Ray* childReflected = 0;

		//Check if the ray intersects with the BoundingBox
		for( int index = 0; index < children.size(); ++index )
		{
			InstanceNode* intersectedChild = 0;
			bool isChildFront = false;
			double previusMaxT = ray.maxt;

			childReflected = children[index]->Intersect( ray, rand, sceneMap, &intersectedChild, &isChildFront );

			if( ray.maxt < previusMaxT )
			{
				delete reflected;
				reflected = 0;
				*modelNode = intersectedChild;
				*isFront = isChildFront;
			}

			if( childReflected )
			{
				reflected = new Ray( *childReflected );
				delete childReflected;
				childReflected = 0;
			}

		}
		if( reflected )
		{
			result = new Ray( *reflected );
			delete reflected;
			reflected = 0;
		}

	}
	else
	{
		TShapeKit* shapeKit  = static_cast< TShapeKit* > ( GetNode() );

		QPair< SbBox3f, Transform* > childData;
		if( children.count() == 0 ) return result;

		if( children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
			childData = sceneMap->value( children[0] );
		else if( ( children.count() > 1 ) && ( children[1]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) ) )
					childData = sceneMap->value( children[1] );
		else return result;


		Transform* shapeWorldToObject =  childData.second;

		Ray childCoordinatesRay( (*shapeWorldToObject)( ray ) );
		childCoordinatesRay.maxt = ray.maxt;

		double objectMaxT = childCoordinatesRay.maxt;
		bool isShapeFront = false;
		Ray* reflected = shapeKit->Intersect( childCoordinatesRay, &isShapeFront, rand );


		if( objectMaxT != childCoordinatesRay.maxt )
		{
			ray.maxt = childCoordinatesRay.maxt;
			if( reflected )
			{

				Transform shapeObjectToWorld = shapeWorldToObject->GetInverse();
				result = new Ray( shapeObjectToWorld( *reflected ) );
			}
			delete reflected;
			*modelNode = this;

			*isFront = isShapeFront;
		}

	}

	return result;
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
