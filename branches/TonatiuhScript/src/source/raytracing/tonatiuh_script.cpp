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

#include <QScriptContext>
#include <QScriptEngine>

#include "ScriptRayTracer.h"
#include "RandomDeviateFactory.h"
#include "tonatiuh_script.h"


int tonatiuh_script::init( QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	rayTracer->Clear();

	QScriptValue fun_tonatiuh_exportAll = engine->newFunction( tonatiuh_script::tonatiuh_exportAll );
	engine->globalObject().setProperty("tonatiuh_exportAll", fun_tonatiuh_exportAll );

	QScriptValue fun_tonatiuh_export = engine->newFunction( tonatiuh_script::tonatiuh_export );
	engine->globalObject().setProperty("tonatiuh_export", fun_tonatiuh_export );

	QScriptValue fun_tonatiuh_filename = engine->newFunction( tonatiuh_script::tonatiuh_filename );
	engine->globalObject().setProperty("tonatiuh_filename", fun_tonatiuh_filename );

	QScriptValue fun_tonatiuh_numrays = engine->newFunction( tonatiuh_script::tonatiuh_numrays );
	engine->globalObject().setProperty("tonatiuh_numrays", fun_tonatiuh_numrays );

	QScriptValue fun_tonatiuh_photon_map = engine->newFunction( tonatiuh_script::tonatiuh_photon_map );
	engine->globalObject().setProperty("tonatiuh_photon_map", fun_tonatiuh_photon_map );

	QScriptValue fun_tonatiuh_random_generator = engine->newFunction( tonatiuh_script::tonatiuh_random_generator );
	engine->globalObject().setProperty("tonatiuh_random_generator", fun_tonatiuh_random_generator );

	QScriptValue fun_tonatiuh_sunposition = engine->newFunction( tonatiuh_script::tonatiuh_sunposition );
	engine->globalObject().setProperty("tonatiuh_sunposition", fun_tonatiuh_sunposition );

	QScriptValue fun_tonatiuh_trace = engine->newFunction( tonatiuh_script::tonatiuh_trace );
	engine->globalObject().setProperty("tonatiuh_trace", fun_tonatiuh_trace );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_exportAll(QScriptContext* context, QScriptEngine* engine )
{
	if( context->argumentCount() != 1 ) return 0; //throw Error( "tonatiuh_filename() takes exactly one argument." ); )
	if( !context->argument( 0 ).isString() ) return 0; //throw Error( "tonatiuh_filename() argument is not a string." ); )

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	QString fileName = context->argument(0).toString();
	rayTracer->SetExportFileName( fileName );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_export(QScriptContext* context, QScriptEngine* engine )
{
	if( context->argumentCount() < 2 ) return 0; //throw Error( "tonatiuh_export() takes at least two argument." ); )
	if( context->argumentCount() > 3 ) return 0; //throw Error( "tonatiuh_export() takes at most three argument." ); )
	if( !context->argument( 0 ).isString() ) return 0; //throw Error( "tonatiuh_export() argument 1 is not a string." ); )
	if( !context->argument( 1 ).isString() ) return 0; //throw Error( "tonatiuh_export() argument 2 is not a string." ); )
	if( (context->argumentCount() == 3) && !context->argument( 2 ).isBool() ) return 0; //throw Error( "tonatiuh_export() argument 3 is not a boolean." ); )

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	QString fileName = context->argument(0).toString();
	rayTracer->SetExportFileName( fileName );

	QString surfaceName = context->argument(1).toString();
	rayTracer->SetExportSurfaceName( surfaceName );

	if( context->argumentCount() == 3)
	{
		bool coordinates= context->argument(2).toBool();
		rayTracer->SetExportSurfaceCoordinates( coordinates );
	}

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_filename(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() != 1 ) return 0; //throw Error( "tonatiuh_filename() takes exactly one argument." ); )
	if( !context->argument( 0 ).isString() ) return 0; //throw Error( "tonatiuh_filename() argument is not a string." ); )

	QString fileName = context->argument(0).toString();
	rayTracer->SetTonatiuhModelFile( fileName );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_numrays(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();


	if( context->argumentCount() != 1 ) return 0; //throw Error( "tonatiuh_filename() takes exactly one argument." ); )
	if( !context->argument( 0 ).isNumber() ) return 0; //throw Error( "tonatiuh_filename() argument is not a number." ); );

	double nrays = context->argument(0).toNumber();
	rayTracer->SetNumberOfRays( nrays );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_photon_map(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();

	if( context->argumentCount() != 1 ) return 0; //throw Error( "tonatiuh_filename() takes exactly one argument." ); )
	if( !context->argument( 0 ).isString() ) return 0; //throw Error( "tonatiuh_filename() argument is not a string." ); )

	QString photonMapType = context->argument(0).toString();
	rayTracer->SetPhotonMapType( photonMapType );

	return 1;
}

QScriptValue tonatiuh_script::tonatiuh_random_generator(QScriptContext* context, QScriptEngine* engine )
{
	if( context->argumentCount() != 1 ) return 0; //throw Error( "tonatiuh_filename() takes exactly one argument." ); )
	if( !context->argument( 0 ).isString() ) return 0; //throw Error( "tonatiuh_filename() argument is not a string." ); )

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	QString randomDeviateType = context->argument(0).toString();
	rayTracer->SetRandomDeviateType( randomDeviateType );

	return 1;
}

QScriptValue  tonatiuh_script::tonatiuh_sunposition(QScriptContext* context, QScriptEngine* engine )
{
	if( context->argumentCount() != 3 ) return 0; //throw Error( "tonatiuh_sunposition() takes exactly one argument." ); )
	if( !context->argument( 0 ).isNumber() ) return 0; //throw Error( "tonatiuh_sunposition() argument 1 is not a number." ); );
	if( !context->argument( 1 ).isNumber() ) return 0; //throw Error( "tonatiuh_sunposition() argument 2 is not a number." ); );
	if( !context->argument( 2 ).isNumber() ) return 0; //throw Error( "tonatiuh_sunposition() argument 3 is not a number." ); );

	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	double azimuth = context->argument(0).toNumber();
	rayTracer->SetSunAzimtuh( azimuth );

	double elevation = context->argument(1).toNumber();
	rayTracer->SetSunElevation( elevation );

	double distance = context->argument(2).toNumber();
	rayTracer->SetSunDistance( distance );

	return 1;

}

QScriptValue tonatiuh_script::tonatiuh_trace(QScriptContext* context, QScriptEngine* engine )
{
	QScriptValue rayTracerValue = engine->globalObject().property("rayTracer");
	ScriptRayTracer* rayTracer = ( ScriptRayTracer* ) rayTracerValue.toQObject();
	if( !rayTracer ) return 0;

	if( context->argumentCount() != 0 ) return 0; //throw Error( "tonatiuh_trace() no takes argumets." ); )


	int result = rayTracer->Trace();
	if( result == 0 ) return 0; //throw Error( "tonatiuh_trace() error." ); )

	return 1;
}
