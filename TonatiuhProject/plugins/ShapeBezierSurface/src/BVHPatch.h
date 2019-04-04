/*
 * BVHPatch.h
 *
 *  Created on: 11/03/2016
 *      Author: amutuberria
 */

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
