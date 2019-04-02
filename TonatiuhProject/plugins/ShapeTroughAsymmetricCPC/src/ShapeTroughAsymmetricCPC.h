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

#ifndef SHAPETROUGHASYMMETRICCPC_H_
#define SHAPETROUGHASYMMETRICCPC_H_

#include <vector>

#include <QString>

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "trt.h"
#include "TShape.h"

class Vector3D;

class ShapeTroughAsymmetricCPC: public TShape
{
	SO_NODE_HEADER(ShapeTroughAsymmetricCPC);

public:
	ShapeTroughAsymmetricCPC( );
	static void initClass();

	double GetArea() const;
	BBox GetBBox() const;
	QString GetIcon() const;
	double GetVolume() const;

	bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v) const;

	trt::TONATIUH_REAL rInt;
	trt::TONATIUH_REAL rExt;
	trt::TONATIUH_REAL acceptanceAngleCW;
	trt::TONATIUH_REAL acceptanceAngleCCW;
	trt::TONATIUH_REAL truncationAngle;
	trt::TONATIUH_REAL truncationOrigin;
	trt::TONATIUH_REAL length;


protected:
	static void updateInternalValues( void *data, SoSensor *);

	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;

	void computeBBox( SoAction* action, SbBox3f& box, SbVec3f& center);
	void generatePrimitives(SoAction *action);
	virtual ~ShapeTroughAsymmetricCPC();

private:
	Vector3D GetDPDURight( double acceptanceAngle, double theta ) const;
	Vector3D GetDPDU( double u, double v ) const;
	Vector3D GetD2PDUURight( double acceptanceAngle, double theta ) const;
	Vector3D GetD2PDUU( double u, double v ) const;

	void SetInternalValues();
	double ConcentratorProfileX( double theta ) const;
	double ConcentratorProfileY( double theta ) const;

	double TDeviation( const Ray ray, double theta, double acceptanceAngle ) const;
	std::vector<double> FindRigthRoots( const Ray ray, double thetaStart , double thetaEnd , double acceptanceAngle ) const;
	std::vector<double> FindRoots( const Ray ray ) const;
	std::vector<double> FindThits( const Ray ray, const std::vector<double> roots ) const;

	double m_tangentAngle;
	double m_thetaZero;
	double m_thetaMin;
	double m_thetaMax;
};

#endif /*SHAPETROUGHASYMMETRICCPC_H_*/
