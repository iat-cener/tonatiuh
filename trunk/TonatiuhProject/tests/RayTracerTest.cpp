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

// Tests the Set method.
TEST(RayTracerTest, CreateTargetPhotonMap )
{
	QDir pluginsDirectory( qApp->applicationDirPath() );
	pluginsDirectory.cd( "plugins" );

	PluginManager pluginManager;
	pluginManager.LoadAvailablePlugins( pluginsDirectory );


	QDir testDirectory( qApp->applicationDirPath() );
	testDirectory.cd( "../../tests" );

	QScriptEngine* interpreter = new QScriptEngine;

	QScriptValue tonatiuh = interpreter->newQObject( 0 );
	interpreter->globalObject().setProperty( "tonatiuh", tonatiuh );

	QObject* rayTracer = new ScriptRayTracer( pluginManager.GetPhotonMapFactories(), pluginManager.GetRandomDeviateFactories() );
	QScriptValue rayTracerValue = interpreter->newQObject( rayTracer );
	interpreter->globalObject().setProperty( "rayTracer", rayTracerValue );

	QString fileName = testDirectory.absoluteFilePath( "RayTracerTest.tnhs" );
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
		QScriptValue lineNumber = result.property( "lineNumber");

		QString errorMessage = QString( "Script Execution Error.\nLine %1. %2" ).arg( QString::number( lineNumber.toNumber() ), result.toString() );
		FAIL()<<errorMessage.toStdString();

	}
}

TEST(RayTracerTest, PowerPerPhoton )
{
	QString targetDataFileName( "tests/targetSurfaceData.dat" );
	QFile targetPhotonsFile( targetDataFileName );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		//FAIL();
		QString errorMessage = QString( "Cannont open file %1." ).arg( targetDataFileName );
		FAIL()<<errorMessage.toStdString();
	}

	QDataStream in( &targetPhotonsFile );

	double nPhotons = 6000000;
	double irradiance = 1000;
	double inputArea = 8.5 * 9.25;

	double wPhoton;
	in>>wPhoton;

	targetPhotonsFile.close();
	EXPECT_DOUBLE_EQ( ( ( irradiance * inputArea )/ nPhotons) , wPhoton );

}

TEST(RayTracerTest, TotalPower )
{
	QFile targetPhotonsFile( "tests/targetSurfaceData.dat" );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		//FAIL();
		QMessageBox::warning( 0, QString( "RayTracerTest"), "Cannont open file" );
	}
	QDataStream in( &targetPhotonsFile );

	double wPhoton;
	in>>wPhoton;

	QVector< Point3D > photons;
	while( !in.atEnd() )
	{
		double id, prev, next;
		double x, y, z;
		in>>id>>x>>y>>z>>prev>>next;
		photons.push_back( Point3D( x, y, z) );
	}
	targetPhotonsFile.close();

	double totalPower = ( wPhoton * photons.size() )/1000;
	double expectedTotalPower = 9.76163; /* kW */

	double relativeError = ( totalPower - expectedTotalPower ) / expectedTotalPower;
	EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError, 0.02);

}

TEST(RayTracerTest, FluxMaximum )
{
	QFile targetPhotonsFile( "tests/targetSurfaceData.dat" );
	if( !targetPhotonsFile.open( QIODevice::ReadOnly ) )
	{
		//FAIL();
		QMessageBox::warning( 0, QString( "RayTracerTest"), "Cannont open file" );
	}
	QDataStream in( &targetPhotonsFile );

	double wPhoton;
	in>>wPhoton;

	QVector< Point3D > photons;
	while( !in.atEnd() )
	{
		double id, prev, next;
		double x, y, z;
		in>>id>>x>>y>>z>>prev>>next;
		photons.push_back( Point3D( x, y, z) );
	}
	targetPhotonsFile.close();

	int widthDivisions = 10;
	int heightDivisions = 10;

	//double bins[widthDivisions][heightDivisions];
	double** bins = binCounts( photons, widthDivisions, heightDivisions );

	double max = 0;
	for( int i = 0; i < widthDivisions; i++ )
	{
		for( int j = 0; j < widthDivisions;j++ )
			if( ( ( bins[i][j]*wPhoton ) / 1000 ) > max ) max = ( bins[i][j]*wPhoton ) / 1000;
	}


	double expectedMaximum = 946.489; /* kW/m2 */
	double relativeError = ( max - expectedMaximum ) / expectedMaximum;
	EXPECT_PRED_FORMAT2(::testing::DoubleLE, relativeError, 0.02);
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
