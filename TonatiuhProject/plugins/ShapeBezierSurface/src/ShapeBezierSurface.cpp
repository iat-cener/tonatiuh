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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>
#include <algorithm>

#include <QFileDialog>
#include <QTextStream>

#include <Inventor/sensors/SoFieldSensor.h>

#include "BBox.h"
#include "BezierPatch.h"
#include "Curve.h"
#include "CurveNetwork.h"
#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeBezierSurface.h"



SO_NODE_SOURCE(ShapeBezierSurface);

void ShapeBezierSurface::initClass()
{
	SO_NODE_INIT_CLASS(ShapeBezierSurface, TShape, "TShape");
}

ShapeBezierSurface::ShapeBezierSurface( )
{
	SO_NODE_CONSTRUCTOR(ShapeBezierSurface);


	SO_NODE_ADD_FIELD( inputDataFile, ("") );


	SoFieldSensor* fileSensor = new SoFieldSensor( updateInputDataFile, this );
	fileSensor->setPriority( 0 );
	fileSensor->attach( &inputDataFile );

}

ShapeBezierSurface::~ShapeBezierSurface()
{
	m_surfacesVector.clear();
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
	BBox bBox;
	for (int i = 0; i < m_surfacesVector.size(); i++)
	{
		BBox pBox =  m_surfacesVector[i]->GetComputeBBox();
		bBox = Union( bBox, pBox );
	}
	return bBox;
}

void ShapeBezierSurface::DefineSurfacePatches( QVector< Point3D > inputData, int nUCurves, int nVCurves )
{
	int k = 4;

	double nPointsPerUCurve = nVCurves;
	double nPointsPerVCurve = nUCurves;

	CurveNetwork* curveNetwork = new CurveNetwork();

	QVector< double > uKnotsVector;
	int nU = nPointsPerUCurve +1;
	for( int i = 0; i <= ( nU+ k ) ; i++ )
	{
		 if( i < k ) uKnotsVector<< 0.0;
		 else if( i > nU ) uKnotsVector<<( nU- k + 2 );
		 else uKnotsVector<< ( i -k + 1 );
	 }

	for( int i = 0; i < nUCurves; i++ )
	{
		 QVector< Point3D > data;
		 for( int j = 0; j < nPointsPerUCurve; j++)
		 {
			 data << inputData[nPointsPerUCurve * i + j ];
		 }

		 Curve* uCurve = new Curve( data, uKnotsVector );
		 curveNetwork->AddUCurve( uCurve );
	 }


	QVector< double > vKnotsVector;
	int nV = nPointsPerVCurve + 1;
	 for( int i = 0; i <= ( nV+ k ) ; i++ )
	 {
		 if( i < k ) vKnotsVector<< 0.0;
		 else if( i > nV ) vKnotsVector<<( nV - k + 2 );
		 else vKnotsVector<< ( i -k + 1 );
	 }

	 for( int i = 0; i < nVCurves; i++ )
	 {
		 QVector< Point3D > data;
		 for( int j = 0; j < nPointsPerVCurve; j++)
		 {
			 data << inputData[i + nPointsPerUCurve * j ];
		 }

		 Curve* vCurve = new Curve( data, vKnotsVector );
		 curveNetwork->AddVCurve( vCurve );
	 }

	m_surfacesVector = curveNetwork->GetSurface();
}



bool ShapeBezierSurface::Intersect(const Ray& objectRay, double* tHit, DifferentialGeometry* dg) const
{
	double tHitInitial = objectRay.maxt;
	double tHitShape = objectRay.maxt;
	DifferentialGeometry dgShape;

	//int surface = 0;
	for( int surface = 0; surface < m_surfacesVector.size(); ++surface )
	{

		//Check if the ray intersects with the patch BoundingBox
		BBox bbox = m_surfacesVector[surface]->GetComputeBBox();
		bool intersectBBox = bbox.IntersectP( objectRay );

		if( intersectBBox )
		{
			double tHitPatch;
			DifferentialGeometry dgPatch;
			bool intersected = m_surfacesVector[surface]->Intersect( objectRay, &tHitPatch, &dgPatch );

			if( intersected && tHitShape > tHitPatch )
			{
				tHitShape = tHitPatch;
				dgShape = dgPatch;
			}
		}
	}

	if( tHitShape < tHitInitial )
	{
		dgShape.pShape = this;

		*tHit = tHitShape;
		*dg = dgShape;

		return true;
	}
	return false;

}

bool ShapeBezierSurface::IntersectP( const Ray& objectRay ) const
{
	for( int i = 0; i < m_surfacesVector.size(); ++i )
		if( m_surfacesVector[i]->IntersectP( objectRay ) ) return true;

	return false;
}

Point3D ShapeBezierSurface::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeBezierSurface::GLRender( SoGLRenderAction* action )
{
	for( int i = 0; i < m_surfacesVector.size(); ++i )
		m_surfacesVector[i]->GLRender(action);

}

Point3D ShapeBezierSurface::GetPoint3D( double u, double v ) const
{
	return Point3D();
}

NormalVector ShapeBezierSurface::GetNormal (double u, double v ) const
{
	return NormalVector();
}

void ShapeBezierSurface::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	box.makeEmpty();
	for (int i = 0; i < m_surfacesVector.size(); i++)
	{
		BBox pBox =  m_surfacesVector[i]->GetComputeBBox();
		SbVec3f pMin( pBox.pMin[0], pBox.pMin[1], pBox.pMin[2] );
		box.extendBy( pMin );
		SbVec3f pMax( pBox.pMax[0], pBox.pMax[1], pBox.pMax[2] );
		box.extendBy( pMax );
	}
}

void ShapeBezierSurface::generatePrimitives(SoAction *action)
{
	for( int i = 0; i < m_surfacesVector.size(); ++i )
		m_surfacesVector[i]->GeneratePrimitives(action);
}

void ShapeBezierSurface::updateInputDataFile(void *data, SoSensor *)
{
	ShapeBezierSurface* shape = (ShapeBezierSurface*)data;

	QString fileName( shape->inputDataFile.getValue().getString() );

	QVector< Point3D > fileData;
	int nUCurves;
	int nVCurves;

	if( !shape->IsValidInputDataFile( fileName ) || !shape->ReadInputDataFile( fileName, &fileData, &nUCurves, &nVCurves ) )
		shape->inputDataFile.setValue( shape->lastValidInputFile.toStdString().c_str() );
	else
	{
		//for( int patch = 0 ; patch < m_surfacesVector.size(); ++i )
		shape->m_surfacesVector.clear();
		shape->lastValidInputFile = fileName;
		shape->DefineSurfacePatches( fileData, nUCurves, nVCurves );
	}

}

bool ShapeBezierSurface::IsValidInputDataFile( QString fileName ) const
{
	if( fileName.isEmpty() )	return false;

	QFile file( fileName );
	if( !file.exists() )	return false;
	if( !file.open( QIODevice::ReadOnly ) )	return false;

	file.close();
	return true;
}

bool ShapeBezierSurface::ReadInputDataFile( QString fileName, QVector< Point3D >* inputData, int* nUCurves, int* nVCurves  ) const
{
	QVector< Point3D > surfaceData;

	QFile inputfile( fileName );
	if( !inputfile.open( QIODevice::ReadOnly ) ) return false;

	QTextStream in( &inputfile);

	int vCurves = 0;
	while( !in.atEnd() )
	{
		QString dataLine = in.readLine();
		QStringList curveData = dataLine.split(QRegExp("[\\s+{},]"), QString::SkipEmptyParts);
		if( curveData.size() % 3 != 0 )	 return false;
		int nPoints = curveData.size() / 3;

		if( vCurves > 0 && vCurves != nPoints ) return false;
		else if( vCurves == 0 )	vCurves = nPoints;

		for( int j = 0; j < nPoints; j++)
			surfaceData<<Point3D( curveData[ 3 * j].toDouble(),curveData[ 3* j +1  ].toDouble(),curveData[ 3* j + 2 ].toDouble() );
	 }

	 inputfile.close();

	 *nUCurves = surfaceData.size() / vCurves;
	 *nVCurves = vCurves;

	 *inputData = surfaceData;
	 return true;
}
