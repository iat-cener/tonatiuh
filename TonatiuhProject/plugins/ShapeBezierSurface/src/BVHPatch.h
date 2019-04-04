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

#ifndef BVHPATCH_H_
#define BVHPATCH_H_

#include <vector>

#include "BBox.h"
#include "BezierPatch.h"

class DifferentialGeometry;

/*! *****************************
 * class BVHPatchNode
 * **************************** */
class BVHPatchNode {

public:
	BVHPatchNode();
	~BVHPatchNode();

	BBox GetBoundingBox() const { return ( m_bbox ); };

	int GetIndex() const { return  (m_index ); };
	BVHPatchNode* GetLeftNode() const { return  (m_leftNode ); };
	int GetNumberOfPatches() const { return  (m_nOfPatches ); };
	BVHPatchNode* GetRightNode() const { return  (m_rightNode ); };

	bool IsLeaf() const { return ( m_isLeaf ); };
	void MakeLeaf(unsigned int index, unsigned int nTriangles );

	void SetBoundingBox( BBox bbox );
	void SetLeftNode( BVHPatchNode* childNode );
	void SetRightNode( BVHPatchNode* childNode );


private:
	BBox m_bbox;
	int m_index;
	bool m_isLeaf;
	int m_nOfPatches;

	BVHPatchNode* m_leftNode;
	BVHPatchNode* m_rightNode;



};


/*! *****************************
 * class BVHPatch
 * **************************** */
class BVHPatch {

public:

	BVHPatch( std::vector< BezierPatch*>* patchesList, int leafSize = 1 );
	~BVHPatch();

	BBox GetBBox() const;
	bool Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg, double bezierTol ) const;
	bool Intersect(BVHPatchNode* node, const Ray& objectRay, double *tHit, DifferentialGeometry *dg, double bezierTol ) const;

private:
	void Build();
	void BuildRecursive(int left_index, int right_index, BVHPatchNode* node, int depth );
	void SortPatchesList(int left_index, int right_index, int dimension );


	static bool LessXThan( const BezierPatch* p1, const BezierPatch* p2 );
	static bool LessYThan( const BezierPatch* p1, const BezierPatch* p2 );
	static bool LessZThan( const BezierPatch* p1, const BezierPatch* p2 );


	int m_leafSize;
	int m_nNodes;
	int m_nLeafs;


	BVHPatchNode* m_rootNode;
	std::vector< BezierPatch*>* m_patchesList;


};




#endif /* BVHPATCH_H_ */
