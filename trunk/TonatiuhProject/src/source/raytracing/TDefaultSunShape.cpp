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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
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

SoNode* TDefaultSunShape::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	TDefaultSunShape* newSunShape = static_cast< TDefaultSunShape* >( SoNode::copy( copyConnections ) );

	return newSunShape;
}
