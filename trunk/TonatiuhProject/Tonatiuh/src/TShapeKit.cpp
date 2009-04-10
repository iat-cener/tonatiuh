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

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "TCube.h"
#include "tgf.h"
#include "TDefaultPhotonMap.h"
#include "TMaterial.h"
#include "Trace.h"
#include "Transform.h"
#include "TPhotonMap.h"
#include "TShape.h"
#include "TShapeKit.h"


SO_KIT_SOURCE(TShapeKit);

void TShapeKit::initClass()
{
	SO_KIT_INIT_CLASS(TShapeKit, SoShapeKit, "ShapeKit");
}

TShapeKit::TShapeKit()
{

	SO_KIT_CONSTRUCTOR(TShapeKit);

	SO_KIT_CHANGE_ENTRY_TYPE(shape, TShape, TCube);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY( photonMap, TPhotonMap, TDefaultPhotonMap, TRUE, this, "", TRUE );
	SO_KIT_INIT_INSTANCE();

	setPart("shape", NULL );
	//setPart("tphotonmap", NULL);

	SoTransform* transform = new SoTransform;
	setPart("transform", transform);

}

TShapeKit::~TShapeKit()
{
	Trace trace("TShapeKit::~TShapeKit", false );
}

void TShapeKit::SetBoundigBox( SbXfBox3f boundingBox )
{
	m_boundigBox = boundingBox;
}


bool TShapeKit::IntersectP( const Ray& /*ray*/ ) const
{
	return false;
}

Ray* TShapeKit::Intersect( const Ray& ray, RandomDeviate& rand ) const
{
	Trace trace( "TShapeKit::Intersect", false );

	//Test if the ray intersects with this bounding box
	SbBox3f box= m_boundigBox.project();
	double t0 = ray.mint;
	double t1 = ray.maxt;

    for( int i = 0; i < 3; ++i )
    {
    	double invRayDir = 1.0 / ray.direction[i];
    	double tNear = ( box.getMin()[i] - ray.origin[i] ) * invRayDir;
    	double tFar = ( box.getMax()[i] - ray.origin[i] ) * invRayDir;
    	if( tNear > tFar ) std::swap( tNear, tFar );
    	t0 = tNear > t0 ? tNear : t0;
    	t1 = tFar < t1 ? tFar : t1;
    	if( t0 > t1 ) return 0;
    }

	Ray* result = 0;

	//The ray intersects with the BoundingBox
	//Transform the ray to call children intersect
	SoTransform* coinTransform = static_cast< SoTransform* > ( transform.getValue() );
	if( coinTransform )
	{
		SbMatrix coinMatrix;
		coinMatrix.setTransform( coinTransform->translation.getValue(),
						coinTransform->rotation.getValue(),
						coinTransform->scaleFactor.getValue(),
						coinTransform->scaleOrientation.getValue(),
						coinTransform->center.getValue() );

		Transform objectToWorld( coinMatrix[0][0], coinMatrix[1][0], coinMatrix[2][0], coinMatrix[3][0],
							coinMatrix[0][1], coinMatrix[1][1], coinMatrix[2][1], coinMatrix[3][1],
							coinMatrix[0][2], coinMatrix[1][2], coinMatrix[2][2], coinMatrix[3][2],
							coinMatrix[0][3], coinMatrix[1][3], coinMatrix[2][3], coinMatrix[3][3] );

		Transform worldToObject = objectToWorld.GetInverse();

		Ray objectRay( worldToObject( ray ) );
		objectRay.maxt = ray.maxt;

		TShape* tshape = static_cast< TShape* >( shape.getValue() );

		if( tshape )
		{
			double thit = 0.0;
			DifferentialGeometry* dg = new DifferentialGeometry;
			bool intersect = tshape->Intersect( objectRay, &thit, dg );
			if( intersect )
			{
				ray.maxt = thit;

				SoAppearanceKit* soappearance = dynamic_cast< SoAppearanceKit* > ( appearance.getValue() );
				if ( soappearance )
				{
				    TMaterial* tmaterial = dynamic_cast< TMaterial* > ( soappearance->getPart( "material", false ) );
				    if( tmaterial )
				    {
				    	Ray* reflected = tmaterial->GetReflectedRay( objectRay, dg, rand );
				        if( reflected ) result = new Ray( objectToWorld( *reflected ) );
				        delete reflected;
				    }
				}
			}
		    delete dg;
		}
	}
	return result;
}
