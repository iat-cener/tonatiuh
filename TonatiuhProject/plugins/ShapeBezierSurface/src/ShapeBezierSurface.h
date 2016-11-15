/***************************************************************************
Copyright (C) 2008 by the Ekhi Software Development Team.

This file is part of Ekhi.

Ekhi program is free software: you can redistribute it and/or modify
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

The development of Ekhi was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Ekhi under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Ekhi is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************** unsigned **********************************************************/

#ifndef SHAPEBEZIERPATCH_H_
#define SHAPEBEZIERPATCH_H_

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "Vector3D.h"
#include "TShape.h"
#include "trt.h"

class BezierPatch;
class BVHPatch;
class SoMFVec3d;
class SoSensor;


class ShapeBezierSurface : public TShape
{
	SO_NODE_HEADER(ShapeBezierSurface);

public:
    ShapeBezierSurface();

    static void initClass();

	double GetArea() const;
	double GetVolume() const { return 0.0;};
	BBox GetBBox() const;
	QString GetIcon() const;

	bool IntersectP( const Ray& worldRay ) const;
	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const;

	Point3D Sample( double u, double v ) const;

	bool DefineSurfacePatches( std::vector< Point3D > inputData, int nUCurves, int nVCurves );

	//void GLRender(SoGLRenderAction *action);


protected:
	void computeBBox( SoAction *action, SbBox3f &box, SbVec3f &center);
	void generatePrimitives(SoAction *action);

	static void updatePatchesList(void *data, SoSensor *);

	virtual ~ShapeBezierSurface();

private:
	trt::TONATIUH_CONTAINERREALVECTOR3 m_pointsList;
	SoSFInt32 m_nOfUCurves;
	SoSFInt32 m_nOfVCurves;
	std::vector< BezierPatch* > m_surfacesVector;

	SoFieldSensor* m_pPointsSensor;
	SoFieldSensor* m_pUSensor;
	SoFieldSensor* m_pVSensor;

	BVHPatch* m_pBVH;

	double m_tol;
};

#endif /* SHAPEBEZIERPATCH_H_ */
