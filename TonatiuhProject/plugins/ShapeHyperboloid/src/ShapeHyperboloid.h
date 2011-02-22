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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/


#ifndef ShapeHyperboloid_H_
#define ShapeHyperboloid_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "trt.h"

class Vector3D;

class ShapeHyperboloid : public TShape
{
	SO_NODE_HEADER(ShapeHyperboloid);

public:

	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	ShapeHyperboloid( );
	static void initClass();
	double GetArea() const;
	BBox GetBBox() const;
	QString GetIcon() const;

	bool Intersect( const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	Point3D Sample( double u, double v ) const;

	trt::TONATIUH_REAL focusLegth;
	trt::TONATIUH_REAL distanceTwoFocus;
	trt::TONATIUH_REAL reflectorMaxDiameter;
	SoSFEnum activeSide;



protected:
	bool OutOfRange( double u, double v ) const;
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;

	void generatePrimitives(SoAction *action);
	void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	virtual ~ShapeHyperboloid();

private:
	Vector3D Dpdu( double u, double v ) const;
	Vector3D Dpdv( double u, double v ) const;
};

#endif /* ShapeHyperboloid_H_ */
