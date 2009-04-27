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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QMap>
#include <QPair>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

#include "InstanceNode.h"
#include "Ray.h"
#include "Trace.h"
#include "Transform.h"
#include "TSeparatorKit.h"
#include "TShape.h"
#include "TShapeKit.h"


InstanceNode::InstanceNode( SoNode* node )
: m_coinNode( node ), m_parent( 0 )
{
	Trace trace( "InstanceNode::InstanceNode", false );
}

InstanceNode::InstanceNode( const InstanceNode* node )
{
	Trace trace( "InstanceNode::InstanceNode", false );

	m_coinNode = node->m_coinNode;
	node->m_coinNode->ref();
	m_parent = node->m_parent;

	for( int index = 0; index < node->children.count(); index++ )
    {
    	InstanceNode* child = new InstanceNode(*(node->children[index]));

    	children.append(child);
    }
}

InstanceNode::~InstanceNode()
{
	Trace trace( "InstanceNode::~InstanceNode", false );
	qDeleteAll( children );
}

void InstanceNode::SetParent( InstanceNode* parent )
{
	Trace trace( "InstanceNode::SetParent", false );
	m_parent = parent;
}

void InstanceNode::SetNode( SoNode* node )
{
	Trace trace( "InstanceNode::SetNode", false );
	m_coinNode = node;
}

SoNode* InstanceNode::GetNode() const
{
	Trace trace( "InstanceNode::GetNode", false );
	return m_coinNode;
}

InstanceNode* InstanceNode::GetParent() const
{
	Trace trace( "InstanceNode::GetParent", false );
	return m_parent;
}

void InstanceNode::Print( int level ) const
{
	Trace trace( "InstanceNode::Print", false );

	for( int i = 0; i < level; i++ ) std::cout << " ";
    std::cout << m_coinNode->getTypeId().getName().getString() << " has " << children.size() << " children "<<std::endl;
    for( int index = 0; index < children.count(); index++ )
    {
    	children[index]->Print( level++ );
    }
}

void InstanceNode::AddChild( InstanceNode* child )
{
	Trace trace( "InstanceNode::AddChild", false );

    children.append( child );
    child->SetParent( this );
}

void InstanceNode::InsertChild( int row, InstanceNode* instanceChild)
{
	Trace trace( "InstanceNode::InsertChild", false );

	if( row > children.size() ) row = children.size();
 	children.insert( row, instanceChild);
 	instanceChild->SetParent(this);
}

QDataStream& operator<< ( QDataStream & s, const InstanceNode & node )
{
	Trace trace( "InstanceNode operator<<", false );

	s << node.m_coinNode;
	return s;
}

QDataStream& operator>> ( QDataStream & s, const InstanceNode & node )
{
	Trace trace( "InstanceNode operator>>", false );

	s >> node;
	return s;
}

bool operator==(const InstanceNode& thisNode,const InstanceNode& otherNode)
{
	Trace trace( "InstanceNode operator==", false );

	return ( (thisNode.GetNode() == otherNode.GetNode()) &&
			(thisNode.GetParent()->GetNode()==otherNode.GetParent()->GetNode()));
}

Ray* InstanceNode::Intersect( const Ray& ray, RandomDeviate& rand, QMap< InstanceNode*,QPair< SbBox3f, Transform* > >* sceneMap, InstanceNode** modelNode )
{
	Trace trace( "InstanceNode::Intersect", false );

	QPair< SbBox3f, Transform* > instanceData =  sceneMap->value( this );

	//Transform ray to InstaceNode coordinates
	Transform* worldToObject =  instanceData. second;

	Ray objectRay( (*worldToObject)( ray ) );
	objectRay.maxt = ray.maxt;

	//Check if the ray intersects with the BoundingBox
	SbBox3f box = instanceData.first;

	double t0 = ray.mint;
	double t1 = ray.maxt;

	for( int i = 0; i < 3; ++i )
	{
		double invRayDir = 1.0 / ray.direction[i];
		double tNear = ( box.getMin()[i] - ray.origin[i] ) * invRayDir;
		double tFar = ( box.getMax()[i] - ray.origin[i] ) * invRayDir;
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
		//TSeparatorKit* separatorNode = dynamic_cast< TSeparatorKit* > ( GetNode() );
		//if( !separatorNode->IntersectP( objectRay ) ) return 0;

		for( int index = 0; index < children.size(); index++ )
		{
			InstanceNode* intersectedChild = 0;
			double previusMaxT = ray.maxt;

			childReflected = children[index]->Intersect( ray, rand, sceneMap, &intersectedChild );

			if( ray.maxt < previusMaxT )
			{
				delete reflected;
				reflected = 0;
				*modelNode = intersectedChild;

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
		TShapeKit* shapeKit  = dynamic_cast< TShapeKit* > ( GetNode() );

		QPair< SbBox3f, Transform* > childData;
		if( children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
			childData = sceneMap->value( children[0] );
		else
			childData = sceneMap->value( children[1] );

		Transform* shapeWorldToObject =  childData. second;

		Ray childCoordinatesRay( (*shapeWorldToObject)( ray ) );
		childCoordinatesRay.maxt = ray.maxt;

		double objectMaxT = childCoordinatesRay.maxt;
		Ray* reflected = shapeKit->Intersect( childCoordinatesRay, rand );


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
		}
	}

	return result;
}
