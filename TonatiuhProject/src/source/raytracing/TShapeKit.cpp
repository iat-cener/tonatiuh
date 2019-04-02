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

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "TCube.h"
#include "TMaterial.h"
#include "TShape.h"
#include "TShapeKit.h"


SO_KIT_SOURCE(TShapeKit);

/**
 * Sets up initialization for data common to all instances of this class, like submitting necessary information to the Coin type system.
 */
void TShapeKit::initClass()
{
	SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

/**
 * Constructor.
 *
 */
TShapeKit::TShapeKit()
{

	SO_KIT_CONSTRUCTOR(TShapeKit);

	SO_KIT_CHANGE_ENTRY_TYPE(shape, TShape, TCube);
	SO_KIT_CHANGE_NULL_BY_DEFAULT(shape,TRUE);
	SO_KIT_INIT_INSTANCE();

	setPart("shape", NULL );

	//SoTransform* transform = new SoTransform;
	//setPart("transform",  NULL);

}
/*!
 * Destroys the TShapeKit object.
 */
TShapeKit::~TShapeKit()
{

}

/**
 * Check if ray intersects with the node.
 *
 */
bool TShapeKit::IntersectP( const Ray& ) const
{
	return false;
}

/**
 * Interect \a object ray with the shape and computed reflected ray. The \a object ray
 * is on shape local coordinates.
 *
 *Return the reflected ray. If the returned value is null, there is not reflected ray.
 */
Ray* TShapeKit::Intersect( const Ray& /* objectRay */, bool* /* isShapeFront */, RandomDeviate& /* rand */ ) const
{
	return 0;
}
