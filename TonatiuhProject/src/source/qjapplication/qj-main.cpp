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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <any>
#include <locale>
#include <type_traits>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//#include <QLibrary>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVariant>

#include <TNodesDatabase.h>
#include "TMaterialFactory.h"

#include "PluginManager.h"
#include "RayCasting.h"
#include "TJSONNodesDocument.h"
#include "TPhotonMap.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TTrackerFactory.h"


#include "Ejemplo1_Convergencia.h"
#include "Ejemplo1_EfficiencyMatrix.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

template<class T1, class T2>
void print_is_same() {
  std::cout << std::is_same<T1, T2>() << '\n';
}



int main ( int argc, char** argv )
{

	std::cout<<"Tonatiuh 3.0 JSON"<<std::endl;
	qInstallMessageHandler(myMessageOutput);

    QApplication a{ argc, argv };
	a.setApplicationVersion( APP_VERSION );

	std::cout<<"Tonatiuh 3.0 TNodesDatabase"<<std::endl;
	TNodesDatabase::Init();


	QDir pluginsDirectory( qApp->applicationDirPath() );
	pluginsDirectory.cd( "plugins" );


	std::string err;
	PluginManager pluginManager;
	pluginManager.LoadAvailablePlugins( pluginsDirectory.absolutePath().toStdString(), &err );
	if( err == "" )
	{
		std::cerr<<err<<std::endl;
	}


	std::vector< std::string>  photonMapExportTypeNameList = pluginManager.GetPhotonMapExportTypeFactoryNames();
	int numberOfPhotonMapExportTypeFactories = int( photonMapExportTypeNameList.size() );
	std::cout<<"- Number of Photon Map Export Types: "<<numberOfPhotonMapExportTypeFactories<<std::endl;
	for( int p = 0; p < numberOfPhotonMapExportTypeFactories; p++ )
		std::cout<<"  - Photon Map Export Type "<< p <<" : "<<photonMapExportTypeNameList[p]<<std::endl;

	std::vector< std::string>  randomFactoryNameList = pluginManager.GetRandomDeviateFactoryNames();
	int numberOfRandomFactories = int( randomFactoryNameList.size() );
	std::cout<<"- Number of Random Deviates: "<<numberOfRandomFactories<<std::endl;
	for( int r = 0; r < numberOfRandomFactories; r++ )
		std::cout<<"  - Random Deviates "<< r <<" : "<<randomFactoryNameList[r]<<std::endl;

	std::vector< std::string > materialFactoryNameList = pluginManager.GetTMaterialFactoryNames();
	int numberOfMaterialFactories= int( materialFactoryNameList.size() );
	std::cout<<"- Number of materials: "<<numberOfMaterialFactories<<std::endl;
	for( int m = 0; m < numberOfMaterialFactories; m++ )
		std::cout<<"  - Material "<< m <<" : "<<materialFactoryNameList[m]<<std::endl;

	std::vector< std::string >  shapeFactoryNameList = pluginManager.GetTShapeFactoryNames();
	int numberOfShapeFactories = int( shapeFactoryNameList.size() );
	std::cout<<"- Number of shapes: "<<numberOfShapeFactories<<std::endl;
	for( int s = 0; s < numberOfShapeFactories; s++ )
		std::cout<<"  - Shape "<< s <<" : "<<shapeFactoryNameList[s]<<std::endl;


	std::vector< std::string >  sunshapeFactoryNameList = pluginManager.GetTSunshapeFactoryNames();
	int numberOfSunshapeFactories = int( sunshapeFactoryNameList.size() );
	std::cout<<"- Number of sunshapes: "<<numberOfSunshapeFactories<<std::endl;
	for( int s = 0; s < numberOfSunshapeFactories; s++ )
		std::cout<<"  - Sunshape "<< s <<" : "<<sunshapeFactoryNameList[s]<<std::endl;


	std::vector<std::string > trackerFactoryNameList = pluginManager.GetTTrackerFactoryNames();
	int numberOfTrackerFactories= int( trackerFactoryNameList.size() );
	std::cout<<"- Number of trackers: "<<numberOfSunshapeFactories<<std::endl;
	for( int t = 0; t < numberOfTrackerFactories; t++ )
		std::cout<<"  - tracker "<< t <<" : "<<trackerFactoryNameList[t]<<std::endl;


	std::vector<std::string > transmissivityFactoryNameList = pluginManager.GetTTransmissivityFactoryNames();
	int numberOfTransmissivityFactories= int( transmissivityFactoryNameList.size() );
	std::cout<<"- Number of transmissivity models: "<<numberOfTransmissivityFactories<<std::endl;
	for( int t = 0; t < numberOfTransmissivityFactories; t++ )
		std::cout<<"  -  transmissivity "<< t <<" : "<<transmissivityFactoryNameList[t]<<std::endl;



/************************************************************

	QString fileName = argv[1] ;
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly))
	    // handle error
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();
	myEngine.evaluate(contents, fileName);

************************************************************/

/************************************/

	//Run validation examples
	std::string currentDirectory = argv[1];


	int ejemplo = atoi( argv[2] );
	int simulacion  = atoi( argv[3] );

	switch( ejemplo )
	{
		case 1:
		{
			if( simulacion == 0 ) //Create model
			{
				Ejemplo1_CreateModel( currentDirectory, &pluginManager );

				break;
			}

			if( simulacion == 1 ) //Convergencia
			{
				double azimuth = atof( argv[4] );
				double elevation = atof( argv[5] );
				std::cout<<azimuth<<std::endl;
				std::cout<<elevation<<std::endl;
				Ejemplo1_Convergencia( currentDirectory, azimuth, elevation, &pluginManager );
				break;
			}

			if( simulacion == 2 ) //Efficiency Matrix
			{
				Ejemplo1_EfficiencyMatrix( currentDirectory, &pluginManager );
				break;
			}
			break;
		}

	}

/************************************/

/***********************************
	std::cout<<"Tonatiuh 3.0 readDocument"<<std::endl;

	TJSONNodesDocument readDocument;
	bool okRead = readDocument.Read( argv[2] );
	if( !okRead  )
	{
		std::cerr<<"\t Error reading file 0!!!"<<std::endl;
		return ( -1 );
	}


	std::cout<<"Tonatiuh 3.0 sceneNode"<<std::endl;
	std::shared_ptr< TSceneNode > sceneNode = std::dynamic_pointer_cast< TSceneNode > ( readDocument.GetRootNode() );
	if( sceneNode == nullptr )
	{
		std::cout<<"\t Error reading file 1!!!"<<std::endl;
		return ( -2 );
	}

	if( !sceneNode || sceneNode == 0 )
	{
		std::cout<<"\t Error reading file 2!!!"<<std::endl;
		return ( -2 );
	}

	std::cout<<"\t Updating trackers..."<<std::endl;
	sceneNode->UpdateTrackers( );
	std::cout<<"\t Trackers updated"<<std::endl;



***********************************/

/************************************************************


	std::cout<<"Tonatiuh 3.0 saveDocument"<<std::endl;
	TJSONNodesDocument saveDocument2;
	saveDocument2.SetRootNode( sceneNode );
	if( !saveDocument2.Write( argv[2] )  )
	{
		std::cout<<"Error during writing file"<<std::endl;
		return -1;
	}

***********************************/
/***********************************
	//std::cout<<"sceneNode "<<sceneNode->GetName().toStdString()<<std::endl;
	//ChildrenBranch(sceneNode);



	//Codigo para verificar el orden de para aplicar las transformaciones
	Transform translation = Translate( 0,200,0 );
	Transform rotation = Rotate( 15 * gc::Degree, Vector3D( 1,0,0) );
	Transform scale = Scale( 0.5, 1, 1);

	Transform transformOTW = translation * rotation * scale;
	std::cout<<"p: "<<transformOTW(Point3D( 0, 0, 0 ) )<<std::endl;
	*



***********************************/

/***********************************


	std::cout<<"\tCurrent values:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


	sceneNode->ChangeSunPosition( 180*gc::Degree, ( 90-49.2565)*gc::Degree );
	std::cout<<"\tNew values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;

	std::cout<<"Creating the raytracer"<<std::endl;
	QDateTime startTime = QDateTime::currentDateTime();
	RayCasting raytracer;
	if( !raytracer.SetScene( sceneNode ) )
	{

		std::cout<<"ERROR in the scene"<<std::endl;
		return ( -5 );
	}


	//Create the random generator
	std::cout<<"Create the random generator"<<std::endl;
	std::shared_ptr< RandomDeviate > pRand = nullptr;
	if( randomFactoryNameList.size() < 1 )
		return ( -6 );
	pRand =  pluginManager.CreateRandomDeviate( randomFactoryNameList[0] );
	if( pRand == nullptr )
		return ( -7 );

	raytracer.SetRandomNumberGenerator( pRand.get() );

	std::cout<<"\tCreating the photon map"<<std::endl;
	std::unique_ptr< TPhotonMap > pPhotonMap = std::make_unique < TPhotonMap >();
	pPhotonMap->SetBufferSize( 50000000 );

	//Defining how export the stored data.
	std::cout<<"\t Defining how export the stored data."<<std::endl;
	std::unique_ptr< PhotonMapExportType > pPhotonMapExportType = pluginManager.CreatePhotonMapExportType( photonMapExportTypeNameList[0] );
	pPhotonMapExportType->SetSaveCoordinatesEnabled( true );
	pPhotonMapExportType->SetSaveCoordinatesInGlobalSystemEnabled( true );
	pPhotonMapExportType->SetSavePreviousNextPhotonsID( false );
	pPhotonMapExportType->SetSaveSideEnabled( true );
	pPhotonMapExportType->SetSaveSurfacesIDEnabled( true );


	if( argv[3] == NULL || argv[4] == NULL )
	{
		std::cout<<"No file to save results "<<std::endl;
		return ( -8 );
	}


	std::string filesRootName = argv[4];
	pPhotonMapExportType->SetSaveParameterValue( "ExportDirectory", argv[3] );
	std::cout<<"\t\tExportDirectory: "<< argv[3]<<std::endl;
	pPhotonMapExportType->SetSaveParameterValue( "ExportFile", filesRootName );
	std::cout<<"\t\tExportFile: "<<filesRootName<<std::endl;
	pPhotonMapExportType->SetSaveParameterValue( "FileSize",  "1000000" );

	std::vector< std::string > saveSurfaceList{ "//SunNode/RootNode/Tower/Escudo/Receptor/SuperficieReceptor" };
	pPhotonMapExportType->SetSaveSurfacesURLList( saveSurfaceList );
	pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );

	raytracer.SetPhotonMap( pPhotonMap.get() );

	double tracedRays = 5000000;
	std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
	raytracer.Run( tracedRays );

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;



	std::cout<<"END main"<<std::endl;




************************************************************/


	std::cout<<"END" <<std::endl;
	return ( 0 );
}



