/*
 * Ejemplo1_EfficiencyMatrix.h
 *
 *  Created on: 18 jul. 2018
 *      Author: amutuberria
 */

#ifndef EJEMPLO1_EFFICINENCYMATRIX_H_
#define EJEMPLO1_EFFICINENCYMATRIX_H_

#include <sstream>

#include <QDir>

#include "PhotonMapExportType.h"
#include "PluginManager.h"
#include "TJSONNodesDocument.h"

/*!
 * This function simulates the example 1 for a matrix of sun positions.
 */
void Ejemplo1_EfficiencyMatrix( std::string currentDirectory, PluginManager* pPluginManager )
{
	std::cout<< "\n\n**************************************************"<<std::endl;

	std::cout<< "Ejemplo1_EfficiencyMatrix"<<std::endl;

	QDir workingPath{ currentDirectory.c_str() };
	workingPath.cd( "Ejemplo1" );
	std::cout<< "\t workingPath: "<<workingPath.absolutePath().toStdString()<<std::endl;

	std::string filename = workingPath.absoluteFilePath( "Ejemplo1_8534heliostatos_aimingStrategy_v3JS.tnh" ).toStdString();

	TJSONNodesDocument readDocument;
	bool okRead = readDocument.Read( filename );
	if( !okRead  )
	{
		std::cerr<<"¡¡¡Error reading file: "<<filename<<"!!!"<<std::endl;
		return;
	}

	std::cout<<"Tonatiuh 3.0 sceneNode"<<std::endl;
	std::shared_ptr< TSceneNode > sceneNode = std::dynamic_pointer_cast< TSceneNode > ( readDocument.GetRootNode() );
	if( sceneNode == nullptr )
	{
		std::cout<<"¡¡¡Error reading file scene node!!!"<<std::endl;
		return;
	}


	std::cout<<"\tUpdating trackers..."<<std::endl;
	sceneNode->UpdateTrackers( );

	//Change sun position
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();

	int nAzimuths = 13;
	double azimuthValues[] = {50, 70, 90, 105, 120, 150, 180, 210, 240, 255, 270, 290, 310 };

	int nElevations = 7;
	double elevationValues[] = {1.0, 5.0, 15.0, 25.0, 45.0, 65.0,  89.5 };

	QDateTime startTime = QDateTime::currentDateTime();


	std::string fileNames =  "TargetData";
	double tracedRays = 10000000;
	std::vector< std::string > saveSurfaceList{ "//SunNode/solarField_node/tower_node/receiver_node/receiver_rotation/ReceiverSurface" };

	for( int a = 0; a < nAzimuths; a++ )
	{
		for( int e = 0; e < nElevations; e++ )
		{
			std::cout<<"Changing sun position..."<<std::endl;
			double zenith = 90 - elevationValues[e];
			sceneNode->ChangeSunPosition( azimuthValues[a] * gc::Degree, zenith * gc::Degree );

			std::cout<<"\tNew values:"<<std::endl;
			std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
			std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


			std::cout<<"Creating the raytracer"<<std::endl;

			RayCasting raytracer;
			if( !raytracer.SetScene( sceneNode ) )
			{

				std::cerr<<"ERROR in the scene"<<std::endl;
				return;
			}


			//Create the random generator
			std::cout<<"Create the random generator"<<std::endl;
			std::shared_ptr< RandomDeviate > pRand =  pPluginManager->CreateRandomDeviate( "Mersenne Twister" );
			if( pRand == nullptr )
				return;

			raytracer.SetRandomNumberGenerator( pRand.get() );

			std::ostringstream azimuth_convert;
			azimuth_convert << azimuthValues[a];

			std::ostringstream elevation_convert;
			elevation_convert << elevationValues[e];

			std::string positionDirName = "Azimuth" + azimuth_convert.str()+ "Elevation"+ elevation_convert.str();
			QDir exportDir = workingPath;

			if( !exportDir.cd( "Simulaciones" ) )
			{
				exportDir.mkdir( "Simulaciones" );
				exportDir.cd( "Simulaciones"  );
			}

			if( !exportDir.cd( "EfficiencyMatrix" ) )
			{
				exportDir.mkdir( "EfficiencyMatrix" );
				exportDir.cd( "EfficiencyMatrix"  );
			}

			if( !exportDir.cd( "Simulaciones" ) )
			{
				exportDir.mkdir( "Simulaciones" );
				exportDir.cd( "Simulaciones"  );
			}

			if( !exportDir.cd( positionDirName.c_str() ) )
			{
					exportDir.mkdir( positionDirName.c_str() );
					exportDir.cd( positionDirName.c_str() );
			}

			if( !exportDir.exists() )
			{
				std::cerr<<"ERROR defining export folder"<<std::endl;
			}

			//Creating the photon map
			std::cout<<"\tCreating the photon map"<<std::endl;
			std::unique_ptr< TPhotonMap > pPhotonMap = std::make_unique < TPhotonMap >();

			//Defining how export the stored data.
			std::cout<<"\t Defining how export the stored data."<<std::endl;
			std::unique_ptr< PhotonMapExportType > pPhotonMapExportType = pPluginManager->CreatePhotonMapExportType( "Binary_file" );
			if( pPhotonMapExportType == nullptr )
			{
				std::cerr<<"\t Error defining the export type."<<std::endl;
				return;
			}

			pPhotonMapExportType->SetSaveCoordinatesEnabled( true );
			pPhotonMapExportType->SetSaveCoordinatesInGlobalSystemEnabled( false );
			pPhotonMapExportType->SetSavePreviousNextPhotonsID( false );
			pPhotonMapExportType->SetSaveSideEnabled( true );
			pPhotonMapExportType->SetSaveSurfacesIDEnabled( true );


			std::cout<<"\t\tExportDirectory: "<<exportDir.absolutePath().toStdString()<<std::endl;
			pPhotonMapExportType->SetSaveParameterValue( "ExportDirectory", exportDir.absolutePath().toStdString() );

			std::cout<<"\t\tExportFile: "<< fileNames<<std::endl;
			pPhotonMapExportType->SetSaveParameterValue( "ExportFile", fileNames );
			pPhotonMapExportType->SetSaveParameterValue( "FileSize",  "1000000" );

			pPhotonMapExportType->SetSaveSurfacesURLList( saveSurfaceList );
			pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );
			raytracer.SetPhotonMap( pPhotonMap.get() );


			std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
			raytracer.Run( tracedRays );


		}
	}

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;



}


#endif /* EJEMPLO1_EFFICINENCYMATRIX_H_ */
