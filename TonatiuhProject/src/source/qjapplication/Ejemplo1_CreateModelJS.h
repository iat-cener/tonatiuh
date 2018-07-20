/*
 * Ejemplo1_CreateModel.h
 *
 *  Created on: 18 jul. 2018
 *      Author: amutuberria
 */

#ifndef EJEMPLO1_CREATEMODEL_H_
#define EJEMPLO1_CREATEMODEL_H_

#include <fstream>
#include <sstream>
#include "TJSONNodesDocument.h"

/*!
 * Reads a CSV file and stores the data in a vector.
 */
std::vector<std::string> readCSVParameters(std::istream& str, char delim)
{
    std::vector< std::string >   result;
    std::string cell;

    while( std::getline(str, cell, delim) )
        result.push_back(cell);
    return result;
}


/*!
 * This function creates the model for the example 1.
 */
void Ejemplo1_CreateModel( std::string currentDirectory, PluginManager* pPluginManager )
{

	std::cout<< "\n\n**************************************************"<<std::endl;
	std::cout<< "Ejemplo1_CreateModel"<<std::endl;


	std::cout<<"Creating new tonatiuh scene"<<std::endl;

	//TSceneNode* sunNode = new TSceneNode();
	std::shared_ptr< TSceneNode > tonatiuhScene = std::dynamic_pointer_cast<TSceneNode>( TSceneNode::CreateInstance() );
	tonatiuhScene->SetName( "Example1_Heliostat_Field" );

	/*
	std::vector<std::string> scenePartNames = tonatiuhScene->GetPartNames();
	std::cout<<"\t Tonatiuh scene part names: "<<std::endl;
	for( unsigned int p = 0; p < scenePartNames.size(); p++ )
		std::cout<<"\t\t - "<<scenePartNames[p]<<std::endl;
	*/

	QDir workingPath{ currentDirectory.c_str() };
	if( !workingPath.cd( "Ejemplo1" ) )
	{
		std::cerr<<"ERROR 'Ejemplo1' folder does not exists"<<std::endl;
		return;
	}


	//Sun
	std::cout<<"Creating scene sun"<<std::endl;
	double azimuthRad = 180 * gc::Degree;
	double zentihRad = 65 * gc::Degree;
	std::shared_ptr< TSunNode > sunNode = std::dynamic_pointer_cast<TSunNode>( TSunNode::CreateInstance() );
	sunNode->SetName( "SunNode" );
	if( !tonatiuhScene->SetPart( "light",  sunNode ) )
	{
		std::cerr<<"Cannnot be define the scene light node."<<std::endl;
		return;
	}


	sunNode->SetParameterValue<double>( "azimuth", azimuthRad );
	sunNode->SetParameterValue<double>( "zenith", zentihRad );
	std::cout<<"\t - sun parameters: \n\t - azimuth "<<azimuthRad<<"\n\t - zenith "<<zentihRad<<std::endl;

	std::cout<<"Creating sunshape: "<<std::endl;
	std::shared_ptr< TSunshape > sunshapeNode = pPluginManager->CreateTSunshape( "Buie_Sunshape" );
	if( sunshapeNode == nullptr )
	{
		std::cerr<<"Error creating 'Buie_Sunshape' sunshape type node."<<std::endl;
		return;
	}
	sunshapeNode->SetName( "BuieSunshape" );
	sunNode->SetPart( "sunshape",  sunshapeNode );
	sunshapeNode->SetParameterValue<double>( "irradiance", 950 );
	if( !sunshapeNode->SetParameterValue<double>( "csr", 0.028 ) )
	{
		std::cerr<<"Error defining crs value for 'BuieSunshape' node."<<std::endl;
		return;
	}

	//Transmissivity
	std::cout<<"Creating transmissivity: 'Ballestrin Transmissivity'"<<std::endl;
	std::shared_ptr< TTransmissivity > transmissivityNode = pPluginManager->CreateTTransmissivity( "Ballestrin Transmissivity" );
	if( transmissivityNode == nullptr )
	{
		std::cerr<<"ERROR creating 'Ballestrin Transmissivity' transmissivity type node."<<std::endl;
		return;
	}
	transmissivityNode->SetName( "BallestrinTransmissivity");
	if( !tonatiuhScene->SetPart( "transmisivity",  transmissivityNode ) )
	{
		std::cerr<<"Cannnot be define the scene transmissivity node."<<std::endl;
		return;
	}

	//Solar field  node
	std::cout<<"Creating solar field"<<std::endl;
	std::shared_ptr< TGroupNode > solarFieldNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	solarFieldNode->SetName( "solarField_node" );
	if( !tonatiuhScene->SetPart( "childrenRoot",  solarFieldNode ) )
	{
		std::cerr<<"Cannnot be define the scene children node."<<std::endl;
		return;

	}

	//Tower node
	std::cout<<"Creating Tower"<<std::endl;
	std::shared_ptr< TGroupNode > towerRootNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	towerRootNode->SetName( "tower_node" );
	if( !( solarFieldNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( towerRootNode ) )
	{
		std::cerr<<"Error adding tower node to solar field node."<<std::endl;
		return;
	}

	//Base Tower node
	std::shared_ptr< TGroupNode > baseTowerNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	baseTowerNode->SetName( "base_node" );
	baseTowerNode->SetParameterValue<std::string>( "rotation", "-1 0 0  1.5707964" );
	baseTowerNode->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );
	if( !( towerRootNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( baseTowerNode ) )
	{
		std::cerr<<"Error adding Tower to root node."<<std::endl;
		return;
	}

	//! Add Tower shape
	std::shared_ptr< TSurfaceNode > baseBottomSurface = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	baseBottomSurface->SetName( "BaseBottomSurface" );
	( baseTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( baseBottomSurface );


	//! Add base botoom surface shape
	std::string cylinderShapeName = "Cylinder";
	std::shared_ptr< TShape > baseBottomShape = pPluginManager->CreateTShape( cylinderShapeName );
	if( baseBottomShape == nullptr )
	{
		std::cerr<<"Error defining '"<<cylinderShapeName<<"' shape type."<<std::endl;
		return;
	}
	baseBottomShape->SetParameterValue<double>( "radius", 5);
	baseBottomShape->SetParameterValue<double>( "length",  193 );
	baseBottomShape->SetParameterValue<double>("phiMax", 6.2831855);
	baseBottomSurface->SetPart( "shape",  baseBottomShape );

	std::shared_ptr< TGroupNode > coverTowerNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	coverTowerNode->SetName( "cover" );
	coverTowerNode->SetParameterValue<std::string>( "translation", "0 0 193" );
	coverTowerNode->SetParameterValue<std::string>( "rotation", "1 0 0  1.5707964" );
	coverTowerNode->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );
	if( !( baseTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( coverTowerNode ) )
	{
		std::cerr<<"Error adding Tower to root node."<<std::endl;
		return;
	}

	//! Add Cover surface
	std::shared_ptr< TSurfaceNode > topCoverSurface = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	topCoverSurface->SetName( "TopCoverSurface" );
	( coverTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( topCoverSurface );


	//! Add base botoom surface shape
	std::string flatDiskShapeName = "Flat_disk";
	std::shared_ptr< TShape > flatDiskShape = pPluginManager->CreateTShape( flatDiskShapeName );
	if( flatDiskShape == nullptr )
	{
		std::cerr<<"Error createing a '"<<flatDiskShapeName<<"' shape type."<<std::endl;
		return;
	}
	flatDiskShape->SetParameterValue<double>( "radius", 5 );
	topCoverSurface->SetPart( "shape",  flatDiskShape );

	//Receiver Tower node
	std::shared_ptr< TGroupNode > receiverTowerNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	receiverTowerNode->SetName( "receiver_node" );
	if( !( towerRootNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( receiverTowerNode ) )
	{
		std::cerr<<"Error adding receiver node to tower node."<<std::endl;
		return;
	}
	receiverTowerNode->SetParameterValue<std::string>( "translation", "0 170 0" );
	receiverTowerNode->SetParameterValue<std::string>( "rotation", "0 -1 0  1.5707964" );
	receiverTowerNode->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );

	//Receiver Rotation
	std::shared_ptr< TGroupNode > receiverRotation = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	receiverRotation->SetName( "receiver_rotation" );
	if( !( receiverTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( receiverRotation ) )
	{
		std::cerr<<"Error adding receiver rotation node."<<std::endl;
		return;
	}
	receiverRotation->SetParameterValue<std::string>( "translation", "0 12 0" );
	receiverRotation->SetParameterValue<std::string>( "rotation", "1 0 0  1.5707964" );
	receiverRotation->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );


	//! Add receiver surface
	std::shared_ptr< TSurfaceNode > receiverSurface = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	receiverSurface->SetName( "ReceiverSurface" );
	( receiverRotation->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( receiverSurface );

	//! Add receiver shape
	std::shared_ptr< TShape > receiverShape = pPluginManager->CreateTShape( cylinderShapeName );
	if( receiverShape == nullptr )
	{
		std::cerr<<"Error creating '"<<cylinderShapeName<<"' shape type for the receiver."<<std::endl;
		return;
	}
	receiverShape->SetParameterValue<double>( "radius", 9);
	receiverShape->SetParameterValue<double>( "length",  24 );
	receiverShape->SetParameterValue<double>("phiMax", 6.2831855);
	receiverSurface->SetPart( "shape",  receiverShape );

	//Top cover
	std::shared_ptr< TGroupNode > topCover = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	topCover->SetName( "top_cover" );
	if( !( receiverTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( topCover ) )
	{
		std::cerr<<"Error adding to the tower the top cover node."<<std::endl;
		return;
	}
	topCover->SetParameterValue<std::string>( "translation", "0 12 0" );
	topCover->SetParameterValue<std::string>( "rotation", "0 0 1  0" );
	topCover->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );

	//! Add top cover surface
	std::shared_ptr< TSurfaceNode > topCoverSurfaceR = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	topCoverSurfaceR->SetName( "TopCoverSurface" );
	( topCover->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( topCoverSurfaceR );

	//! Add top cover shape
	std::shared_ptr< TShape > topCoverShape = pPluginManager->CreateTShape( flatDiskShapeName );
	if( topCoverShape == nullptr )
	{
		std::cerr<<"Error creating '"<<flatDiskShapeName<<"' shape type for the top cover."<<std::endl;
		return;
	}

	topCoverShape->SetParameterValue<double>( "radius", 9);
	topCoverSurfaceR->SetPart( "shape",  topCoverShape );

	//Botton cover
	std::shared_ptr< TGroupNode > bottomCover = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	bottomCover->SetName( "botton_cover" );
	if( !( receiverTowerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( bottomCover ) )
	{
		std::cerr<<"Error adding Tower to root node."<<std::endl;
		return;
	}
	bottomCover->SetParameterValue<std::string>( "translation", "0 -12 0" );
	bottomCover->SetParameterValue<std::string>( "rotation", "0 0 1  0" );
	bottomCover->SetParameterValue<std::string>( "scaleFactor", "1 1 1" );


	//! Add bottom cover surface
	std::shared_ptr< TSurfaceNode > bottomCoverSurface = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	bottomCoverSurface->SetName( "BottomCoverSurface" );
	( bottomCover->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( bottomCoverSurface );

	//! Add bottom cover shape
	std::shared_ptr< TShape > bottomCoverShape = pPluginManager->CreateTShape( flatDiskShapeName );
	if( bottomCoverShape == nullptr )
	{
		std::cerr<<"ERROR creating '"<<flatDiskShapeName<<"' shape type for the bottom cover."<<std::endl;
		return;
	}

	bottomCoverShape->SetParameterValue<double>( "radius", 9);
	bottomCoverSurface->SetPart( "shape",  bottomCoverShape );

	//Heliostat List
	if( !workingPath.cd( "Input" ) )
	{
		std::cerr<<"ERROR 'Input' folder does not exists inside 'Ejemplo1' folder."<<std::endl;
		return;
	}
	if( !workingPath.exists( "Ejemplo1_Coordeandas&Aimingcvs.csv" ) )
	{
		std::cerr<<"ERROR: The file 'Ejemplo1_Coordeandas&Aimingcvs.csv'with the heliostat coordinates does not exists inside 'Input' folder."<<std::endl;
		return;
	}
	std::string inputFilename = workingPath.absoluteFilePath( "Ejemplo1_Coordeandas&Aimingcvs.csv" ).toStdString();

	std::cout<<"Creating heliostat list"<<std::endl;
	std::shared_ptr< TGroupNode > heliostatFieldNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	heliostatFieldNode->SetName( "HeliostatFiled" );
	if( !( solarFieldNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( heliostatFieldNode ) )
	{
		std::cerr<<"Error 'HeliostatFiled' to 'solarField_node'."<<std::endl;
		return;
	}

	std::ifstream file( inputFilename );
	std::vector<std::string> csvLines = readCSVParameters(file, '\n');

	std::string trackerFactoryName = "Heliostat_tracker" ;
	std::string heliostatMirrorShapeName = "Spherical_rectangle";
	std::string heliostatMirrorMaterialName = "Specular_Standard_Material";
	for( unsigned int h = 0; h < csvLines.size(); h++ )
	{
		std::cout<<"\t Creating Heliostat"<<h<<std::endl;

		//! Read traslation and aiming point from file:
		std::istringstream iss( csvLines[h] );
		std::vector<std::string> coord = readCSVParameters(iss, ';');
		if( coord.size() != 7 )
		{
			std::cerr<<"ERROR reading cvs file in line "<<h<<std::endl;
			return;
		}

		//! Traslation

		//! Traslation
		Point3D heliostatPosition3D;
		std::stringstream{ coord[1] } >> heliostatPosition3D.x;
		std::stringstream{ coord[2] } >> heliostatPosition3D.y;
		std::stringstream{ coord[3] } >> heliostatPosition3D.z;

		//! Aiming point
		Point3D aimingPoint3D;
		std::stringstream{ coord[4] } >> aimingPoint3D.x;
		std::stringstream{ coord[5] } >> aimingPoint3D.y;
		std::stringstream{ coord[6] } >> aimingPoint3D.z;

		//Heliostat h
		std::cout<<"\t\t Creating heliostat: "<<std::endl;
		std::shared_ptr< TGroupNode > heliostatNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
		heliostatNode->SetName("Heliostat" + std::to_string(h) );
		heliostatNode->SetParameterValue<std::string>( "translation", coord[1] + " " + coord[2] + " " + coord[3] );
		if( !( heliostatFieldNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( heliostatNode ) )
		{
			std::cerr<<"Error adding 'Helisotat"<<h<<"' to 'HeliostatFiled' node."<<std::endl;
			return;
		}

		//! Add heliostat tracker
		std::cout<<"\t\t Creating heliostat tracker "<<std::endl;
		std::shared_ptr< TTracker > trackerNode = pPluginManager->CreateTTracker( trackerFactoryName );
		if( trackerNode == nullptr )
		{
			std::cerr<<"Error creating '"<<trackerFactoryName<<"' tracker type node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}
		trackerNode->SetName( "Heliostat_Tracker");
		trackerNode->SetParameterValue<Point3D>( "aimingPoint", aimingPoint3D );
		( heliostatNode->GetPart("childrenList" )->as< TNodesList>())->InsertItem( trackerNode );


		//! Add heliostat Surface node
		std::cout<<"\t\t Creating heliostat surface"<<std::endl;
		std::shared_ptr< TSurfaceNode > heliostatMirrorSurfaceNode = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
		heliostatMirrorSurfaceNode->SetName( "HeliostatMirrorSurface" );
		if( !( trackerNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( heliostatMirrorSurfaceNode ) )
		{
			std::cerr<<"Error adding surface node to heliostat tracker node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}

		//! Add heliostat shape node
		std::cout<<"\t\t Creating mirror shape node"<<std::endl;
		std::shared_ptr< TShape > heliostatShape = pPluginManager->CreateTShape( heliostatMirrorShapeName );
		if( heliostatShape == nullptr )
		{
			std::cerr<<"ERROR creating '"<<heliostatMirrorShapeName<<"' shape type node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}
		heliostatShape->SetName( "HeliostatMirrorGeometry");


		//! The radius must be calculated as 2*slant range
		double heliostatRadius = 2* Distance( heliostatPosition3D, aimingPoint3D);
		heliostatShape->SetParameterValue<double>( "radius", heliostatRadius);
		heliostatShape->SetParameterValue<double>( "widthX",  11.2592 );
		heliostatShape->SetParameterValue<double>( "widthZ",  10.2778 );
		if( !heliostatMirrorSurfaceNode->SetPart( "shape",  heliostatShape ) )
		{
			std::cerr<<"Error adding shape node to heliostat mirror surface node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}

		//! Add heliostat material
		std::cout<<"\t\t Creating mirror material node"<<std::endl;
		std::shared_ptr< TMaterial > heliostatMaterial = pPluginManager->CreateTMaterial( heliostatMirrorMaterialName );
		if( heliostatMaterial == nullptr )
		{
			std::cerr<<"Error defining '"<<heliostatMirrorMaterialName<<"' material type node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}
		heliostatMaterial->SetName( "HeliostatMaterial");
		heliostatMaterial->SetParameterValue<double>( "reflectivity", 0.88 );
		heliostatMaterial->SetParameterValue<double>( "sigmaSlope", 1.068 );
		if( !heliostatMirrorSurfaceNode->SetPart( "material",  heliostatMaterial ) )
		{
			std::cerr<<"Error adding material node to heliostat mirror surface node for 'Helisotat"<<h<<"'."<<std::endl;
			return;
		}
	}



	std::cout<<"\tUpdating trackers..."<<std::endl;
	tonatiuhScene->UpdateTrackers( );


	workingPath.cdUp();
	std::string filename = workingPath.absoluteFilePath( "Ejemplo1_8534heliostatos_aimingStrategy_v3JS.tnhs" ).toStdString();

	std::cout<<"Saving the scene..."<<std::endl;
	TJSONNodesDocument saveDocument;
	saveDocument.SetRootNode( tonatiuhScene );
	if( !saveDocument.Write( filename )  )
	{
		std::cerr<<"Error during writing file"<<std::endl;
		return;
	}
	std::cout<<"The process has finished successfully."<<std::endl;

}


#endif /* EJEMPLO1_CREATEMODEL_H_ */
