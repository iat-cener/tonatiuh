/*
 * BVH.cpp
 *
 *  Created on: 03/11/2015
 *      Author: amutuberria
 */

#include <algorithm>

#include "BVH.h"
#include "DifferentialGeometry.h"
#include "gf.h"
#include "Ray.h"

/*! *****************************
 * class BVHNode
 * **************************** */

BVHNode::BVHNode()
: m_bbox(),
  m_index( 0 ),
  m_isLeaf( 0 ),
  m_nOfTriangles( 0 ),
  m_leftNode( 0 ),
  m_rightNode( 0 )
{

}

/*!
 * Removes the node and its children
 */
BVHNode::~BVHNode()
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
void BVHNode::MakeLeaf(unsigned int index, unsigned int nTriangles )
{
	m_isLeaf = true;
	m_index = index;
	m_nOfTriangles = nTriangles;

}

/*!
 * Sets left child to \a childNode
 */
void BVHNode::SetLeftNode( BVHNode* childNode )
{
	m_leftNode = childNode;
}

/*!
 * Sets right child to \a childNode
 */
void BVHNode::SetRightNode( BVHNode* childNode )
{
	m_rightNode = childNode;
}


/*!
 * Sets bbox to the node
 */
void BVHNode::SetBoundingBox( BBox bbox )
{
	m_bbox = bbox;
}

/*! *****************************
 * class BVH
 * **************************** */

/*!
 * Creates bounding volume hierarchy object
 */
BVH::BVH( std::vector< Triangle*>* triangleList, int leafSize )
:m_leafSize( leafSize ),
 m_nNodes ( 0 ),
 m_nLeafs( 0 ),
 m_rootNode( 0 ),
 m_triangleList( triangleList )
{

	Build();

	/*
	if( m_rootNode )
	{

		for( unsigned int t = 0; t < m_triangleList->size(); t++ )
		{
			Point3D v1 = m_triangleList->at(t)->GetVertex1();
			Point3D v2 = m_triangleList->at(t)->GetVertex2();
			Point3D v3 = m_triangleList->at(t)->GetVertex3();
			Point3D centroid = m_triangleList->at(t)->GetCentroid();

			std::cout<<v1.x<<"\t"<<v1.y<<"\t"<<v1.z<<"\t"
					<<v2.x<<"\t"<<v2.y<<"\t"<<v2.z<<"\t"
					<<v3.x<<"\t"<<v3.y<<"\t"<<v3.z<<"\t"
					<<centroid.x<<"\t"<<centroid.y<<"\t"<<centroid.z
					<<std::endl;
		}

	}
	*/


}

/*!
 * Destroys hierarchy
 */
BVH::~BVH()
{
	delete m_rootNode;
}


BBox BVH::GetBBox() const
{
	if( m_rootNode )
	{
		return ( m_rootNode->GetBoundingBox( ) );
	}
	return BBox();
}

bool BVH::Intersect(const Ray& objectRay , double* tHit, DifferentialGeometry* dg ) const
{
	//std::cout<<"objectRay: "<<objectRay.origin<<"\t "<<objectRay.direction()<<std::endl;

	if( ! m_rootNode )
		return ( false );

	if( !m_rootNode->GetBoundingBox().IntersectP( objectRay ) )	return ( false );

	double tHitBVH = objectRay.maxt;
	if(!Intersect(m_rootNode, objectRay, &tHitBVH, dg ) )	return ( false );
	if( tHitBVH < *tHit )
	{
		*tHit = tHitBVH;
		return ( true );
	}
	return ( false );

}


bool BVH::Intersect(BVHNode* node, const Ray& objectRay, double *tHit, DifferentialGeometry *dg ) const
{

	double tHitNode = *tHit;

	//std::cout<<"node: "<<node->GetBoundingBox()<<std::endl;
	if( !node->IsLeaf() )
	{

		double tL0 = gc::Infinity;
		double tL1= gc::Infinity;
		bool intersectedL = false;

		double tR0 = gc::Infinity;
		double tR1= gc::Infinity;
		bool intersectedR = false;

		BVHNode* firstNode = 0;
		BVHNode* secondNode = 0;
		BVHNode* leftNode = node->GetLeftNode();
		if( leftNode )
		{
			intersectedL = leftNode->GetBoundingBox().IntersectP( objectRay, &tL0, &tL1 );
			//std::cout<<"left : "<<leftNode->GetBoundingBox()<<" intersectedL: "<<intersectedL<<" tL0: "<<tL0<<std::endl;
			if( intersectedL && tL0 <= tHitNode )
			{
				firstNode = leftNode;

				//std::cout<<"\t left intersect tL0: "<<tL0;
			}
		}



		BVHNode* rightNode = node->GetRightNode();
		if( rightNode )
		{
			intersectedR = rightNode->GetBoundingBox().IntersectP( objectRay, &tR0, &tR1 );
			//std::cout<<"right : "<<rightNode->GetBoundingBox()<<" intersectedR: "<<intersectedR<<" tR0: "<<tR0<<std::endl;
			if( intersectedR && tR0 <= tHitNode )
			{
				secondNode = rightNode;
				//std::cout<<"\t right intersect tR0: "<<tR0<<std::endl;
			}
		}


		/*if( intersectedL && intersectedR && tR0 < tL0 )
		{
			BVHNode* aux = firstNode;
			firstNode = secondNode;
			secondNode = aux;
		}
		*/

		bool isIntersection = false;

		//Fist node
		if( firstNode )
		{
			double thit1 = tHitNode;
			DifferentialGeometry dg1;
			bool isIntersection1 = Intersect( firstNode, objectRay, &thit1, &dg1 );

			if( isIntersection1 && thit1 < tHitNode )
			{
				tHitNode = thit1;
				*tHit = thit1;
				*dg = dg1;

				isIntersection = true;

				//std::cout<<"\t firstNode tHitNode: "<<tHitNode<<std::endl;

			}
		}

		//Second node
		if( secondNode )
		{
			double thit2 = tHitNode;
			DifferentialGeometry dg2;

			bool isIntersection2 = Intersect( rightNode, objectRay, &thit2, &dg2 );

			if( isIntersection2 && thit2 < tHitNode )
			{
				tHitNode = thit2;
				*tHit = thit2;
				*dg = dg2;
				isIntersection = true;
				//std::cout<<"\t secondNode tHitNode: "<<tHitNode<<std::endl;
			}
		}

		return ( isIntersection );
	}
	else
	{
		//std::cout<<"\t leaf node"<<std::endl;
		bool isIntersection = false;
		int left_index = node->GetIndex();
		//std::cout<<"\t left_index: "<<left_index<<" node->GetNumberOfTriangles(): "<<node->GetNumberOfTriangles()<<std::endl;
		for( int f = 0; f < node->GetNumberOfTriangles(); f++ )
		{
			int tIndex = left_index + f;
			Triangle* triangle = m_triangleList->at( tIndex );

			if( triangle )
			{

				Point3D v1 = triangle->GetVertex1();
				Point3D v2 = triangle->GetVertex2();
				Point3D v3 = triangle->GetVertex3();

				/*
				std::cout<<"\t\t--"<<tIndex<<"\t"<<v1.x<<"\t"<<v1.y<<"\t"<<v1.z<<"\t"
						<<v2.x<<"\t"<<v2.y<<"\t"<<v2.z<<"\t"
						<<v3.x<<"\t"<<v3.y<<"\t"<<v3.z
						<<std::endl;
				*/

				double thitT = tHitNode;
				DifferentialGeometry dgT;


				bool isIntersectionT = triangle->Intersect( objectRay, &thitT, &dgT );
				/*std::cout<<"\t\t--isIntersectionT: "<<isIntersectionT
						<<"\t objectRay.maxt: "<<objectRay.maxt
						<<"\t tHitNode: "<<tHitNode
						<<"\t thitT: "<<thitT
										<<std::endl;
				*/
				if( isIntersectionT && thitT < tHitNode )
				{
					tHitNode = thitT;
					//std::cout<<"\t\t--"<<tHitNode <<std::endl;
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
void BVH::Build()
{

	BBox hBBox;

	for( unsigned int t = 0; t < m_triangleList->size(); t++ )
	{

		Triangle* triangle = m_triangleList->at(t);

		hBBox = Union ( hBBox, triangle->GetBBox( ) );
	}


	int left_index = 0;
	int right_index = m_triangleList->size();
	//int nodeIndex = 1;

	m_rootNode = new BVHNode;
	m_rootNode->SetBoundingBox( hBBox );
	m_nNodes++;

	BuildRecursive( left_index, right_index, m_rootNode, 0 );

}

void BVH::BuildRecursive(int left_index, int right_index, BVHNode* node, int depth)
{

	if( ( right_index - left_index ) <= m_leafSize )
	{
		node->MakeLeaf( left_index, right_index - left_index );
		//std::cout<<"leaf: "<<left_index<<std::endl;
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

		/*
		if( left_index == 22 && right_index == 44 )
		{
			std::cout<<"\t"<<left_index<<"\t"<<right_index<<std::endl;

			std::cout<<"Before sort"<<std::endl;
			for( unsigned int t = 0; t < right_index; t++ )
			{
				Point3D v1 = m_triangleList->at(t)->GetVertex1();
				Point3D v2 = m_triangleList->at(t)->GetVertex2();
				Point3D v3 = m_triangleList->at(t)->GetVertex3();
				Point3D centroid = m_triangleList->at(t)->GetCentroid();

				std::cout<<v1.x<<"\t"<<v1.y<<"\t"<<v1.z<<"\t"
						<<v2.x<<"\t"<<v2.y<<"\t"<<v2.z<<"\t"
						<<v3.x<<"\t"<<v3.y<<"\t"<<v3.z<<"\t"
						<<centroid.x<<"\t"<<centroid.y<<"\t"<<centroid.z
						<<std::endl;
			}
		}
		*/

		SortTrinaglesList( left_index, right_index, dimension1 );

		/*
		if( left_index == 22 && right_index == 44 )
		{
			std::cout<<"\t"<<left_index<<"\t"<<right_index<<std::endl;

			std::cout<<"After sort"<<std::endl;
			for( unsigned int t = 0; t < right_index; t++ )
			{
				Point3D v1 = m_triangleList->at(t)->GetVertex1();
				Point3D v2 = m_triangleList->at(t)->GetVertex2();
				Point3D v3 = m_triangleList->at(t)->GetVertex3();
				Point3D centroid = m_triangleList->at(t)->GetCentroid();

				std::cout<<v1.x<<"\t"<<v1.y<<"\t"<<v1.z<<"\t"
						<<v2.x<<"\t"<<v2.y<<"\t"<<v2.z<<"\t"
						<<v3.x<<"\t"<<v3.y<<"\t"<<v3.z<<"\t"
						<<centroid.x<<"\t"<<centroid.y<<"\t"<<centroid.z
						<<std::endl;
			}
		}
	*/

		BBox leftBBox;
		int splitIndex = right_index;
		for ( int f = left_index; f < right_index; f++ )
		{

			Triangle* triangle = m_triangleList->at( f );

			if( ( dimension1 == 0 ) && ( triangle->GetCentroid().x > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			else if( ( dimension1 == 1 ) && ( triangle->GetCentroid().y > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			else if( ( dimension1 == 2 ) && (triangle->GetCentroid().z > dMean1 ) )
			{
				splitIndex = f;
				break;
			}
			leftBBox = Union ( leftBBox, triangle->GetBBox( ) );

		}



		//std::cout<<" --splitIndex: "<<splitIndex<<std::endl;
		if( ( splitIndex == right_index ) || ( splitIndex == left_index ) )
		{

			splitIndex  = left_index + 0.5 * ( right_index -left_index ) ;
			leftBBox = BBox( );
			for ( int f = left_index; f < splitIndex; f++ )
			{
				Triangle* triangle = m_triangleList->at( f );
				leftBBox = Union ( leftBBox, triangle->GetBBox( ) );
			}


			/*std::cout<<" --splitIndex: "<<splitIndex<<" dimension2: "<<dimension2<<std::endl;
			SortTrinaglesList( left_index, right_index, dimension2 );
			leftBBox = BBox( );

			int splitIndex = right_index;
			for ( int f = left_index; f < right_index; f++ )
			{

				Triangle* triangle = m_triangleList->at( f);

				if( ( dimension2 == 0 ) && ( triangle->GetCentroid().x > dMean2 ) )
				{
					splitIndex = f;
					break;
				}
				else if( ( dimension2 == 1 ) && ( triangle->GetCentroid().y > dMean2 ) )
				{
					splitIndex = f;
					break;
				}
				else if( ( dimension2 == 2 ) && (triangle->GetCentroid().z > dMean2 ) )
				{
					splitIndex = f;
					break;
				}
				leftBBox = Union ( leftBBox, triangle->GetBBox( ) );

			}
			if( ( splitIndex == right_index ) || ( splitIndex == left_index ) )
			{

				splitIndex  = left_index + 0.5 * ( right_index -left_index ) ;
				leftBBox = BBox( );
				for ( int f = left_index; f < splitIndex; f++ )
				{
					Triangle* triangle = m_triangleList->at( f );
					leftBBox = Union ( leftBBox, triangle->GetBBox( ) );
				}
			}
			*/

		}


		BBox rightBBox;
		for ( int f = splitIndex; f < right_index; f++ )
		{
			Triangle* triangle = m_triangleList->at( f );
			rightBBox = Union ( rightBBox, triangle->GetBBox( ) );
		}

		//std::cout<<"\t"<<left_index<<"\t"<<splitIndex<<"\t"<<right_index<<std::endl;
		//if( left_index == 22 ) std::cout<<"\t"<<left_index<<"\t"<<splitIndex<<"\t"<<right_index<<" leftBBox: "<<leftBBox<<" rightBBox: "<<rightBBox<<std::endl;

		BVHNode* leftNode = new BVHNode;
		leftNode->SetBoundingBox( leftBBox );
		node->SetLeftNode( leftNode );
		m_nNodes++;

		BVHNode* rightNode = new BVHNode;
		rightNode->SetBoundingBox( rightBBox );
		node->SetRightNode( rightNode );
		m_nNodes++;

		//for( int i = 0; i < depth; i++ )
		//		std::cout<<"\t";

		//std::cout<<left_index<<"\t"<<splitIndex<<"\t"<<right_index<<std::endl;
		BuildRecursive( left_index, splitIndex, leftNode, depth +1 );
		BuildRecursive( splitIndex, right_index, rightNode, depth +1 );
	}
}


void BVH::SortTrinaglesList(int left_index, int right_index, int dimension )
{
	if( dimension == 0 )  //x axis
		std::sort( m_triangleList->begin()+ left_index, m_triangleList->begin() +  right_index  , BVH::LessXThan );
	else if( dimension == 1 )  //y axis
		std::sort( m_triangleList->begin()+ left_index, m_triangleList->begin() +  right_index  , BVH::LessYThan );
	else if( dimension == 2 )  //y axis
		std::sort( m_triangleList->begin()+ left_index, m_triangleList->begin() +  right_index  , BVH::LessZThan );

}


bool BVH::LessXThan( const Triangle* t1, const Triangle* t2 )
{

	if( t1->GetCentroid().x < t2->GetCentroid().x )
		return ( true );
	return ( false );
}

bool BVH::LessYThan( const Triangle* t1, const Triangle* t2 )
{

	if( t1->GetCentroid().y < t2->GetCentroid().y )
		return ( true );
	return ( false );
}

bool BVH::LessZThan( const Triangle* t1, const Triangle* t2 )
{

	if( t1->GetCentroid().z < t2->GetCentroid().z )
		return ( true );
	return ( false );
}


