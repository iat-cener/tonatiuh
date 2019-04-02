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
***************************************************************************/

#ifndef BVH_H_
#define BVH_H_




#include <vector>

#include "BBox.h"
#include "Triangle.h"

class DifferentialGeometry;

/*! *****************************
 * class BVHNode
 * **************************** */
class BVHNode {

public:
	BVHNode();
	~BVHNode();

	BBox GetBoundingBox() const { return ( m_bbox ); };

	int GetIndex() const { return  (m_index ); };
	BVHNode* GetLeftNode() const { return  (m_leftNode ); };
	int GetNumberOfTriangles() const { return  (m_nOfTriangles ); };
	BVHNode* GetRightNode() const { return  (m_rightNode ); };

	bool IsLeaf() const { return ( m_isLeaf ); };
	void MakeLeaf(unsigned int index, unsigned int nTriangles );

	void SetBoundingBox( BBox bbox );
	void SetLeftNode( BVHNode* childNode );
	void SetRightNode( BVHNode* childNode );


private:
	BBox m_bbox;
	int m_index;
	bool m_isLeaf;
	int m_nOfTriangles;

	BVHNode* m_leftNode;
	BVHNode* m_rightNode;



};


/*! *****************************
 * class BVH
 * **************************** */
class BVH {

public:

	BVH( std::vector< Triangle*>* triangleList, int leafSize = 1 );
	~BVH();

	BBox GetBBox() const;
	bool Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg ) const;
	bool Intersect(BVHNode* node, const Ray& objectRay, double *tHit, DifferentialGeometry *dg ) const;
	//bool getIntersection( const Ray& ray, IntersectionInfo *intersection, bool occlusion) const;

private:
	void Build();
	void BuildRecursive(int left_index, int right_index, BVHNode* node, int depth );
	void SortTrinaglesList(int left_index, int right_index, int dimension );


	static bool LessXThan( const Triangle* t1, const Triangle* t2 );
	static bool LessYThan( const Triangle* t1, const Triangle* t2 );
	static bool LessZThan( const Triangle* t1, const Triangle* t2 );


	int m_leafSize;
	int m_nNodes;
	int m_nLeafs;


	BVHNode* m_rootNode;
	std::vector< Triangle*>* m_triangleList;


};


#endif /* BVH_H_ */
