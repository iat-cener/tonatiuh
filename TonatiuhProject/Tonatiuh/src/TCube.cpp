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

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/misc/SoState.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "TCube.h"
#include "Trace.h"

using namespace std;

SO_NODE_SOURCE(TCube);

// Normals to the six sides of the cube
   SbVec3d TCube::frontNormal, TCube::rearNormal;
   SbVec3d TCube::leftNormal, TCube::rightNormal;
   SbVec3d TCube::baseNormal, TCube::topNormal;

void TCube::initClass()
{
	Trace trace("TCube::initClass", false );
	SO_NODE_INIT_CLASS(TCube, TShape, "TShape");
}

TCube::TCube()
{
	Trace trace("TCube::TCube", false );

	SO_NODE_CONSTRUCTOR(TCube);
	SO_NODE_ADD_FIELD(m_width, (2.0));
	SO_NODE_ADD_FIELD(m_height, (2.0));
	SO_NODE_ADD_FIELD(m_depth, (2.0));

	// If this is the first time the constructor is called, set
	// up the static normals.
	if ( SO_NODE_IS_FIRST_INSTANCE() )
	{
		frontNormal.setValue( 0.0, 0.0, 1.0 );
		rearNormal.setValue( 0.0, 0.0, -1.0 );
		leftNormal.setValue( 1.0, 0.0, 0.0 );
		rightNormal.setValue( -1.0, 0.0, 0.0 );
		baseNormal.setValue( 0.0, -1.0, 0.0 );
		topNormal.setValue( 0.0, 1.0, 0.0 );
	}
}

TCube::~TCube()
{

}


double TCube::GetArea() const
{
	Trace trace( "TCube::GetArea", false );

	double frontArea = m_width.getValue() * m_height.getValue();
	double leftArea = m_height.getValue() * m_depth.getValue();
	double baseArea = m_depth.getValue() * m_depth.getValue();

	return ( 2 * frontArea ) + ( 2 * leftArea ) + (2 * baseArea );

}

QString TCube::getIcon()
{
	Trace trace("TCube::getIcon", false );
	return ":/icons/tcube.png";
}

bool TCube::Intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const
{
	Trace trace( "TCube::IntersectP", false );

	//Yet to be implemented
	return false;
}

bool TCube::IntersectP( const Ray& /*objectRay*/ ) const
{
	Trace trace( "TCube::IntersectP", false );

	//Yet to be implemented
	return false;
}


void TCube::computeBBox(SoAction*, SbBox3f& box, SbVec3f& center)
{
	Trace trace( "TCube::computeBBox", false );

    // These points define the min and max extents of the box.
    SbVec3f min, max;

     // Compute the half-width, half-height, and half-depth of
     // the pyramid. We'll use this info to set the min and max
     // points.
    double halfWidth = m_width.getValue()/2.0;
    double halfHeight = m_height.getValue()/2.0;
    double halfDepth = m_depth.getValue()/2.0;

    min.setValue(-halfWidth, -halfHeight, -halfDepth);
    max.setValue(halfWidth, halfHeight, halfDepth);


    // Set the box to bound the two extreme points.
    box.setBounds(min, max);

   // This defines the "natural center" of the TCube. We could
   // define it to be the center of the base, if we want, but
   // let's just make it the center of the bounding box.
   center.setValue(0.0, 0.0, 0.0);
}

void TCube::generatePrimitives(SoAction *action)
{
	Trace trace( "TCube::generatePrimitives", false );

    // The TCube will generate 6 quads: 1 for each side
    // This variable is used to store each vertex.
    SoPrimitiveVertex pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

   // If we need to generate texture coordinates with a
   // function, we'll need an SoGLTextureCoordinateElement.
   // Otherwise, we'll set up the coordinates directly.
   const SoTextureCoordinateElement* tce = 0;
   SbVec4f texCoord;
   if ( useTexFunc )	tce = SoTextureCoordinateElement::getInstance( state );
   else {
     texCoord[2] = 0.0;
     texCoord[3] = 1.0;
   }


   // We need the size to adjust the coordinates.
   double halfWidth = m_width.getValue()/2.0;
   double halfHeight = m_height.getValue()/2.0;
   double halfDepth = m_depth.getValue()/2.0;

   // We'll use this macro to make the code easier. It uses the
   // "point" variable to store the primitive vertex's point.
   SbVec3f  point;


#define GEN_VERTEX(pv, x, y, z, s, t, normal)   \
     point.setValue(halfWidth  * x,             \
                halfHeight * y,                 \
                halfDepth  * z);                \
     if (useTexFunc)                            \
       texCoord = tce->get(SbVec3f( point ), SbVec3f( normal ) );      \
     else {                                     \
       texCoord[0] = s;                         \
       texCoord[1] = t;                         \
     }                                          \
     pv.setPoint(point);                        \
     pv.setNormal( SbVec3f( normal ) );         \
     pv.setTextureCoords(texCoord);             \
     shapeVertex(&pv)

     // We will generate two triangles for the base, as a
     // triangle strip.
     beginShape(action, QUADS);

     // Base: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, -1.0,  1.0, 1.0,  1.0, baseNormal);
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 0.0,  1.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 0.0,  0.0, baseNormal);
     GEN_VERTEX(pv, -1.0, -1.0,  1.0, 1.0,  0.0, baseNormal);

     // Top: left front, left rear, right front, right rear
     GEN_VERTEX(pv,  1.0, 1.0,  1.0, 1.0,  1.0, topNormal);
     GEN_VERTEX(pv,  1.0, 1.0, -1.0, 0.0,  1.0, topNormal);
     GEN_VERTEX(pv, -1.0, 1.0, -1.0, 0.0,  0.0, topNormal);
     GEN_VERTEX(pv, -1.0, 1.0,  1.0, 1.0,  0.0, topNormal);

     // Front
     GEN_VERTEX(pv,  1.0, -1.0, 1.0, 1.0,  1.0, frontNormal);
     GEN_VERTEX(pv,  1.0,  1.0, 1.0, 0.0,  1.0, frontNormal);
     GEN_VERTEX(pv, -1.0,  1.0, 1.0, 0.0,  0.0, frontNormal);
     GEN_VERTEX(pv, -1.0, -1.0, 1.0, 1.0,  0.0, frontNormal);

	  // Rear
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 1.0,  1.0, frontNormal);
     GEN_VERTEX(pv,  1.0,  1.0, -1.0, 0.0,  1.0, frontNormal);
     GEN_VERTEX(pv, -1.0,  1.0, -1.0, 0.0,  0.0, frontNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 1.0,  0.0, frontNormal);

     //Left
     GEN_VERTEX(pv,  1.0, -1.0,  1.0, 1.0,  1.0, leftNormal);
     GEN_VERTEX(pv,  1.0,  1.0,  1.0, 0.0, 	1.0, leftNormal);
     GEN_VERTEX(pv,  1.0,  1.0, -1.0, 0.0,  0.0, leftNormal);
     GEN_VERTEX(pv,  1.0, -1.0, -1.0, 1.0,  0.0, leftNormal);

	  //Right
     GEN_VERTEX(pv, -1.0, -1.0,  1.0, 1.0,  1.0, leftNormal);
     GEN_VERTEX(pv, -1.0,  1.0,  1.0, 0.0,  1.0, leftNormal);
     GEN_VERTEX(pv, -1.0,  1.0, -1.0, 0.0,  0.0, leftNormal);
     GEN_VERTEX(pv, -1.0, -1.0, -1.0, 1.0,  0.0, leftNormal);

     endShape();

}

Point3D TCube::Sample( double /*u1*/, double /*u2*/ ) const
{
	Trace trace( "TCube::Sample", false );

	//Yet to be implemented
	return Point3D( 0, 0, 0 );
}

Point3D TCube::GetPoint3D( double /*u1*/, double /*u2*/ ) const
{
	Trace trace( "TCube::GetPoint3D", false );

	//Yet to be implemented
	return Point3D( 0, 0, 0 );
}

NormalVector TCube::GetNormal(double /*u*/, double /*v*/ ) const
{
	Trace trace( "TCube::GetNormal", false );
	return NormalVector( 0, 1, 0 );
}

