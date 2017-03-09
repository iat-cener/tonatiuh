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
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/

#include <algorithm>
#include <functional> // for std::bind

#include <QString>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>


#include "gc.h"
#include "gf.h"
#include "trt.h"

#include "BBox.h"
#include "DifferentialGeometry.h"
#include "NormalVector.h"
#include "Ray.h"
#include "ShapeCAD.h"
#include "Triangle.h"


/*! *****************************
 * class ShapeCAD
 * **************************** */

SO_NODE_SOURCE(ShapeCAD);

void ShapeCAD::initClass()
{

	SO_NODE_INIT_CLASS(ShapeCAD, TShape, "TShape");
}

ShapeCAD::ShapeCAD(  )
: m_pBVH( 0 )
{
	SO_NODE_CONSTRUCTOR(ShapeCAD);
	SO_NODE_ADD_FIELD( v1VertexList, (0, 0, 0 ) );
	SO_NODE_ADD_FIELD( v2VertexList, (0, 0, 0 ) );
	SO_NODE_ADD_FIELD( v3VertexList, (0, 0, 0 ) );
	SO_NODE_ADD_FIELD( normalVertexList, (0, 0, 0 ) );


	m_v1Sensor = new SoFieldSensor(updateTrinaglesList, this);
	m_v1Sensor->setPriority( 0 );
	m_v1Sensor->attach( &v1VertexList );
	m_v2Sensor = new SoFieldSensor(updateTrinaglesList, this);
	m_v2Sensor->setPriority( 0 );
	m_v2Sensor->attach( &v2VertexList );
	m_v3Sensor = new SoFieldSensor(updateTrinaglesList, this);
	m_v3Sensor->setPriority( 0 );
	m_v3Sensor->attach( &v3VertexList );
	m_normalSensor = new SoFieldSensor(updateTrinaglesList, this);
	m_normalSensor->setPriority( 0 );
	m_normalSensor->attach( &normalVertexList );
}

ShapeCAD::~ShapeCAD()
{
	delete m_pBVH;


	if( m_pTriangleList.size() > 0 )
	{
		for( unsigned int f = 0; f < m_pTriangleList.size(); f++ )
		{
			delete m_pTriangleList[f];
		}
	}
	m_pTriangleList.clear();

	delete m_v1Sensor;
	delete m_v2Sensor;
	delete m_v3Sensor;
	delete m_normalSensor;
}


double ShapeCAD::GetArea() const
{
	return -1;
}

/*!
 * Return the shape bounding box.
 */
BBox ShapeCAD::GetBBox() const
{

	//return BBox( Point3D( m_xMin, m_yMin, m_zMin ), Point3D( m_xMax, m_yMax, m_zMax ) );
	if( m_pBVH )
		return ( m_pBVH->GetBBox() );

	return ( BBox( ) );
}


QString ShapeCAD::GetIcon() const
{
	return ( QLatin1String( ":/icons/ShapeCAD.png" ) );
}

bool ShapeCAD::Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const
{
	/*
	BBox bbox = GetBBox();

	double t0;
	double t1;
	if( !bbox.IntersectP(objectRay, &t0, &t1 ) )	return ( false );

	Point3D bBoxPoint = objectRay( t0 );

	std::vector< Triangle* > sortedTrianglesList = m_pTriangleList;
	std::sort( sortedTrianglesList.begin(), sortedTrianglesList.end(), Sorter( bBoxPoint ) );


	double thit= objectRay.maxt;
	DifferentialGeometry dgShape;
	bool isIntersection = false;

	for( unsigned int t = 0; t < sortedTrianglesList.size(); t++ )
	{

		double tHitTriangle = objectRay.maxt;
		DifferentialGeometry dgTriangle;
		bool isTriangleIntersection = sortedTrianglesList[t]->Intersect( objectRay, &tHitTriangle, &dgTriangle );
		if( isTriangleIntersection && tHitTriangle < thit )
		{
			thit = tHitTriangle;
			dgShape = dgTriangle;
			isIntersection = true;

		}

	}

	if( !isIntersection )	return ( false );
	if( ( tHit == 0 ) && ( dg == 0 ) )	return ( true );
	if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeCAD::Intersect(...) called with null pointers" );

	*tHit = thit;
	*dg = dgShape;
	dg->pShape = this;


	return ( true );
	*/
	double tHitShape= objectRay.maxt;
	DifferentialGeometry dgShape;


	//Ray ray ( Point3D( -0.05, 11, 0.05), Vector3D( 0, -1, 0));
	//std::cout<<"ShapeCAD::Intersect objectRay: "<<objectRay.origin<<"\t "<<objectRay.direction()<<std::endl;
	if( !m_pBVH )	return ( false );

	if ( !m_pBVH->Intersect( objectRay, &tHitShape, &dgShape ) )	return ( false );

	if( ( tHit == 0 ) && ( dg == 0 ) )	return ( true );
	if( ( tHit == 0 ) || ( dg == 0 ) ) gf::SevereError( "Function ShapeCAD::Intersect(...) called with null pointers" );


	*tHit = tHitShape;
	*dg = dgShape;
	dg->pShape = this;

	return ( true );


}

bool ShapeCAD::IntersectP( const Ray& worldRay ) const
{
	return Intersect( worldRay, 0, 0 );
}

Point3D ShapeCAD::Sample( double /*u*/, double /*v*/ ) const
{
	//return GetPoint3D( u, v );
	return ( Point3D( 0.0, 0.0, 0.0 ) );
}


bool ShapeCAD:: SetFacetList( std::vector< Triangle* > triangleList )
{
	m_v1Sensor->detach();
	m_v2Sensor->detach();
	m_v3Sensor->detach();
	m_normalSensor->detach();


	if( m_pTriangleList.size() > 0 )
	{
		for( unsigned int f = 0; f < m_pTriangleList.size(); f++ )
		{
			delete m_pTriangleList[f];
		}
	}
	m_pTriangleList.clear();

	m_pTriangleList = triangleList;

	/*
	m_xMin = gc::Infinity;
	m_yMin = gc::Infinity;
	m_zMin = gc::Infinity;

	m_xMax = - gc::Infinity;
	m_yMax = - gc::Infinity;
	m_zMax = - gc::Infinity;
	*/

	for( unsigned int f = 0; f < triangleList.size(); f++ )
	{

		Triangle* facet = triangleList[f];

		Point3D v1 = facet->GetVertex1();
		Point3D v2 = facet->GetVertex2();
		Point3D v3 = facet->GetVertex3();
		v1VertexList.set1Value( f, v1.x, v1.y, v1.z);
		v2VertexList.set1Value( f, v2.x, v2.y, v2.z);
		v3VertexList.set1Value( f, v3.x, v3.y, v3.z);
		normalVertexList.set1Value( f, facet->GetNormal().x, facet->GetNormal().y, facet->GetNormal().z);

		/*
		if( v1.x < m_xMin ) m_xMin = v1.x;
		if( v2.x < m_xMin ) m_xMin = v2.x;
		if( v3.x < m_xMin ) m_xMin = v3.x;

		if( v1.y < m_yMin ) m_yMin = v1.y;
		if( v2.y < m_yMin ) m_yMin = v2.y;
		if( v3.y < m_yMin ) m_yMin = v3.y;

		if( v1.z < m_zMin ) m_zMin = v1.z;
		if( v2.z < m_zMin ) m_zMin = v2.z;
		if( v3.z < m_zMin ) m_zMin = v3.z;

		if( v1.x > m_xMax ) m_xMax = v1.x;
		if( v2.x > m_xMax ) m_xMax = v2.x;
		if( v3.x > m_xMax ) m_xMax = v3.x;

		if( v1.y > m_yMax ) m_yMax = v1.y;
		if( v2.y > m_yMax ) m_yMax = v2.y;
		if( v3.y > m_yMax ) m_yMax = v3.y;

		if( v1.z > m_zMax ) m_zMax = v1.z;
		if( v2.z > m_zMax ) m_zMax = v2.z;
		if( v3.z > m_zMax ) m_zMax = v3.z;
		*/

	}

	if( m_pBVH )
	{
		delete m_pBVH;
		m_pBVH = 0;
	}

	m_pBVH = new BVH( &m_pTriangleList, 1 );


	m_v1Sensor->setPriority( 0 );
	m_v1Sensor->attach( &v1VertexList );
	m_v2Sensor->setPriority( 0 );
	m_v2Sensor->attach( &v2VertexList );
	m_v3Sensor->setPriority( 0 );
	m_v3Sensor->attach( &v3VertexList );
	m_normalSensor->setPriority( 0 );
	m_normalSensor->attach( &normalVertexList );
	return ( true);
}

int ShapeCAD::getFields(SoFieldList & /*fields*/ ) const
{
	return 0;
}

void ShapeCAD::computeBBox(SoAction *, SbBox3f &box, SbVec3f& /*center*/ )
{


	BBox bBox = GetBBox();
	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);
}

void ShapeCAD::generatePrimitives(SoAction *action)
{

	SoPrimitiveVertex   pv;

	// Access the state from the action.
	SoState  *state = action->getState();

	// See if we have to use a texture coordinate function,
	// rather than generating explicit texture coordinates.
	SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) == SoTextureCoordinateElement::FUNCTION );

	// If we need to generate texture coordinates with a
	// function, we'll need an SoGLTextureCoordinateElement.
	// Otherwise, we'll set up the coordinates directly.
	const SoTextureCoordinateElement* tce = 0;
	if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	beginShape(action, TRIANGLES );

	for( int f = 0; f < v1VertexList.getNum(); f++ )
	{
		//SbVec3f aPoint = v1VertexList[f];
		SbVec3f aPoint( v1VertexList[f][0], v1VertexList[f][1], v1VertexList[f][2] );
		SbVec3f bPoint( v2VertexList[f][0], v2VertexList[f][1], v2VertexList[f][2] );
		SbVec3f cPoint( v3VertexList[f][0], v3VertexList[f][1], v3VertexList[f][2] );

		//SbVec3f dpdu =  bPoint - aPoint;
		//SbVec3f dpdv = cPoint - aPoint;
		SbVec3f normal( normalVertexList[f][0], normalVertexList[f][1], normalVertexList[f][2] );
		normal.normalize();


		//a Point
		SbVec4f aTexCoord = useTexFunc ? tce->get(aPoint, normal): SbVec4f( 0.0 ,0.0, 0.0, 1.0 );
		pv.setPoint(aPoint);
		pv.setNormal(normal);
		pv.setTextureCoords(aTexCoord);
		shapeVertex(&pv);

		//b Point
		SbVec4f bTexCoord = useTexFunc ? tce->get(bPoint, normal): SbVec4f( 1.0, 0.0, 0.0, 1.0 );
		pv.setPoint(bPoint);
		pv.setNormal(normal);
		pv.setTextureCoords(bTexCoord);
		shapeVertex(&pv);

		//a Point
		SbVec4f cTexCoord = useTexFunc ? tce->get(cPoint, normal): SbVec4f( 0.0, 1.0, 0.0, 1.0 );
		pv.setPoint(cPoint);
		pv.setNormal(normal);
		pv.setTextureCoords(cTexCoord);
		shapeVertex(&pv);

	}

    endShape();


}


void ShapeCAD::updateTrinaglesList( void* data, SoSensor* )
{

	ShapeCAD* shapeCAD = (ShapeCAD *) data;


	if( shapeCAD->m_pTriangleList.size() > 0 )
	{
		for( unsigned int f = 0; f < shapeCAD->m_pTriangleList.size(); f++ )
		{
			delete shapeCAD->m_pTriangleList[f];
		}
	}
	shapeCAD->m_pTriangleList.clear();

	if( shapeCAD->m_pBVH )
	{
		delete shapeCAD->m_pBVH;
		shapeCAD->m_pBVH = 0;
	}


	int v1Size = shapeCAD->v1VertexList.getNum();
	if( ( v1Size > 0 ) &&
			( shapeCAD->v2VertexList.getNum() == v1Size ) &&
			( shapeCAD->v3VertexList.getNum() == v1Size ) &&
			( shapeCAD->normalVertexList.getNum() == v1Size )  )
	{
		/*
		shapeCAD->m_xMin = gc::Infinity;
		shapeCAD->m_yMin = gc::Infinity;
		shapeCAD->m_zMin = gc::Infinity;

		shapeCAD->m_xMax = - gc::Infinity;
		shapeCAD->m_yMax = - gc::Infinity;
		shapeCAD->m_zMax = - gc::Infinity;
		*/

		for(  int f = 0; f < v1Size; f++ )
		{
			Point3D v1 = Point3D( shapeCAD->v1VertexList[f][0], shapeCAD->v1VertexList[f][1], shapeCAD->v1VertexList[f][2] );
			Point3D v2 = Point3D( shapeCAD->v2VertexList[f][0], shapeCAD->v2VertexList[f][1], shapeCAD->v2VertexList[f][2] );
			Point3D v3 = Point3D( shapeCAD->v3VertexList[f][0], shapeCAD->v3VertexList[f][1], shapeCAD->v3VertexList[f][2] );
			NormalVector normal = NormalVector( shapeCAD->normalVertexList[f][0], shapeCAD->normalVertexList[f][1], shapeCAD->normalVertexList[f][2] );
			Triangle* triangle = new Triangle( v1, v2, v3, normal );
			shapeCAD->m_pTriangleList.push_back( triangle );

			/*
			if( v1.x < shapeCAD->m_xMin )	shapeCAD->m_xMin = v1.x;
			if( v2.x < shapeCAD->m_xMin )	shapeCAD->m_xMin = v2.x;
			if( v3.x < shapeCAD->m_xMin )	shapeCAD->m_xMin = v3.x;

			if( v1.y < shapeCAD->m_yMin )	shapeCAD->m_yMin = v1.y;
			if( v2.y < shapeCAD->m_yMin )	shapeCAD->m_yMin = v2.y;
			if( v3.y < shapeCAD->m_yMin ) 	shapeCAD->m_yMin = v3.y;

			if( v1.z < shapeCAD->m_zMin )	shapeCAD->m_zMin = v1.z;
			if( v2.z < shapeCAD->m_zMin )	shapeCAD->m_zMin = v2.z;
			if( v3.z < shapeCAD->m_zMin )	shapeCAD->m_zMin = v3.z;

			if( v1.x > shapeCAD->m_xMax )	shapeCAD->m_xMax = v1.x;
			if( v2.x > shapeCAD->m_xMax )	shapeCAD->m_xMax = v2.x;
			if( v3.x > shapeCAD->m_xMax )	shapeCAD->m_xMax = v3.x;

			if( v1.y > shapeCAD->m_yMax )	shapeCAD->m_yMax = v1.y;
			if( v2.y > shapeCAD->m_yMax )	shapeCAD->m_yMax = v2.y;
			if( v3.y > shapeCAD->m_yMax )	shapeCAD->m_yMax = v3.y;

			if( v1.z > shapeCAD->m_zMax )	shapeCAD->m_zMax = v1.z;
			if( v2.z > shapeCAD->m_zMax )	shapeCAD->m_zMax = v2.z;
			if( v3.z > shapeCAD->m_zMax )	shapeCAD->m_zMax = v3.z;
			*/
		}
		shapeCAD->m_pBVH = new BVH( &shapeCAD->m_pTriangleList, 1);
	}
}

