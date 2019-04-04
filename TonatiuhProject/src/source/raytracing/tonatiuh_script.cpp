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

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QScriptContext>
#include <QScriptEngine>

#include "ScriptRayTracer.h"
#include "RandomDeviateFactory.h"
#include "tonatiuh_script.h"
#include "sunpos.h"


int tonatiuh_script::init( QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	rayTracer->Clear();

	QScriptValue fun_tonatiuh_filename = engine->newFunction( tonatiuh_script::tonatiuh_filename );
	engine->globalObject().setProperty("tonatiuh_filename", fun_tonatiuh_filename );

	QScriptValue fun_tonatiuh_irradiance = engine->newFunction( tonatiuh_script::tonatiuh_irradiance );
	engine->globalObject().setProperty("tonatiuh_irradiance", fun_tonatiuh_irradiance );

	QScriptValue fun_tonatiuh_numrays = engine->newFunction( tonatiuh_script::tonatiuh_numrays );
	engine->globalObject().setProperty("tonatiuh_numrays", fun_tonatiuh_numrays );

	QScriptValue fun_tonatiuh_numdivisions = engine->newFunction( tonatiuh_script::tonatiuh_numdivisions );
	engine->globalObject().setProperty("tonatiuh_numdivisions", fun_tonatiuh_numdivisions );

	QScriptValue fun_tonatiuh_photon_map = engine->newFunction( tonatiuh_script::tonatiuh_photon_map_export_mode );
	engine->globalObject().setProperty("tonatiuh_photon_map", fun_tonatiuh_photon_map );

	QScriptValue fun_tonatiuh_random_generator = engine->newFunction( tonatiuh_script::tonatiuh_random_generator );
	engine->globalObject().setProperty("tonatiuh_random_generator", fun_tonatiuh_random_generator );

	QScriptValue fun_tonatiuh_sunposition = engine->newFunction( tonatiuh_script::tonatiuh_sunposition );
	engine->globalObject().setProperty("tonatiuh_sunposition", fun_tonatiuh_sunposition );

	QScriptValue fun_tonatiuh_calculatesun = engine->newFunction( tonatiuh_script::tonatiuh_calculatesun );
	engine->globalObject().setProperty("tonatiuh_calculatesun", fun_tonatiuh_calculatesun );

	QScriptValue fun_tonatiuh_setsunpositiontoscene = engine->newFunction( tonatiuh_script::tonatiuh_setsunpositiontoscene );
	engine->globalObject().setProperty("tonatiuh_setsunpositiontoscene", fun_tonatiuh_setsunpositiontoscene );

	QScriptValue fun_tonatiuh_setdisconnectalltrackers = engine->newFunction( tonatiuh_script::tonatiuh_setdisconnectalltrackers );
	engine->globalObject().setProperty("tonatiuh_setdisconnectalltrackers", fun_tonatiuh_setdisconnectalltrackers );

	QScriptValue fun_tonatiuh_saveas = engine->newFunction( tonatiuh_script::tonatiuh_saveas );
	engine->globalObject().setProperty("tonatiuh_saveas", fun_tonatiuh_saveas );

	QScriptValue fun_tonatiuh_trace = engine->newFunction( tonatiuh_script::tonatiuh_trace );
	engine->globalObject().setProperty("tonatiuh_trace", fun_tonatiuh_trace );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_filename(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() != 1 )	return context->throwError( "tonatiuh_filename: takes exactly one argument." );
	if( !context->argument( 0 ).isString() )	return context->throwError( "tonatiuh_filename: argument is not a string.");



	QString fileName = context->argument(0).toString();
	if( fileName.isEmpty()  )	return	context->throwError( "tonatiuh_filename: the model file is not correct." );

	QFileInfo file(  fileName );
	if( !file.isAbsolute() )
	{
		QString dirName = rayTracer->GetDir();
		QDir currentDir( dirName );
		QFileInfo absolutefile( currentDir, fileName );
		fileName = absolutefile.absoluteFilePath();

	}

	QFileInfo modelFile( fileName ) ;
	if( !modelFile.exists() )
	{
		QString message = QString( "tonatiuh_filename: The %1 file can not be opened." ).arg( fileName );
		return context->throwError( QScriptContext::UnknownError, message );
	}

	int result = rayTracer->SetTonatiuhModelFile( fileName );
	if( result == 0 )
	{
		QString message = QString( "tonatiuh_filename: The %1 file is not a valid file." ).arg( fileName );
		return context->throwError( message );
	}

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_irradiance(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();


	if( context->argumentCount() != 1 )	return context->throwError( "tonatiuh_irradiance: takes exactly one argument." );
	if( !context->argument( 0 ).isNumber() )	return context->throwError( "tonatiuh_irradiance: argument is not a number." );

	double irradiance = context->argument(0).toNumber();

	int result = rayTracer->SetIrradiance( irradiance );
	if( result == 0 )	return context->throwError( QScriptContext::UnknownError, "tonatiuh_irradiance: UnknownError." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_numrays(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() != 1 )	return context->throwError( "tonatiuh_numrays: takes exactly one argument." );
	if( !context->argument( 0 ).isNumber() )	return context->throwError( "tonatiuh_numrays: argument is not a number." );

	double nrays = context->argument(0).toNumber();
	if( nrays < 1 )	return context->throwError( "tonatiuh_numrays: the number of rays must be at least 1." );

	int result = rayTracer->SetNumberOfRays( nrays );
	if( result == 0 )	return context->throwError( "tonatiuh_numrays: UnknownError." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_numdivisions(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() < 1 )	return context->throwError( "tonatiuh_numdivisions: takes at least one argument." );
	if( context->argumentCount() > 2 )	return context->throwError( "tonatiuh_numdivisions: takes no more than two arguments." );
	if(context->argumentCount()==2){
		if( !context->argument( 1 ).isNumber() )	return context->throwError( "tonatiuh_numdivisions: second argument is not a number." );

			double nheightdivisions = context->argument(1).toNumber();
			if( nheightdivisions < 1 )	return context->throwError( "tonatiuh_numdivisions: the number of heigth divisions must be at least 1." );

			int result = rayTracer->SetNumberOfHeightDivisions( nheightdivisions );
			if( result == 0 )	return context->throwError( "tonatiuh_numdivisions: UnknownError." );
	}
	if( !context->argument( 0 ).isNumber() )	return context->throwError( "tonatiuh_numdivisions: argument is not a number." );

	double nwidthdivisions = context->argument(0).toNumber();
	if( nwidthdivisions < 1 )	return context->throwError( "tonatiuh_numdivisions: the number of width divisions must be at least 1." );

	int result = rayTracer->SetNumberOfWidthDivisions( nwidthdivisions );
	if( result == 0 )	return context->throwError( "tonatiuh_numdivisions: UnknownError." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_photon_map_export_mode(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() != 1 )	return context->throwError( "tonatiuh_photon_map: takes exactly one argument." );
	if( !context->argument( 0 ).isString() )	return context->throwError( "tonatiuh_photon_map: argument is not a string." );

	QString photonMapExportType = context->argument(0).toString();
	int result = 	rayTracer->SetPhotonMapExportMode( photonMapExportType );
	if( result == 0 )	return context->throwError( "tonatiuh_photon_map: UnknownError." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_random_generator(QScriptContext* context, QScriptEngine* engine )
{

	if( context->argumentCount() != 1 )	return context->throwError( "tonatiuh_random_generator: takes exactly one argument." );
	if( !context->argument( 0 ).isString() )	return context->throwError( "tonatiuh_random_generator: argument is not a string." );

	QScriptValue rayTracerValue = engine->globalObject().property( "rayTracer" );
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	QString randomDeviateType = context->argument(0).toString();
	if( !rayTracer->IsValidRandomGeneratorType( randomDeviateType ) )	return context->throwError( "tonatiuh_random_generator: defined random generator type is not valid." );
	rayTracer->SetRandomDeviateType( randomDeviateType );

	int result = 	rayTracer->SetRandomDeviateType( randomDeviateType );
	if( result == 0 )	return context->throwError( "tonatiuh_random_generator: UnknownError." );

	return 1;
}

QScriptValue  tonatiuh_script::tonatiuh_sunposition(QScriptContext* context, QScriptEngine* engine )
{
	//tonatiuh_sunposition( azimuth, elevation, distance );
	if( ( context->argumentCount() < 2 ) || ( context->argumentCount() > 3 ) )
		return context->throwError( "tonatiuh_sunposition: takes 2 argument." );
	if( !context->argument( 0 ).isNumber() )	return context->throwError( "tonatiuh_sunposition: argument 1 is not a number." );
	if( !context->argument( 1 ).isNumber() )	return context->throwError( "tonatiuh_sunposition: argument 2 is not a number." );

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	double azimuth = context->argument(0).toNumber();
	if( ( azimuth < 0. ) || ( azimuth > 360. ) )	return context->throwError( "tonatiuh_sunposition: azimuth value must be between 0 and 360 degrees." );
	rayTracer->SetSunAzimtuh( azimuth );

	double elevation = context->argument(1).toNumber();
	if( ( elevation < 0. ) || ( elevation > 90. ) )	return context->throwError( "tonatiuh_sunposition: elevation value must be between 0 and 90 degrees." );
	rayTracer->SetSunElevation( elevation );

	return 1;

}

QScriptValue  tonatiuh_script::tonatiuh_calculatesun(QScriptContext* context, QScriptEngine* engine )
{
	//tonatiuh_sunposition( azimuth, elevation, distance );
	if( ( context->argumentCount()!=6))
		return context->throwError( "tonatiuh_calculatesun: must take 5 arguments." );
	if( !context->argument( 0 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 1 is not a number." );
	if( !context->argument( 1 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 2 is not a number." );
	if( !context->argument( 2 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 3 is not a number." );
	if( !context->argument( 3 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 4 is not a number." );
	if( !context->argument( 4 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 5 is not a number." );
	if( !context->argument( 5 ).isNumber() )	return context->throwError( "tonatiuh_calculatesun: argument 6 is not a number." );

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	int year = context->argument(0).toNumber();
	int month = context->argument(1).toNumber();
	int day = context->argument(2).toNumber();
	double hours = context->argument(3).toNumber();
	double latitude = context->argument(4).toNumber();
	double longitude = context->argument(5).toNumber();
	if( ( month < 0 ) || ( month> 12 ) ) return context->throwError( "tonatiuh_calculatesun: month must be between 1 and 12." );
	if( ( day < 0 ) || ( day > 31  ) ) return context->throwError( "tonatiuh_calculatesun: the day must be between 1 and 31." );
	if( ( hours < 0 ) || ( hours > 23  ) ) return context->throwError( "tonatiuh_calculatesun: the hour must be between 1 and 31." );
	if( ( longitude < -180. ) || ( longitude > 180.  ) ) return context->throwError( "tonatiuh_calculatesun: the longitude must be between -180 and 180." );
	if( ( latitude < -90. ) || ( latitude > 90.  ) ) return context->throwError( "tonatiuh_calculatesun: the latitude must be between -90 and 90." );

	cTime myTime = { year, month, day, hours, 0, 0 };
	cLocation myLocation = {longitude , latitude };
	cSunCoordinates results;
	sunpos( myTime, myLocation, &results );

	//if( ( results.dAzimuth < 0. ) || ( azimuth > 360. ) )	return context->throwError( "tonatiuh_sunposition: azimuth value must be between 0 and 360 degrees." );
	rayTracer->SetSunAzimtuh( results.dAzimuth );

	//double elevation = context->argument(1).toNumber();
	//if( ( < 0. ) || ( elevation > 90. ) )	return context->throwError( "tonatiuh_sunposition: elevation value must be between 0 and 90 degrees." );
	rayTracer->SetSunElevation(results.dZenithAngle );

	/*if(90-results.dZenithAngle<0){
		return 2;
	}*/
	rayTracer->SetSunPositionToScene();
	if((90-results.dZenithAngle)<0)return false;
	else return true;

}

QScriptValue tonatiuh_script::tonatiuh_setsunpositiontoscene(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	if( context->argumentCount() )	context->throwError( "tonatiuh_setsunpositiontoscene() no takes arguments." );

	int result = rayTracer->SetSunPositionToScene();
	if( result == 0 ) 	context->throwError( "tonatiuh_setsunpositiontoscene() error." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_setdisconnectalltrackers(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	if( context->argumentCount()!=1 )	context->throwError( "tonatiuh_setdisconnectalltrackers() takes exactly one argument." );
	if( !context->argument( 0 ).isBool() )	context->throwError( "tonatiuh_setdisconnectalltrackers: argument is not a bool.");
	int result = rayTracer->SetDisconnectAllTrackers(context->argument( 0 ).toBool());
	if( result == 0 ) 	context->throwError( "tonatiuh_setdisconnectalltrackers() error." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_saveas(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	if( context->argumentCount()!=1 )	context->throwError( "tonatiuh_saveas() takes exactly one argument." );
	if( !context->argument( 0 ).isString() )	context->throwError( "tonatiuh_saveas: argument is not a string.");

	QString fileName = context->argument(0).toString();
	if( fileName.isEmpty()  )	context->throwError( "tonatiuh_saveas: the model file is not correct." );

	QFileInfo file(  fileName );
	if( !file.isAbsolute() )
	{
		QString dirName = rayTracer->GetDir();
		QDir currentDir( dirName );
		QFileInfo absolutefile( currentDir, fileName );
		fileName = absolutefile.absoluteFilePath();
	}


	int result = rayTracer->Save(fileName);
	if( result == 0 ) 	context->throwError( "tonatiuh_saveas() error." );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_trace(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	if( context->argumentCount() )	return context->throwError( "tonatiuh_trace() no takes arguments." );

	int result = rayTracer->Trace();
	if( result == 0 )	return context->throwError( "tonatiuh_trace() error." );

	return 1;
}
