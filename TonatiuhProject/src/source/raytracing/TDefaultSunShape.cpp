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

#include <cmath>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/misc/SoState.h>
#include "TDefaultSunShape.h"

#include <cmath>
#include "tgc.h"
#include "tgf.h"
#include "Vector3D.h"

SO_NODE_SOURCE(TDefaultSunShape);

void TDefaultSunShape::initClass()
{
	SO_NODE_INIT_CLASS(TDefaultSunShape, TSunShape, "TSunShape");
}

TDefaultSunShape::TDefaultSunShape( )
{
	SO_NODE_CONSTRUCTOR(TDefaultSunShape);
}

TDefaultSunShape::~TDefaultSunShape()
{
}

//Light Interface
void TDefaultSunShape::GenerateRayDirection( Vector3D& /* direction */, RandomDeviate& /* rand */ ) const
{

}

double TDefaultSunShape::GetIrradiance() const
{
	return -1;
}

double TDefaultSunShape::GetThetaMax() const
{
	return 0.0;
}

SoNode* TDefaultSunShape::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	TDefaultSunShape* newSunShape = static_cast< TDefaultSunShape* >( SoNode::copy( copyConnections ) );

	return newSunShape;
}
