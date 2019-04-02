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

#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

#include "BezierPatch.h"
#include "Point3D.h"
#include "ShapeBezierSurfaceFactory.h"

QString ShapeBezierSurfaceFactory::TShapeName() const
{
	return QString("Bezier_Patch");
}

QIcon ShapeBezierSurfaceFactory::TShapeIcon() const
{
	return QIcon( ":/icons/ShapeBezierSurface.png" );
}

ShapeBezierSurface* ShapeBezierSurfaceFactory::CreateTShape( ) const
{

	static bool firstTime = true;
	if ( firstTime )
	{
	    ShapeBezierSurface::initClass();
	    firstTime = false;
	}
	else
	{

		QString fileName = QFileDialog::getOpenFileName( 0, "Open File", ".", "Text (*.txt )" );

		if( !IsValidInputDataFile( fileName ) )
		{
			QMessageBox::warning( 0, "Error", "Can not open points data file" );
			return 0;
		}

		std::vector< Point3D > pointsListData;
		int nUCurves;
		int nVCurves;

		bool validData = ReadInputDataFile( fileName, &pointsListData, &nUCurves, &nVCurves );
		if( !validData )
		{
			QMessageBox::warning( 0, "Error", "Invalid data in file error" );
			return 0;
		}

		ShapeBezierSurface* shape = new ShapeBezierSurface;
		shape->DefineSurfacePatches( pointsListData, nUCurves, nVCurves );

		return shape;
	}
	return 0;
}

bool ShapeBezierSurfaceFactory::IsValidInputDataFile( QString fileName ) const
{
	if( fileName.isEmpty() )	return false;

	QFile file( fileName );
	if( !file.exists() )	return false;
	if( !file.open( QIODevice::ReadOnly ) )	return false;

	file.close();
	return true;
}

bool ShapeBezierSurfaceFactory::ReadInputDataFile( QString fileName, std::vector< Point3D >* inputData, int* nUCurves, int* nVCurves  ) const
{
	std::vector< Point3D > surfaceData;

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
			surfaceData.push_back( Point3D( curveData[ 3 * j].toDouble(),curveData[ 3* j +1  ].toDouble(),curveData[ 3* j + 2 ].toDouble() ) );
	 }

	 inputfile.close();

	 *nVCurves = vCurves;
	 *nUCurves = surfaceData.size() / *nVCurves;

	 *inputData = surfaceData;
	 return true;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(ShapeBezierSurface, ShapeBezierSurfaceFactory)
#endif
