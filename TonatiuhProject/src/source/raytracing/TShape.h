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
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef TSHAPE_H_
#define TSHAPE_H_

#include <string>

#include <Inventor/nodes/SoShape.h>

#include "BBox.h"
#include "DifferentialGeometry.h"

/*!
 * @class TShape
 * @brief Abstract base class for all custom shapes in the scene.
 *
 * TShape defines the interface for 3D geometric objects that can be intersected and rendered.
 *
 * The subclasses of TShape must implement:
 * - Bounding box computation (`GetBBox`, `computeBBox`).
 * - Icon representation (`GetIcon`).
 * - Ray intersection tests (`IntersectP`, `Intersect`).
 * - Primitive generation for rendering (`generatePrimitives`).
 *
 * The class also provides an optional parameter validation mechanism via
 * `ValidateParameterValue`.
 */
class TShape : public SoShape
{
	SO_NODE_ABSTRACT_HEADER(TShape);

public:
    static void initClass();

	virtual BBox GetBBox() const = 0;
	virtual std::string GetIcon() const = 0;

	virtual bool IntersectP( const Ray& objectRay ) const = 0;
	virtual bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const = 0;

	virtual bool ValidateParamaterValue( std::string /*name*/, std::string /*value*/ ) const { return true; };

protected:
	virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center) = 0;
	virtual void generatePrimitives(SoAction *action) = 0;

    TShape();
    ~TShape();
};

#endif /*TSHAPE_H_*/
