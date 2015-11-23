/*
 * ShapeCAD.h
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */

#ifndef SHAPECAD_H_
#define SHAPECAD_H_

#include <vector>

#include <Inventor/sensors/SoFieldSensor.h>

#include "BVH.h"
#include "Point3D.h"
#include "NormalVector.h"
#include "trt.h"
#include "Triangle.h"
#include "TShape.h"

/*! *****************************
 * class ShapeCAD
 * **************************** */

class ShapeCAD : public TShape
{
	SO_NODE_HEADER(ShapeCAD);

	/*! *****************************
	 * class sorter
	 * **************************** */
	class Sorter {
	public:
		Sorter( Point3D referencePoint) : m_referencePoint( referencePoint) {}
	      bool operator()( const Triangle* t1, const Triangle* t2)
	      {
	    	  Point3D cT1 = t1->GetCentroid();
	    	  double dT1 = Distance( cT1, m_referencePoint );
	    	  Point3D cT2 = t2->GetCentroid();
	    	  double dT2 = Distance( cT2, m_referencePoint );

	    	  if( dT1 < dT2 )	return ( true );
	    	  return ( false );
	      }

	private:
	      Point3D m_referencePoint;
	};

public:
	ShapeCAD( );
	static void initClass();
	double GetArea() const;
	double GetVolume() const {return 0.0;};
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	bool SetFacetList( std::vector< Triangle* > triangleList );

	int	getFields(SoFieldList & fields) const;


protected:
	static void updateTrinaglesList(void *data, SoSensor *);
	//bool Near( const Triangle* t1, const Triangle* t2, Point3D referencePoint ) const;
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	void generatePrimitives(SoAction *action);
	virtual ~ShapeCAD();



private:

	trt::TONATIUH_CONTAINERREALVECTOR3 v1VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 v2VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 v3VertexList;
	trt::TONATIUH_CONTAINERREALVECTOR3 normalVertexList;

	std::vector< Triangle*> m_pTriangleList;

	SoFieldSensor* m_v1Sensor;
	SoFieldSensor* m_v2Sensor;
	SoFieldSensor* m_v3Sensor;
	SoFieldSensor* m_normalSensor;

/*
	double m_xMin;
	double m_xMax;
	double m_yMin;
	double m_yMax;
	double m_zMin;
	double m_zMax;
	*/


	BVH* m_pBVH;

};




#endif /* SHAPECAD_H_ */
