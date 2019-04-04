/***************************************************************************
 Copyright (C) 2008 by the Tonatiuh Software Development Team.

 This file is part of Tonatiuh.

 Tonatiuh program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.


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

 Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola, Gilda Jimenez,
 Juana Amieva, Azael Mancillas, Cesar Cantu.
 ***************************************************************************/

#include <cmath>

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptValue>
#include <QTextStream>

#include <gtest/gtest.h>

#include "PluginManager.h"
#include "Point3D.h"
#include "ScriptRayTracer.h"
#include "tonatiuh_script.h"

double** binCounts( QVector< Point3D >  photons, int widthDivisions, int heightDivisions );
double m_area;
double m_numrays;

// Tests the Set method.
TEST(RayTracerTest, CreateTargetPhotonMap )
{
	/*QDir pluginsDirectory( qApp->applicationDirPath() );
	pluginsDirectory.cd( "plugins" );

	PluginManager pluginManager;
	pluginManager.LoadAvailablePlugins( pluginsDirectory );


	QDir testDirectory( TEST_DIR );
	QString testDirPath = testDirectory.absolutePath();

	QScriptEngine* interpreter = new QScriptEngine;

	QScriptValue tonatiuh = interpreter->newQObject( 0 );
	interpreter->globalObject().setProperty( "tonatiuh", tonatiuh );

	QObject* rayTracer = new ScriptRayTracer( pluginManager.GetPhotonMapFactories(), pluginManager.GetRandomDeviateFactories() );
	QScriptValue rayTracerValue = interpreter->newQObject( rayTracer );
	interpreter->globalObject().setProperty( "rayTracer", rayTracerValue );

	QString fileName = QDir( testDirPath ).absoluteFilePath( "RayTracerTest.tnhs" );
	QFile scriptFile( fileName );
	if( !scriptFile.open( QIODevice::ReadOnly) )
	{
		QString errorMessage = QString( "Cannot open file %1." ).arg( fileName );
		FAIL()<<errorMessage.toStdString();
	}


	QTextStream in( &scriptFile );
	QString program = in.readAll();
	scriptFile.close();

	int initialized = tonatiuh_script::init( interpreter );
	if( !initialized )
	{
		QString errorMessage( "Script Execution Error." );
		FAIL()<<errorMessage.toStdString();
	}

	QScriptValue rTValue = interpreter->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracerObject = ( ScriptRayTracer* ) rTValue.toQObject();
	rayTracerObject->SetDir( testDirectory.absolutePath() );

	QScriptSyntaxCheckResult checkResult = interpreter->checkSyntax( program );
	if( checkResult.state() != QScriptSyntaxCheckResult::Valid )
	{
		QString errorMessage = QString( "Script Syntaxis Error.\n"
				"Line: %1. %2" ).arg( QString::number( checkResult.errorLineNumber() ), checkResult.errorMessage () );
		FAIL()<<errorMessage.toStdString();
	}

	QScriptValue result = interpreter->evaluate( program );
	if( result.isError () )
	{
		QScriptValue lineNumber = result.property( "lineNumber"  );

		QString errorMessage = QString( "Script Execution Error. %1" ).arg( result.toString() );
		FAIL()<<errorMessage.toStdString();

	}
	m_area=rayTracerObject->GetArea();
	m_numrays=rayTracerObject->GetNumrays();
	*/
}

TEST(RayTracerTest, PowerPerPhoton )
{
	/*QDir testDirectory( TEST_DIR );
	QString testDirPath = testDirectory.absolutePath();

	QFile targetPhotonsFile( QDir( testDirPath ).absoluteFilePath( "targetSurfaceData.dat" ) );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		//FAIL();
		QString errorMessage = QString( "Cannont open file %1." ).arg(  QDir( testDirPath ).absoluteFilePath( "targetSurfaceData.dat" ) );
		FAIL()<<errorMessage.toStdString();
	}

	QDataStream in( &targetPhotonsFile );

	double nPhotons =m_numrays;
	double irradiance = 1000;
	double inputArea = m_area;

	double expectedWPhoton = ( ( irradiance * inputArea )/ nPhotons) ;

	double wPhoton;
	in>>wPhoton;

	targetPhotonsFile.close();

	EXPECT_PRED_FORMAT2(::testing::DoubleLE, ( wPhoton - expectedWPhoton ) / expectedWPhoton, 0.02);
	*/

}

TEST(RayTracerTest, TotalPower )
{
	/*QDir testDirectory( TEST_DIR );
	QString testDirPath = testDirectory.absolutePath();

	QString taragetPhotonsFileName = QDir( testDirPath ).absoluteFilePath( "targetSurfaceData.dat" );

	QFile targetPhotonsFile( taragetPhotonsFileName );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		QString errorMessage = QString( QLatin1String( "Cannont open file %1." ) ).arg(  taragetPhotonsFileName );
		FAIL()<<errorMessage.toStdString();

	}
	QDataStream in( &targetPhotonsFile );

	double wPhoton;
	in>>wPhoton;

	QVector< Point3D > photons;
	while( !in.atEnd() )
	{
		double id, prev, next;
		double x, y, z;
		double side;
		in>>id>>x>>y>>z>>side>>prev>>next;
		photons.push_back( Point3D( x, y, z) );
	}
	targetPhotonsFile.close();
    double irradiance=1000;
	double totalPower = ( wPhoton * photons.size() )/1000;
	double expectedTotalPower =((m_area*irradiance)/m_numrays)*photons.size()/1000; // kW

	double relativeError = ( totalPower - expectedTotalPower ) / expectedTotalPower;
	EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError, 0.02);
	*/

}

TEST(RayTracerTest, FluxMaximum )
{
	/*QDir testDirectory( TEST_DIR );
	QString testDirPath = testDirectory.absolutePath();

	QFile targetPhotonsFile( QDir( testDirPath ).absoluteFilePath( "targetSurfaceData.dat" ) );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		QString errorMessage = QString( "Cannont open file %1." ).arg( QDir( testDirPath ).absoluteFilePath( "targetSurfaceData.dat" ) );
		FAIL()<<errorMessage.toStdString();
	}
	QDataStream in( &targetPhotonsFile );

	double wPhoton;
	in>>wPhoton;

	QVector< Point3D > photons;
	while( !in.atEnd() )
	{
		double id, prev, next;
		double x, y, z;
		double side;
		in>>id>>x>>y>>z>>side>>prev>>next;
		photons.push_back( Point3D( x, y, z) );
	}
	targetPhotonsFile.close();

	int widthDivisions = 10;
	int heightDivisions = 10;

	double** bins = binCounts( photons, widthDivisions, heightDivisions );

	double max = 0;
	for( int i = 0; i < widthDivisions; i++ )
	{
		for( int j = 0; j < widthDivisions;j++ )
			if( ( ( bins[i][j]*wPhoton ) / 1000 ) > max ) max = ( bins[i][j]*wPhoton ) / 1000;
	}


	double expectedMaximum = 946.489; // kW/m2
	double relativeError = ( max - expectedMaximum ) / expectedMaximum;
	EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError, 0.02);
	*/
}

double** binCounts( QVector< Point3D > photons, int widthDivisions, int heightDivisions )
{
	double** bins = new double*[widthDivisions];
	for( int i = 0; i < widthDivisions; i++ )
	{
		bins[i] = new double[heightDivisions];
		for( int j = 0; j < widthDivisions;j++ )
			bins[i][j] = 0;
	}

	double targetWidth = 0.8;
	double targetHeight = 0.8;
	double elementWidth = targetWidth /widthDivisions;
	double elementHeight = targetHeight/heightDivisions;

	for( int photon = 0; photon < photons.size(); photon++ )
	{
		Point3D p = photons[photon];
		int i = floor( ( p.x + 0.5 * targetWidth ) / elementWidth );
		int j = floor( ( p.y + 0.5 * targetHeight ) / elementHeight );
		bins[i][j]++;
	}

	return bins;
}
