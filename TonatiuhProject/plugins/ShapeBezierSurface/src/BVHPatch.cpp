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

#include <algorithm>

#include "BVHPatch.h"
#include "DifferentialGeometry.h"
#include "gf.h"
#include "Ray.h"

/*! *****************************
 * class BVHPatchNode
 * **************************** */

BVHPatchNode::BVHPatchNode()
: m_bbox(),
  m_index( 0 ),
  m_isLeaf( 0 ),
  m_nOfPatches( 0 ),
  m_leftNode( 0 ),
  m_rightNode( 0 )
{

}

/*!
 * Removes the node and its children
 */
BVHPatchNode::~BVHPatchNode()
{
	if( !m_isLeaf )
	{
		if( m_leftNode )	delete m_leftNode;
		if( m_rightNode )	delete m_rightNode;
	}
}


/*!
 * Defines this node as leaf node.
 */
void BVHPatchNode::MakeLeaf(unsigned int index, unsigned int nTriangles )
{
	m_isLeaf = true;
	m_index = index;
	m_nOfPatches = nTriangles;

}

/*!
 * Sets left child to \a childNode
 */
void BVHPatchNode::SetLeftNode( BVHPatchNode* childNode )
{
	m_leftNode = childNode;
}

/*!
 * Sets right child to \a childNode
 */
void BVHPatchNode::SetRightNode( BVHPatchNode* childNode )
{
	m_rightNode = childNode;
}


/*!
 * Sets bbox to the node
 */
void BVHPatchNode::SetBoundingBox( BBox bbox )
{
	m_bbox = bbox;
}

/*! *****************************
 * class BVHPatch
 * **************************** */

/*!
 * Creates bounding volume hierarchy object
 */
BVHPatch::BVHPatch( std::vector< BezierPatch*>* patchesList, int leafSize )
:m_leafSize( leafSize ),
 m_nNodes ( 0 ),
 m_nLeafs( 0 ),
 m_rootNode( 0 ),
 m_patchesList( patchesList )
{

	Build();



}

/*!
 * Destroys hierarchy
 */
BVHPatch::~BVHPatch()
{
	delete m_rootNode;
}


BBox BVHPatch::GetBBox() const
{
	if( m_rootNode )
	{
		return ( m_rootNode->GetBoundingBox( ) );
	}
	return BBox();
}

bool BVHPatch::Intersect(const Ray& objectRay , double* tHit, DifferentialGeometry* dg, double bezierTol ) const
{
	if( ! m_rootNode )
		return ( false );

	if( !m_rootNode->GetBoundingBox().IntersectP( objectRay ) )	return ( false );

	double tHitBVH = objectRay.maxt;
	if(!Intersect(m_rootNode, objectRay, &tHitBVH, dg, bezierTol ) )	return ( false );

	if( tHitBVH < *tHit )
	{
		*tHit = tHitBVH;
		return ( true );
	}
	return ( false );

}


bool BVHPatch::Intersect(BVHPatchNode* node, const Ray& objectRay, double *tHit, DifferentialGeometry *dg, double bezierTol ) const
{

	double tHitNode = *tHit;
	if( !node->IsLeaf() )
	{

		double tL0 = gc::Infinity;
		double tL1= gc::Infinity;
		bool intersectedL = false;

		double tR0 = gc::Infinity;
		double tR1= gc::Infinity;
		bool intersectedR = false;

		BVHPatchNode* firstNode = 0;
		BVHPatchNode* secondNode = 0;
		BVHPatchNode* leftNode = node->GetLeftNode();
		if( leftNode )
		{
			intersectedL = leftNode->GetBoundingBox().IntersectP( objectRay, &tL0, &tL1 );
			if( intersectedL && tL0 <= tHitNode )
			{
				firstNode = leftNode;
			}
		}



		BVHPatchNode* rightNode = node->GetRightNode();
		if( rightNode )
		{
			intersectedR = rightNode->GetBoundingBox().IntersectP( objectRay, &tR0, &tR1 );
			if( intersectedR && tR0 <= tHitNode )
			{
				secondNode = rightNode;
			}
		}


		bool isIntersection = false;

		//Fist node
		if( firstNode )
		{
			double thit1 = tHitNode;
			DifferentialGeometry dg1;
			bool isIntersection1 = Intersect( firstNode, objectRay, &thit1, &dg1, bezierTol );

			if( isIntersection1 && thit1 < tHitNode )
			{
				tHitNode = thit1;
				*tHit = thit1;
				*dg = dg1;

				isIntersection = true;

			}
		}

		//Second node
		if( secondNode )
		{
			double thit2 = tHitNode;
			DifferentialGeometry dg2;

			bool isIntersection2 = Intersect( rightNode, objectRay, &thit2, &dg2, bezierTol );

			if( isIntersection2 && thit2 < tHitNode )
			{
				tHitNode = thit2;
				*tHit = thit2;
				*dg = dg2;
				isIntersection = true;
			}
		}

		return ( isIntersection );
	}
	else
	{
		bool isIntersection = false;
		int left_index = node->GetIndex();
		for( int f = 0; f < node->GetNumberOfPatches(); f++ )
		{
			int tIndex = left_index + f;
			BezierPatch* patch = m_patchesList->at( tIndex );
			if( patch )
			{

				double thitT = tHitNode;
				DifferentialGeometry dgT;


				bool isIntersectionT = patch->Intersect( objectRay, &thitT, &dgT, bezierTol );

				if( isIntersectionT && thitT < tHitNode )
				{
					tHitNode = thitT;
					*tHit = thitT;
					*dg = dgT;
					isIntersection = true;
				}
			}
		}

		return ( isIntersection );
	}

	return ( false );
}

/*!
 * Creates the nodes hierarchy.
 */
void BVHPatch::Build()
{

	BBox hBBox;

	for( unsigned int t = 0; t < m_patchesList->size(); t++ )
	{

		BezierPatch* patch = m_patchesList->at(t);

		hBBox = Union ( hBBox, patch->GetBBox( ) );
	}


	int left_index = 0;
	int right_index = m_patchesList->size();

	m_rootNode = new BVHPatchNode;
	m_rootNode->SetBoundingBox( hBBox );
	m_nNodes++;

	BuildRecursive( left_index, right_index, m_rootNode, 0 );

}

void BVHPatch::BuildRecursive(int left_index, int right_index, BVHPatchNode* node, int depth)
{

	if( ( right_index - left_index ) <= m_leafSize )
	{
		node->MakeLeaf( left_index, right_index - left_index );
		m_nLeafs++;
	}
	else
	{
		int dimension1 = 0; //x
		BBox nodeBB = node->GetBoundingBox();

		double xLength = nodeBB.pMax.x - nodeBB.pMin.x;
		double yLength = nodeBB.pMax.y - nodeBB.pMin.y;
		double zLength = nodeBB.pMax.z - nodeBB.pMin.z;
		double dMeanX = nodeBB.pMin.x + 0.5 * xLength;
		double dMeanY = nodeBB.pMin.y + 0.5 * yLength;
		double dMeanZ = nodeBB.pMin.z + 0.5 * zLength;

		double dLength1 = xLength;
		double dMean1 = dMeanX;

		if( yLength > dLength1 )
		{
			dLength1 = yLength;
			dMean1 = dMeanY;
			dimension1 = 1;
		}
		if( zLength > dLength1 )
		{
			dLength1 = zLength;
			dMean1 = dMeanZ;
			dimension1 = 2;
		}



		SortPatchesList( left_index, right_index, dimension1 );


		BBox leftBBox;
		int splitIndex = right_index;
		for ( int f = left_index; f < right_index; f++ )
		{

			BezierPatch* patch = m_patchesList->at( f );

			if( ( dimension1 == 0 ) && ( patch->GetCentroid().x > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			else if( ( dimension1 == 1 ) && ( patch->GetCentroid().y > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			else if( ( dimension1 == 2 ) && (patch->GetCentroid().z > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			leftBBox = Union ( leftBBox, patch->GetBBox( ) );

		}



		if( ( splitIndex == right_index ) || ( splitIndex == left_index ) )
		{

			splitIndex  = left_index + 0.5 * ( right_index -left_index ) ;
			leftBBox = BBox( );
			for ( int f = left_index; f < splitIndex; f++ )
			{
				BezierPatch* patch = m_patchesList->at( f );
				leftBBox = Union ( leftBBox, patch->GetBBox( ) );
			}


		}


		BBox rightBBox;
		for ( int f = splitIndex; f < right_index; f++ )
		{
			BezierPatch* patch = m_patchesList->at( f );
			rightBBox = Union ( rightBBox, patch->GetBBox( ) );
		}

		BVHPatchNode* leftNode = new BVHPatchNode;
		leftNode->SetBoundingBox( leftBBox );
		node->SetLeftNode( leftNode );
		m_nNodes++;

		BVHPatchNode* rightNode = new BVHPatchNode;
		rightNode->SetBoundingBox( rightBBox );
		node->SetRightNode( rightNode );
		m_nNodes++;

		BuildRecursive( left_index, splitIndex, leftNode, depth +1 );
		BuildRecursive( splitIndex, right_index, rightNode, depth +1 );
	}
}


void BVHPatch::SortPatchesList(int left_index, int right_index, int dimension )
{
	if( dimension == 0 )  //x axis
		std::sort( m_patchesList->begin()+ left_index, m_patchesList->begin() +  right_index  , BVHPatch::LessXThan );
	else if( dimension == 1 )  //y axis
		std::sort( m_patchesList->begin()+ left_index, m_patchesList->begin() +  right_index  , BVHPatch::LessYThan );
	else if( dimension == 2 )  //y axis
		std::sort( m_patchesList->begin()+ left_index, m_patchesList->begin() +  right_index  , BVHPatch::LessZThan );

}


bool BVHPatch::LessXThan( const BezierPatch* t1, const BezierPatch* t2 )
{

	if( t1->GetCentroid().x < t2->GetCentroid().x )
		return ( true );
	return ( false );
}

bool BVHPatch::LessYThan( const BezierPatch* t1, const BezierPatch* t2 )
{

	if( t1->GetCentroid().y < t2->GetCentroid().y )
		return ( true );
	return ( false );
}

bool BVHPatch::LessZThan( const BezierPatch* t1, const BezierPatch* t2 )
{

	if( t1->GetCentroid().z < t2->GetCentroid().z )
		return ( true );
	return ( false );
}





