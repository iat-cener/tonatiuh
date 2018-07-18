/*
 * Ejemplo1_Convergencia.h
 *
 *  Created on: 18 jul. 2018
 *      Author: amutuberria
 */

#ifndef EJEMPLO1_CONVERGENCIA_H_
#define EJEMPLO1_CONVERGENCIA_H_

#include <sstream>

#include <QDir>

#include "PhotonMapExportType.h"
#include "PluginManager.h"
#include "TNodesDocument.h"

/*!
 * Esta función ejecuta la simulación de convergencia para el ejemplo 1.
 * Azimuth and elevation values are defined in degrees.
 */
void Ejemplo1_Convergencia( std::string currentDirectory, double azimuth, double elevation, PluginManager* pPluginManager )
{
	std::cout<< "\n\n**************************************************"<<std::endl;

	std::cout<< "Ejemplo1_Convergencia"<<std::endl;

	QDir workingPath{ currentDirectory.c_str() };
	workingPath.cd( "Ejemplo1" );
	std::cout<< "\t workingPath: "<<workingPath.absolutePath().toStdString()<<std::endl;

	std::string filename = workingPath.absoluteFilePath( "Ejemplo1_8534heliostatos_aimingStrategy_v3XML.tnhs" ).toStdString();

	TNodesDocument readDocument;
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
	std::cout<<"Current sun position:"<<std::endl;
	std::cout<<"\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t zenith: "<<sun->GetZenith()<<std::endl;


	std::cout<<"Changing sun position..."<<std::endl;
	double zenith = 90-elevation;
	sceneNode->ChangeSunPosition( azimuth*gc::Degree, zenith * gc::Degree );


	std::cout<<"\tNew values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


	std::cout<<"Creating the raytracer"<<std::endl;

	QDateTime startTime = QDateTime::currentDateTime();
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
	azimuth_convert << azimuth;

	std::ostringstream elevation_convert;
	elevation_convert << elevation;

	std::string positionDirName = "Azimuth" + azimuth_convert.str()+ "Elevation"+ elevation_convert.str();
	QDir exportDir = workingPath;

	if( !exportDir.cd( "Simulaciones" ) )
		exportDir.mkdir( "Simulaciones" );

	if( !exportDir.cd( "Convergencia" ) )
		exportDir.mkdir( "Convergencia" );

	if( !exportDir.cd( positionDirName.c_str() ) )
			exportDir.mkdir( positionDirName.c_str() );

	if( !exportDir.exists() )
	{
		std::cerr<<"ERROR defining export folder"<<std::endl;
	}

	double tracedRays = 500000;
	std::vector< std::string > saveSurfaceList{ "//SunNode/solarField_node/tower_node/receiver_node/receiver_rotation/ReceiverSurface" };
	for( int s = 0; s < 20; s++ )
	{
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


		std::ostringstream s_convert;
		s_convert << s;
		std::string fileNames =  "TargetData_s" + s_convert.str();
		std::cout<<"\t\tExportFile: "<< fileNames<<std::endl;
		pPhotonMapExportType->SetSaveParameterValue( "ExportFile", fileNames );
		pPhotonMapExportType->SetSaveParameterValue( "FileSize",  "1000000" );

		pPhotonMapExportType->SetSaveSurfacesURLList( saveSurfaceList );
		pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );
		raytracer.SetPhotonMap( pPhotonMap.get() );


		std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
		raytracer.Run( tracedRays );

	}

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;



}


#endif /* EJEMPLO1_CONVERGENCIA_H_ */
