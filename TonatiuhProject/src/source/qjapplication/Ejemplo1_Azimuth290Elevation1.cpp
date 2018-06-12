/*
 * Ejemplo1_Azimuth290Elevation1.cpp
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

int Ejemplo1_Azimuth290Elevation1(std::shared_ptr< TSceneNode >& sceneNode, const PluginManager& pluginManager, std::string directoryRootName,
		std::string randomFactoryName, std::string photonMapExportTypeName  )
{
	//Ejemplo 1. Convergencia eficiencia

	//Change sun position
	std::cout<<"Changing sun position..."<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();
	sceneNode->ChangeSunPosition( 290*gc::Degree, ( 90 - 1 )*gc::Degree );
	std::cout<<"\tNew values:"<<std::endl;
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
	if( randomFactoryName.empty()  )
		return ( -6 );
	pRand =  pluginManager.CreateRandomDeviate( randomFactoryName );
	if( pRand == nullptr )
		return ( -7 );

	raytracer.SetRandomNumberGenerator( pRand.get() );


	for( int s = 0; s < 20; s++ )
	{
		//Creating the photon map
		std::cout<<"\tCreating the photon map"<<std::endl;
		std::unique_ptr< TPhotonMap > pPhotonMap = std::make_unique < TPhotonMap >();
		pPhotonMap->SetBufferSize( 1000000 );

		//Defining how export the stored data.
		std::cout<<"\t Defining how export the stored data."<<std::endl;
		if( photonMapExportTypeName.empty()  )
			return ( -8 );
		std::unique_ptr< PhotonMapExportType > pPhotonMapExportType = pluginManager.CreatePhotonMapExportType( photonMapExportTypeName );
		pPhotonMapExportType->SetSaveCoordinatesEnabled( true );
		pPhotonMapExportType->SetSaveCoordinatesInGlobalSystemEnabled( false );
		pPhotonMapExportType->SetSavePreviousNextPhotonsID( false );
		pPhotonMapExportType->SetSaveSideEnabled( true );
		pPhotonMapExportType->SetSaveSurfacesIDEnabled( true );


		if( directoryRootName.empty()  )
		{
			std::cout<<"No file to save results "<<std::endl;
			return ( -9 );
		}

		std::ostringstream int_convert;
		int_convert << s;

		std::string filesRootName = std::string{ "TargetData_s" } + int_convert.str();
		pPhotonMapExportType->SetSaveParameterValue( "ExportDirectory", directoryRootName );
		std::cout<<"\t\tExportDirectory: "<< directoryRootName<<std::endl;
		pPhotonMapExportType->SetSaveParameterValue( "ExportFile", filesRootName );
		std::cout<<"\t\tExportFile: "<<filesRootName<<std::endl;
		pPhotonMapExportType->SetSaveParameterValue( "FileSize",  "1000000" );


		//std::vector< std::string > saveSurfaceList{ "//SunNode", "//SunNode/SolarField/Tower/Receiver/ReceiverRotation/ReceiverSurface", "//SunNode/SolarField/Heliostat_Field_Component/Heliostat0/HeliostatTrackerNode/SurfaceNode" };
		std::vector< std::string > saveSurfaceList{ "//SunNode/SolarField/Tower/Receiver/ReceiverRotation/ReceiverSurface" };


		pPhotonMapExportType->SetSaveSurfacesURLList( saveSurfaceList );

		pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );

		raytracer.SetPhotonMap( pPhotonMap.get() );


		double tracedRays = 5000000;

		std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
		raytracer.Run( tracedRays );
	}

	return ( 0 );
}


