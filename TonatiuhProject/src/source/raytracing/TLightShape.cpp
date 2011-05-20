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

#include <iostream>
#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "TLightShape.h"
#include "tgf.h"
#include "tgc.h"
#include "Transform.h"
#include "TShapeKit.h"
#include "Vector3D.h"



SO_NODE_SOURCE(TLightShape);

void TLightShape::initClass()
{
	SO_NODE_INIT_CLASS(TLightShape, SoShape, "Shape");
}

TLightShape::TLightShape( )
:m_heightElements( 0 ),
 m_lightAreaMatrix( 0 ),
 m_widthElements( 0 )
{
	SO_NODE_CONSTRUCTOR(TLightShape);
	SO_NODE_ADD_FIELD( xMin, (-0.5) );
	SO_NODE_ADD_FIELD( xMax, (0.5) );
	SO_NODE_ADD_FIELD( zMin, (-0.5) );
	SO_NODE_ADD_FIELD( zMax, (0.5) );
	SO_NODE_ADD_FIELD( delta, (100.0) );
}

TLightShape::~TLightShape()
{
	for( int i = 0; i < m_heightElements; i++ )
		delete[] m_lightAreaMatrix[i];
	delete[] m_lightAreaMatrix;
}

double TLightShape::GetArea() const
{
	double width =  xMax.getValue() - xMin.getValue();
	double height = zMax.getValue() - zMin.getValue();
	return ( width * height );
}

bool TLightShape::IsIntoValidArea( Point3D point ) const
{
	if( point.x < xMin.getValue() || point.x > xMax.getValue() )
		return false;
	if( point.y < -tgc::Epsilon || point.y > tgc::Epsilon )
		return false;
	if( point.z < zMin.getValue() || point.z > zMax.getValue() )
		return false;

	double width =  xMax.getValue() - xMin.getValue();
	double pixelWidth = width / m_widthElements;
	int w = ( point.x - xMin.getValue() ) /pixelWidth;

	double height = zMax.getValue() - zMin.getValue();
	double pixelHeight = height / m_heightElements;
	int h = ( point.z - zMin.getValue() ) /pixelHeight;

	if(  m_lightAreaMatrix[h][w] == 0 )	return false;

	//std::cout<<"m_lightAreaMatrix[h][w] "<<m_lightAreaMatrix[h][w] <<std::endl;
	return true;
}

Point3D TLightShape::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

Point3D TLightShape::GetPoint3D (double u, double v) const
{
	if( OutOfRange( u, v ) ) 	tgf::SevereError("Function TLightShape::GetPoint3D called with invalid parameters" );

	double width =  xMax.getValue() - xMin.getValue();
	double height = zMax.getValue() - zMin.getValue();
	double x = ( u * width ) + xMin.getValue();
	double z = ( v * height ) + zMin.getValue();

	return Point3D( x, 0, z );
}

void TLightShape::SetLightSourceArea(  int h, int w, int** lightArea )
{

	if( m_lightAreaMatrix != 0 )
	{
		for( int i = 0; i < m_heightElements; i++ )
			delete[] m_lightAreaMatrix[i];
		delete[] m_lightAreaMatrix;
	}

	m_heightElements = h;
	m_widthElements = w;
	m_lightAreaMatrix = lightArea;
}

bool TLightShape::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void TLightShape::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( xMin.getValue(), 0.0, zMin.getValue() );
    max.setValue( xMax.getValue(), 0.0, zMax.getValue() );

    // Set the box to bound the two extreme points.
    box.setBounds( min, max );

}

void TLightShape::generatePrimitives(SoAction *action)
{
	SoPrimitiveVertex   pv;
	SoState  *state = action->getState();

	// See if we have to use a texture coordinate function,
	// rather than generating explicit texture coordinates.
	SbBool useTexFunc =  (SoTextureCoordinateElement::getType(state) == SoTextureCoordinateElement::FUNCTION);

	const SoTextureCoordinateElement* tce = 0;
	SbVec4f texCoord;
	if (useTexFunc)	tce = SoTextureCoordinateElement::getInstance(state);
	else
	{
		texCoord[2] = 0.0;
		texCoord[3] = 1.0;
	}

	SbVec3f normal( 0.0, -1.0, 0.0 );
	beginShape(action, TRIANGLE_STRIP);

	 //P1
	 SbVec3f point( xMin.getValue(), 0.0, zMax.getValue() );
	 if (useTexFunc )
		 texCoord = tce->get(point, normal);
	 else
	 {
		 texCoord[0] = 1.0;
		 texCoord[1] = 0.0;
	 }

	pv.setPoint( point );
	pv.setNormal( normal );
	pv.setTextureCoords( texCoord );
	shapeVertex( &pv );

	 //P2
	point = SbVec3f( xMin.getValue(), 0.0, zMin.getValue() );

	 if (useTexFunc )
		 texCoord = tce->get(point, normal);
	 else
	 {
		 texCoord[0] = 0.0;
		 texCoord[1] = 0.0;
	 }

	pv.setPoint( point );
	pv.setNormal( normal );
	pv.setTextureCoords( texCoord );
	shapeVertex( &pv );

	 //P3
	point = SbVec3f( xMax.getValue(), 0.0, zMax.getValue() );
	 if (useTexFunc )
		 texCoord = tce->get(point, normal);
	 else
	 {
		 texCoord[0] = 1.0;
		 texCoord[1] = 1.0;
	 }

	pv.setPoint( point );
	pv.setNormal( normal );
	pv.setTextureCoords( texCoord );
	shapeVertex( &pv );

	 //P4
	point = SbVec3f( xMax.getValue(), 0.0, zMin.getValue() );
	 if (useTexFunc )
		 texCoord = tce->get(point, normal);
	 else
	 {
		 texCoord[0] = 0.0;
		 texCoord[1] = 1.0;
	 }

	pv.setPoint( point );
	pv.setNormal( SbVec3f( 0.0, -1.0, 0.0 ) );
	pv.setTextureCoords( texCoord );
	shapeVertex( &pv );

	 endShape();

}
