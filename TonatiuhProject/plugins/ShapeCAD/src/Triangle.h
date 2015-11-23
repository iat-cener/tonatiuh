/*
 * Triangle.h
 *
 *  Created on: 03/11/2015
 *      Author: amutuberria
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "BBox.h"
#include "NormalVector.h"
#include "Point3D.h"
#include "Vector3D.h"

class DifferentialGeometry;
class Ray;

class Triangle
{

public:
	Triangle( Point3D v1, Point3D v2, Point3D v3, NormalVector normal );


	BBox GetBBox() const { return ( m_bbox ); } ;
	Point3D GetCentroid() const { return ( m_centoid ); };
	NormalVector GetNormal() const { return ( m_normal ); } ;
	Point3D GetVertex1() const { return ( m_v1 ); } ;
	Point3D GetVertex2() const { return ( m_v2 ); } ;
	Point3D GetVertex3() const { return ( m_v3 ); } ;

	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const;

private:

	BBox m_bbox;
	Point3D m_centoid;
	NormalVector m_normal;
	Point3D m_v1;
	Point3D m_v2;
	Point3D m_v3;


	Vector3D m_vE1;
	Vector3D m_vE2;
	Vector3D m_vW1;


};


#endif /* TRIANGLE_H_ */
