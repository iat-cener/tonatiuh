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
***************************************************************************/

#include <iostream>
#include <algorithm>

#include <QFileDialog>
#include <QTextStream>

#include <Inventor/sensors/SoFieldSensor.h>


#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>


#include "BBox.h"
#include "BezierPatch.h"
#include "BVHPatch.h"
#include "Curve.h"
#include "CurveNetwork.h"
#include "gf.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeBezierSurface.h"



SO_NODE_SOURCE(ShapeBezierSurface);

void ShapeBezierSurface::initClass()
{
	SO_NODE_INIT_CLASS(ShapeBezierSurface, TShape, "TShape");
}

ShapeBezierSurface::ShapeBezierSurface( )
:m_pBVH( 0 )
{
	SO_NODE_CONSTRUCTOR(ShapeBezierSurface);

	SO_NODE_ADD_FIELD( m_pointsList, (0, 0, 0 ) );
	SO_NODE_ADD_FIELD( m_nOfUCurves, ( 0 ) );
	SO_NODE_ADD_FIELD( m_nOfVCurves, (0  ) );

	m_pPointsSensor = new SoFieldSensor(updatePatchesList, this);
	m_pPointsSensor->setPriority( 0 );
	m_pPointsSensor->attach( &m_pointsList );

	m_pUSensor = new SoFieldSensor(updatePatchesList, this);
	m_pUSensor->setPriority( 0 );
	m_pUSensor->attach( &m_nOfUCurves );

	m_pVSensor = new SoFieldSensor(updatePatchesList, this);
	m_pVSensor->setPriority( 0 );
	m_pVSensor->attach( &m_nOfVCurves );

}

ShapeBezierSurface::~ShapeBezierSurface()
{
	delete m_pBVH;

	if( m_surfacesVector.size() > 0 )
	{
		for( unsigned int f = 0; f < m_surfacesVector.size(); f++ )
		{
			delete m_surfacesVector[f];
		}
	}
	m_surfacesVector.clear();

	delete m_pPointsSensor;
	delete m_pUSensor;
	delete m_pVSensor;
}

QString ShapeBezierSurface::GetIcon() const
{
	return ":/icons/ShapeBezierSurface.png";
}

double ShapeBezierSurface::GetArea() const
{
	return -1;
}

BBox ShapeBezierSurface::GetBBox() const
{
	if( m_pBVH )
		return ( m_pBVH->GetBBox() );

	return ( BBox( ) );
}

bool ShapeBezierSurface::DefineSurfacePatches( std::vector< Point3D > inputData, int nUCurves, int nVCurves )
{

	m_pPointsSensor->detach();
	m_pUSensor->detach();
	m_pVSensor->detach();

	if( m_surfacesVector.size() > 0 )
	{
		for( unsigned int f = 0; f < m_surfacesVector.size(); f++ )
		{
			delete m_surfacesVector[f];
			m_surfacesVector[f] = 0;
		}
	}
	m_surfacesVector.clear();


	m_nOfUCurves.setValue( nUCurves );
	m_nOfVCurves.setValue( nVCurves );

	for( unsigned int p = 0; p < inputData.size(); p++ )
	{
		Point3D point = inputData[p];
		m_pointsList.set1Value( p, point.x, point.y, point.z);
	}

	int k = 4;

	double nPointsPerUCurve = nVCurves;
	double nPointsPerVCurve = nUCurves;

	CurveNetwork* curveNetwork = new CurveNetwork();

	std::vector< double > uKnotsVector;
	int nU = nPointsPerUCurve +1;
	for( int i = 0; i <= ( nU+ k ) ; i++ )
	{
		 if( i < k ) uKnotsVector.push_back( 0.0 );
		 else if( i > nU ) uKnotsVector.push_back( nU- k + 2 );
		 else uKnotsVector.push_back( i -k + 1 );
	 }

	for( int i = 0; i < nUCurves; i++ )
	{
		 std::vector< Point3D > data;
		 for( int j = 0; j < nPointsPerUCurve; j++)
		 {
			 data.push_back( inputData[nPointsPerUCurve * i + j ] );
		 }

		 Curve* uCurve = new Curve( data, uKnotsVector );
		 curveNetwork->AddUCurve( uCurve );
	 }


	std::vector< double > vKnotsVector;
	int nV = nPointsPerVCurve + 1;
	 for( int i = 0; i <= ( nV+ k ) ; i++ )
	 {
		 if( i < k ) vKnotsVector.push_back(0.0 );
		 else if( i > nV ) vKnotsVector.push_back( nV - k + 2 );
		 else vKnotsVector.push_back( i -k + 1 );
	 }

	 for( int i = 0; i < nVCurves; i++ )
	 {
		 std::vector< Point3D > data;
		 for( int j = 0; j < nPointsPerVCurve; j++)
		 {
			 data.push_back( inputData[i + nPointsPerUCurve * j ] );
		 }

		 Curve* vCurve = new Curve( data, vKnotsVector );
		 curveNetwork->AddVCurve( vCurve );
	 }

	m_surfacesVector = curveNetwork->GetSurface();


	if( m_pBVH )
	{
		delete m_pBVH;
		m_pBVH = 0;
	}

	m_pBVH = new BVHPatch( &m_surfacesVector, 1 );


	m_pPointsSensor->setPriority( 0 );
	m_pPointsSensor->attach( &m_pointsList );
	m_pUSensor->setPriority( 0 );
	m_pUSensor->attach( &m_nOfUCurves );
	m_pVSensor->setPriority( 0 );
	m_pVSensor->attach( &m_nOfVCurves );


	return ( true );


}



bool ShapeBezierSurface::Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg) const
{
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

bool ShapeBezierSurface::IntersectP( const Ray& worldRay ) const
{
	return ( Intersect( worldRay, 0, 0 ) );
}

Point3D ShapeBezierSurface::Sample( double /*u*/, double /*v*/ ) const
{
	return Point3D();
}

/*
void ShapeBezierSurface::GLRender( SoGLRenderAction* action )
{
	for( int i = 0; i < m_surfacesVector.size(); ++i )
		m_surfacesVector[i]->GLRender(action);

}
*/

void ShapeBezierSurface::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{

	BBox bBox = GetBBox();

	// These points define the min and max extents of the box.
    SbVec3f min, max;

    min.setValue( bBox.pMin.x, bBox.pMin.y, bBox.pMin.z );
    max.setValue( bBox.pMax.x, bBox.pMax.y, bBox.pMax.z );

    // Set the box to bound the two extreme points.
    box.setBounds(min, max);
}

void ShapeBezierSurface::generatePrimitives(SoAction *action)
{

	//std::cout<<"ShapeBezierSurface::generatePrimitives"<<std::endl;

	//for( unsigned int i = 0; i < m_surfacesVector.size(); ++i )
	//m_surfacesVector[i]->GeneratePrimitives( action);
	//std::cout<<"END ShapeBezierSurface::generatePrimitives"<<std::endl;

	 SoPrimitiveVertex   pv;
	    SoState  *state = action->getState();

	    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
	                          SoTextureCoordinateElement::FUNCTION );

	    const SoTextureCoordinateElement* tce = 0;
	    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	    const int sNumOfRows = 30;
	    const int sNumOfColumns = 30;

	    float complexity = this->getComplexityValue(action);

	 	int rows = sNumOfRows * 0.5* complexity; // Number of points per row
	    int columns = sNumOfColumns * 0.5 * complexity; // Number of points per column


	    if (rows < 4) rows = 4;
	    if (columns < 4) columns = 4;
	    if (columns > 128) columns = 128;


		beginShape(action, QUADS );

		//for( unsigned int p = 0; p < m_surfacesVector.size(); p++ )
		for( unsigned int p = 0; p < m_surfacesVector.size(); p++ )
		{
			const int totalPoints = (rows)*(columns); // Total points in the grid
			float vertex[totalPoints][6];

			int h = 0;
			double ui = 0;
			double vj = 0;

			for (int i = 0; i < rows; ++i )
			{
				ui =( 1.0 /(double)(rows-1) ) * i;
				for ( int j = 0 ; j < columns ; ++j )
				{
					vj = ( 1.0 /(double)(columns-1) ) * j;

					Point3D point = m_surfacesVector[p]->GetPoint3D(ui, vj);
					NormalVector normal = m_surfacesVector[p]->GetNormal(ui, vj);
					vertex[h][0] = point.x;
					vertex[h][1] = point.y;
					vertex[h][2] = point.z;
					vertex[h][3] = normal.x;
					vertex[h][4] = normal.y;
					vertex[h][5] = normal.z;

					pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
					h++; //Increase h to the next point.
					}
			}

			float u = 1;
			float v = 1;
			beginShape(action, QUADS );
			for( int irow = 0; irow < (rows-1); ++irow )
			{
				for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
				{
					int index0 = irow*columns + icolumn;
					SbVec3f  point0( vertex[index0][0], vertex[index0][1],  vertex[index0][2] );
					SbVec3f normal0(vertex[index0][3], vertex[index0][4], vertex[index0][5] );
					SbVec4f texCoord0 = useTexFunc ? tce->get(point0, normal0): SbVec4f( u,v, 0.0, 1.0 );
					pv.setPoint(point0);
					pv.setNormal(normal0);
					pv.setTextureCoords(texCoord0);
					shapeVertex(&pv);

					int index1 = index0 + 1;
					SbVec3f  point1( vertex[index1][0], vertex[index1][1],  vertex[index1][2] );
					SbVec3f normal1(vertex[index1][3], vertex[index1][4], vertex[index1][5] );
					SbVec4f texCoord1 = useTexFunc ? tce->get(point1, normal1): SbVec4f( u,v, 0.0, 1.0 );
					pv.setPoint(point1);
					pv.setNormal(normal1);
					pv.setTextureCoords(texCoord1);
					shapeVertex(&pv);

					int index3 = index0 + columns;
					int index2 = index3 + 1;

					SbVec3f  point2( vertex[index2][0], vertex[index2][1],  vertex[index2][2] );
					SbVec3f normal2(vertex[index2][3], vertex[index2][4], vertex[index2][5] );
					SbVec4f texCoord2 = useTexFunc ? tce->get(point2, normal2): SbVec4f( u,v, 0.0, 1.0 );
					pv.setPoint(point2);
					pv.setNormal(normal2);
					pv.setTextureCoords(texCoord2);
					shapeVertex(&pv);

					SbVec3f  point3( vertex[index3][0], vertex[index3][1],  vertex[index3][2] );
					SbVec3f normal3(vertex[index3][3], vertex[index3][4], vertex[index3][5] );
					SbVec4f texCoord3 = useTexFunc ? tce->get(point3, normal3): SbVec4f( u,v, 0.0, 1.0 );
					pv.setPoint(point3);
					pv.setNormal(normal3);
					pv.setTextureCoords(texCoord3);
					shapeVertex(&pv);
				}
			}
			endShape();
		}


}


void ShapeBezierSurface::updatePatchesList( void *data, SoSensor* )
{
	ShapeBezierSurface* shapeBezier = (ShapeBezierSurface *) data;
	if( shapeBezier->m_surfacesVector.size() > 0 )
	{
		for( unsigned int f = 0; f < shapeBezier->m_surfacesVector.size(); f++ )
		{
			delete ( shapeBezier->m_surfacesVector[f] );
			shapeBezier->m_surfacesVector[f] = 0;
		}
	}
	shapeBezier->m_surfacesVector.clear();

	if( shapeBezier->m_pBVH )
	{
		delete shapeBezier->m_pBVH;
		shapeBezier->m_pBVH = 0;
	}


	int pointsSize = shapeBezier->m_pointsList.getNum();

	int nUCurves = shapeBezier->m_nOfUCurves.getValue();
	int nVCurves = shapeBezier->m_nOfVCurves.getValue();
	if( ( pointsSize > 0 ) &&
			( nUCurves > 0 ) &&
			( nVCurves > 0 )   )
	{

		int k = 4;
		double nPointsPerUCurve = shapeBezier->m_nOfVCurves.getValue();
		double nPointsPerVCurve = shapeBezier->m_nOfUCurves.getValue();
		CurveNetwork curveNetwork;
		std::vector< double > uKnotsVector;
		int nU = nPointsPerUCurve +1;
		for( int i = 0; i <= ( nU+ k ) ; i++ )
		{
			if( i < k ) uKnotsVector.push_back(  0.0 );
			else if( i > nU ) uKnotsVector.push_back( nU- k + 2  );
			else uKnotsVector.push_back( i -k + 1 );
		}

		for( int i = 0; i < nUCurves; i++ )
		{
			std::vector< Point3D > data;
			for( int j = 0; j < nPointsPerUCurve; j++)
			{
				int pointIndex =nPointsPerUCurve * i + j;
				Point3D point = Point3D( shapeBezier->m_pointsList[pointIndex][0],
						shapeBezier->m_pointsList[pointIndex][1],
						shapeBezier->m_pointsList[pointIndex][2] );

				data.push_back( point );
			}

			Curve* uCurve = new Curve( data, uKnotsVector );
			curveNetwork.AddUCurve( uCurve );
		}

		std::vector< double > vKnotsVector;
		int nV = nPointsPerVCurve + 1;
		for( int i = 0; i <= ( nV+ k ) ; i++ )
		{
			if( i < k ) vKnotsVector.push_back( 0.0 );
			else if( i > nV ) vKnotsVector.push_back( nV - k + 2 );
			else vKnotsVector.push_back ( i -k + 1 );
		}

		for( int i = 0; i < nVCurves; i++ )
		{
			std::vector< Point3D > data;
			for( int j = 0; j < nPointsPerVCurve; j++)
			{

				int pointIndex = i + nPointsPerUCurve * j;
				Point3D point = Point3D( shapeBezier->m_pointsList[pointIndex][0],
						shapeBezier->m_pointsList[pointIndex][1],
						shapeBezier->m_pointsList[pointIndex][2] );

				data.push_back( point );
			}

			Curve* vCurve = new Curve( data, vKnotsVector );
			curveNetwork.AddVCurve( vCurve );
		}
		shapeBezier->m_surfacesVector = curveNetwork.GetSurface();


		shapeBezier->m_pBVH = new BVHPatch( &(shapeBezier->m_surfacesVector), 1);
	}


}

