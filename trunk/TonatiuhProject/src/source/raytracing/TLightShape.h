/*
 * TLightShape.h
 *
 *  Created on: 31/03/2011
 *      Author: amutuberria
 */

#ifndef TLIGHTSHAPE_H_
#define TLIGHTSHAPE_H_



#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

class Transform;
class TShapeKit;

class TLightShape : public SoShape
{
	SO_NODE_HEADER(TLightShape);

public:
	TLightShape( );
	static void initClass();
	double GetArea() const;
	double GetVolume() const {return 0.0;};

	bool IsIntoValidArea( Point3D point ) const;
	Point3D Sample( double u, double v ) const;
	void SetLightSourceArea( int h, int w, int** lightArea );

	trt::TONATIUH_REAL xMin;
	trt::TONATIUH_REAL xMax;
	trt::TONATIUH_REAL zMin;
	trt::TONATIUH_REAL zMax;
	trt::TONATIUH_REAL delta;

protected:
	Point3D GetPoint3D ( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;

	void generatePrimitives(SoAction *action);
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	~TLightShape();

	int m_heightElements;
	int** m_lightAreaMatrix;
	int m_widthElements;

};

#endif /* TLIGHTSHAPE_H_ */
