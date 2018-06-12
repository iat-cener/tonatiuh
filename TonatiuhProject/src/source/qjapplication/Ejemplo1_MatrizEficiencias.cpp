/*
 * Ejemplo1_MatrizEficiencias.cpp
 *
 *  Created on: 12 jun. 2018
 *      Author: amutuberria
 */


#include <iostream>
#include <sstream>
#include <vector>

#include "gc.h"
#include "PluginManager.h"
#include "RayCasting.h"
#include "TPhotonMap.h"
#include "TSceneNode.h"
#include "TSunNode.h"

int Ejemplo1_MatrizEficiencias(std::shared_ptr< TSceneNode >& sceneNode, const PluginManager& pluginManager, std::string directoryRootName,
		std::string randomFactoryName, std::string photonMapExportTypeName  )
{
	//Change sun position
	//std::cout<<"Change sun position:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();

	std::vector< double > azimuthValues{50,70,90,105,120,150,180, 210, 240, 255, 270, 290, 310 };
	std::vector< double >  elevationValues{ 1.0, 5.0, 15.0, 25.0, 45.0, 65.0,  89.5 };

	for( unsigned int a = 0; a < azimuthValues.size(); a++ )
	{
		for( unsigned int e = 0; e < elevationValues.size(); e++ )
		{
			std::cout<<"Changing sun position..."<<std::endl;
			sceneNode->ChangeSunPosition( azimuthValues[a]*gc::Degree, ( 90 - elevationValues[e] )*gc::Degree );

			std::cout<<"\t New values:"<<std::endl;
			std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
			std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


			std::cout<<"Creating the raytracer"<<std::endl;

			RayCasting raytracer;
			if( !raytracer.SetScene( sceneNode ) )
			{
				std::cout<<"ERROR in the scene"<<std::endl;
				return ( -5 );
			}

			//Create the random generator
			std::cout<<"Create the random generator"<<std::endl;
			std::shared_ptr< RandomDeviate > pRand = nullptr;
			if( randomFactoryName.empty() )
				return ( -6 );
			pRand =  pluginManager.CreateRandomDeviate( randomFactoryName );
			if( pRand == nullptr )
				return ( -7 );

			raytracer.SetRandomNumberGenerator( pRand.get() );

			//Creating the photon map
			std::cout<<"Creating the photon map"<<std::endl;
			std::unique_ptr< TPhotonMap > pPhotonMap = std::make_unique < TPhotonMap >();
			pPhotonMap->SetBufferSize( 1000000 );

			//Defining how export the stored data.
			std::cout<<"\t Defining how export the stored data."<<std::endl;

			if( photonMapExportTypeName.empty() )
				return ( -8 );
			std::unique_ptr< PhotonMapExportType > pPhotonMapExportType = pluginManager.CreatePhotonMapExportType( photonMapExportTypeName );
			pPhotonMapExportType->SetSaveCoordinatesEnabled( true );
			pPhotonMapExportType->SetSaveCoordinatesInGlobalSystemEnabled( false );
			pPhotonMapExportType->SetSavePreviousNextPhotonsID( false );
			pPhotonMapExportType->SetSaveSideEnabled( true );
			pPhotonMapExportType->SetSaveSurfacesIDEnabled( true );


			if( directoryRootName.empty() )
			{
				std::cout<<"No file to save results "<<std::endl;
				return ( -9 );
			}

			std::ostringstream az_convert;
			az_convert << azimuthValues[a];


			std::ostringstream el_convert;
			el_convert << elevationValues[e];

			//std::string filesRootName = argv[3] + int_convert.str();

			std::string positionDirName = "Azimuth" + az_convert.str() + "Elevation"+ el_convert.str();
			std::string positionDir = directoryRootName  +  std::string{ "/" } + positionDirName;

			pPhotonMapExportType->SetSaveParameterValue( "ExportDirectory", positionDir );
			std::cout<<"\t\tExportDirectory: "<< positionDir<<std::endl;
			pPhotonMapExportType->SetSaveParameterValue( "ExportFile", std::string{"TargetData"});
			pPhotonMapExportType->SetSaveParameterValue( "FileSize",  "1000000" );


			//std::vector< std::string > saveSurfaceList{ "//SunNode", "//SunNode/SolarField/Tower/Receiver/ReceiverRotation/ReceiverSurface", "//SunNode/SolarField/Heliostat_Field_Component/Heliostat0/HeliostatTrackerNode/SurfaceNode" };
			std::vector< std::string > saveSurfaceList{ "//SunNode/SolarField/Tower/Receiver/ReceiverRotation/ReceiverSurface" };


			pPhotonMapExportType->SetSaveSurfacesURLList( saveSurfaceList );

			pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );

			raytracer.SetPhotonMap( pPhotonMap.get() );

			double tracedRays = 10000000;
			std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
			raytracer.Run( tracedRays );
		}
	}

	return ( 0 );

}

