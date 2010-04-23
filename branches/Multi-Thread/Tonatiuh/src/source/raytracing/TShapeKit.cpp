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
Ray* TShapeKit::Intersect( const Ray& objectRay, bool* isShapeFront, RandomDeviate& rand ) const
{
	//The ray intersects with the BoundingBox
	//Transform the ray to call children intersect


	Ray* result = 0;

	if( shape.getValue() )
	{
		TShape* tshape = static_cast< TShape* >( shape.getValue() );

		double thit = 0.0;
		DifferentialGeometry* dg = new DifferentialGeometry;
		bool intersect = tshape->Intersect( objectRay, &thit, dg );
		*isShapeFront = dg->shapeFrontSide;
		if( intersect )
		{
			objectRay.maxt = thit;

			if ( appearance.getValue() )
			{
				SoAppearanceKit* soappearance = static_cast< SoAppearanceKit* > ( appearance.getValue() );
				if( soappearance->getPart( "material", false ) )
				{
					TMaterial* tmaterial = static_cast< TMaterial* > ( soappearance->getPart( "material", false ) );

					Ray* reflected = tmaterial->OutputRay( objectRay, dg, rand );
					if( reflected ) result = new Ray( *reflected );
					delete reflected;
				}
			}
		}
		delete dg;
	}
	return result;
}
