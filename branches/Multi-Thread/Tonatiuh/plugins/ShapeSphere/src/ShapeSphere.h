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

#ifndef SHAPESPHERE_H_
#define SHAPESPHERE_H_

#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>

#include "TShape.h"
#include "tpt.h"

class SoSensor;

class ShapeSphere : public TShape
{
	SO_NODE_HEADER(ShapeSphere);

public:
	enum Side{
		INSIDE = 0,
		OUTSIDE   = 1,
	};

	ShapeSphere( );
	static void initClass();
    SoNode* copy( SbBool copyConnections ) const;
	double GetArea() const;
    QString getIcon();

    Point3D Sample( double u1, double u2 ) const;

	bool Intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg ) const;
	bool IntersectP( const Ray &ray ) const;

	tpt::TONATIUH_REAL radius;
	tpt::TONATIUH_REAL yMax;
	tpt::TONATIUH_REAL yMin;
	tpt::TONATIUH_REAL phiMax;
	SoSFEnum activeSide;

protected:
	static void updateRadius(void *data, SoSensor *);
	static void updateYMin(void *data, SoSensor *);
	static void updateYMax(void *data, SoSensor *);
	static void updatePhiMax(void *data, SoSensor *);

	bool OutOfRange( double u, double v ) const;
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;

	virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
	virtual void generatePrimitives(SoAction *action);
   	virtual ~ShapeSphere();

private:
	double m_lastValidYMax;
	double m_lastValidYMin;
};

#endif /*SHAPESPHERE_H_*/
