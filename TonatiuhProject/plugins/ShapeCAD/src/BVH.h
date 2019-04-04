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
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
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
