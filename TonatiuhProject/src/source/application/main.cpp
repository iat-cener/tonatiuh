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
#include "TNodesDocument.h"
#include "TPhotonMap.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TTrackerFactory.h"



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


void do_stuff_with_any(const std::any& obj);
/*
{
    std::cout << std::any_cast<const std::string&>(obj) << "\n";
}
*/

int main ( int argc, char** argv )
{

	std::cout<<"Tonatiuh 3.0"<<std::endl;
	qInstallMessageHandler(myMessageOutput);

    QApplication a{ argc, argv };
	a.setApplicationVersion( APP_VERSION );

	std::cout<<"Tonatiuh 3.0 TNodesDatabase"<<std::endl;
	TNodesDatabase::Init();

	/*

	 QLibrary library1("geometry.dll");
	 QLibrary library2("nodes.dll");
	 if (! library1.load() )	return ( -1 );
	 if (! library2.load() )	return ( -1 );


	 typedef void (*Vector3D )();
	 Vector3D vector3D = (Vector3D) library2.resolve("Vector3D");
	 */


	/*
	do_stuff_with_any(std::string{"Hello World"});
	Vector3D translationValue1{ -5, 0, 0 };
	std::any variantVector3D1( translationValue1 );
	do_stuff_with_any( translationValue1 );
	do_stuff_with_any(std::string{"\n\n\n"});
	*/


/************************************************************/


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



/************************************************************/

/************************************************************
	std::cout<<"Creating new tonatiuh scene"<<std::endl;

	//TSceneNode* sunNode = new TSceneNode();
	std::shared_ptr< TSceneNode > tonatiuhScene = std::dynamic_pointer_cast<TSceneNode>( TSceneNode::CreateInstance() );
	tonatiuhScene->SetName( "parabolic_dish_concentrator" );
	std::cout<<" - tonatiuhScene created."<<std::endl;


	std::vector<std::string> scenePartNames = tonatiuhScene->GetPartNames();
	std::cout<<"- Tonatiuh scene part names: "<<std::endl;
	for( unsigned int p = 0; p < scenePartNames.size(); p++ )
		std::cout<<"  -  "<<scenePartNames[p]<<std::endl;


	//Sun
	std::cout<<"Creating new scene Sun"<<std::endl;
	double azimuthRad = 180 * gc::Degree;
	double zentihRad = 15 * gc::Degree;
	std::shared_ptr< TSunNode > sunNode = std::dynamic_pointer_cast<TSunNode>( TSunNode::CreateInstance() );
	sunNode->SetName( "sun_node" );


	std::cout<<" - sunNode created."<<std::endl;
	tonatiuhScene->SetPart( scenePartNames[1],  sunNode );
	sunNode->SetParameterValue<double>( "azimuth", azimuthRad );
	sunNode->SetParameterValue<double>( "zenith", zentihRad );
	std::cout<<" - sunNode parameters: azimuth "<<azimuthRad<<" && zenith "<<zentihRad<<std::endl;

	std::cout<<"Creating sunshape: "<<std::endl;
	std::cout<<" - finding the desired sunhape index in the sunshapes list"<<std::endl;
	int sunshapeIndex = std::distance( sunshapeFactoryNameList.begin(), std::find(sunshapeFactoryNameList.begin(), sunshapeFactoryNameList.end(), "Buie_Sunshape" ) );
	if( sunshapeIndex >= numberOfSunshapeFactories )
	{
		std::cout<<"Error defining sunshape type."<<std::endl;
		return -1;
	}
	std::cout<<" - Sunshape index: "<<sunshapeIndex<<std::endl;

	//std::shared_ptr< TSunshape > sunshapeNode = sunshapeFactoryList[0]->CreateTSunshape();

	std::shared_ptr< TSunshape > sunshapeNode = pluginManager.CreateTSunshape( "Buie_Sunshape" );
	if( sunshapeNode == nullptr )
	{
		std::cout<<"Error defining 'Buie_Sunshape' sunshape type."<<std::endl;
		return -1;
	}

	sunshapeNode->SetName( "sunshapeModel" );
	sunNode->SetPart( "sunshape",  sunshapeNode );
	if( !sunshapeNode->SetParameterValue<double>( "csr", 0.028 ) )
	{
		std::cout<<"Error defining crs value."<<std::endl;
		return ( -1 );
	}




	//Parabolic dish
	std::cout<<"Eurodish_field"<<std::endl;
	std::shared_ptr< TGroupNode > rootNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );  //= new TGroupNode();
	rootNode->SetName( "Eurodish_field");
	if( !tonatiuhScene->SetPart( scenePartNames[0],  rootNode ) )
	{
		std::cout<<"Cannnot be define the scene children node."<<std::endl;
		return ( -1 );

	}
	std::cout<<"childRoot: "<<rootNode->GetName()<<std::endl;


	//Concentrator 1
	std::shared_ptr< TGroupNode > concentrator1Node = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	concentrator1Node->SetName( "Concentrator_1");
	Vector3D translationValue( -5, 0, 0 );
	std::any variantVector3D( translationValue );
	concentrator1Node->SetParameterValue<std::string>( "translation", "-5 0 0" );
	std::cout<<"  Concentrator 1 Node: "<<concentrator1Node->GetName()<<std::endl;
	if( !( rootNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( concentrator1Node ) )
	{
		std::cout<<"Error adding concentrator 1 to root node."<<std::endl;
		return -1;
	}


	std::cout<<"    concentrator 1 tracker: Tracker_Concentrator_1"<<std::endl;
	std::string trackerFactoryName = "Tracker_ParabolicDish" ;
	int trackerFactoryIndex = std::distance( trackerFactoryNameList.begin(), std::find(trackerFactoryNameList.begin(), trackerFactoryNameList.end(), trackerFactoryName ) );
	if( trackerFactoryIndex >= numberOfTrackerFactories )
	{
		std::cout<<"Error defining concentrator tracker type."<<std::endl;
		return -1;
	}

	std::shared_ptr< TTracker > c1ParabolicDishTracker = pluginManager.CreateTTracker( trackerFactoryName );
	if( c1ParabolicDishTracker == nullptr )
	{
		std::cout<<"Error defining '"<<trackerFactoryName<<"' tracker type."<<std::endl;
		return -1;
	}

	c1ParabolicDishTracker->SetName( "Tracker_Concentrator_1");
	( concentrator1Node->GetPart("childrenList" )->as< TNodesList>())->InsertItem( c1ParabolicDishTracker );


	std::cout<<"      concentrator 1 surface:  ConcentratorSurface"<<std::endl;
	std::shared_ptr< TSurfaceNode > concentratorSurfaceNode = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	concentratorSurfaceNode->SetName( "ConcentratorSurface" );
	( c1ParabolicDishTracker->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( concentratorSurfaceNode );


	std::cout<<"        concentrator 1 shape:  ConcentratorGeometry"<<std::endl;
	std::string concentratorShapeName = "Parabolic_dish";
	int concentratorShapeIndex = std::distance( shapeFactoryNameList.begin(), std::find(shapeFactoryNameList.begin(), shapeFactoryNameList.end(), concentratorShapeName ) );
	if( concentratorShapeIndex >= numberOfShapeFactories )
	{
		std::cout<<"Error defining concentrator shape: "<<concentratorShapeName<<std::endl;
		return -1;
	}

	std::shared_ptr< TShape > concentratorShape = pluginManager.CreateTShape( concentratorShapeName );
	if( concentratorShape == nullptr )
	{
		std::cout<<"Error defining '"<<concentratorShapeName<<"' shape type."<<std::endl;
		return -1;
	}

	concentratorShape->SetName( "ConcentratorGeometry");
	concentratorShape->SetParameterValue<double>( "focusLength", 4.520 );
	concentratorShape->SetParameterValue<double>( "dishMinRadius",  0.0 );
	concentratorShape->SetParameterValue<double>( "dishMaxRadius",  4.25 );
	concentratorSurfaceNode->SetPart( "shape",  concentratorShape );


	std::cout<<"        concentrator 1 material:  ConcentratorMaterial"<<std::endl;
	std::string concentratorMaterialName = "Specular_Standard_Material";
	int concentratorMaterialIndex = std::distance( materialFactoryNameList.begin(), std::find(materialFactoryNameList.begin(), materialFactoryNameList.end(), concentratorMaterialName ) );
	if( concentratorMaterialIndex >= numberOfMaterialFactories )
	{
		std::cout<<"Error defining concentrator material: "<<concentratorMaterialName<<std::endl;
		return -1;
	}

	std::shared_ptr< TMaterial > concentratorMaterial = pluginManager.CreateTMaterial( concentratorMaterialName );
	if( concentratorMaterial == nullptr )
	{
		std::cout<<"Error defining '"<<concentratorMaterialName<<"' material type."<<std::endl;
		return -1;
	}
	concentratorMaterial->SetName( "ConcentratorMaterial");
	concentratorMaterial->SetParameterValue<double>( "reflectivity", 0.925 );
	concentratorMaterial->SetParameterValue<double>( "sigmaSlope", 4.4270 );
	concentratorSurfaceNode->SetPart( "material",  concentratorMaterial );



	//Receiver
	std::cout<<"    receiverNode: Receiver"<<std::endl;
	std::shared_ptr< TGroupNode > receiverNode = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	receiverNode->SetName( "Receiver ");
	receiverNode->SetParameterValue<std::string>( "translation",  "0 4.52 0" );
	( c1ParabolicDishTracker->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( receiverNode );


	std::cout<<"      receiverSurfaceNode: ReceiverSurface"<<std::endl;
	std::shared_ptr< TSurfaceNode > receiverSurfaceNode = std::dynamic_pointer_cast<TSurfaceNode>( TSurfaceNode::CreateInstance() );
	receiverSurfaceNode->SetName( "ReceiverSurface" );
	( receiverNode->GetPart("childrenList" )->as< TNodesList >() )->InsertItem( receiverSurfaceNode );


	std::cout<<"          receiver  shape:  ReceiverGeometry"<<std::endl;
	std::string receiverShapeName = "Flat_disk";
	int receiverShapeIndex = std::distance( shapeFactoryNameList.begin(), std::find(shapeFactoryNameList.begin(), shapeFactoryNameList.end(), receiverShapeName ) );
	if( receiverShapeIndex >= numberOfShapeFactories )
	{
		std::cout<<"Error defining receiver shape: "<<receiverShapeName<<std::endl;
		return -1;
	}
	std::shared_ptr< TShape > receiverShape = pluginManager.CreateTShape( receiverShapeName );
	if( receiverShape == nullptr )
	{
		std::cout<<"Error defining '"<<receiverShapeName<<"' shape type."<<std::endl;
		return( -1 );
	}

	receiverShape->SetName( "ReceiverGeometry");
	receiverShape->SetParameterValue<double>( "radius",  0.30 );
	receiverSurfaceNode->SetPart( "shape",  receiverShape );


	std::cout<<"          receiver  material:  ReceiverMaterial"<<std::endl;
	std::shared_ptr< TMaterial > receiverMaterial = pluginManager.CreateTMaterial( concentratorMaterialName );
	if( receiverMaterial == nullptr )
	{
		std::cout<<"Error defining '"<<concentratorMaterialName<<"' material type."<<std::endl;
		return( -1 );
	}
	receiverMaterial->SetName( "ReceiverMaterial");
	receiverMaterial->SetParameterValue<double>( "reflectivity", 0.0 );
	receiverSurfaceNode->SetPart( "material",  receiverMaterial );



	//Concentrator 2
	std::cout<<"  Concentrator 2 Node: Concentrator_2"<<std::endl;
	std::shared_ptr< TGroupNode > concentrator2Node = std::dynamic_pointer_cast<TGroupNode>( TGroupNode::CreateInstance() );
	concentrator2Node->SetName( "Concentrator_2");
	concentrator2Node->SetParameterValue<std::string>( "translation",  "5 0 0" );
	if( !( rootNode->GetPart( "childrenList" )->as< TNodesList >() )->InsertItem( concentrator2Node ) )
	{
		std::cout<<"Error adding concentrator 2 to root node"<<std::endl;
		return( -1 );
	}


	std::cout<<"    concentrator2 tracker"<<std::endl;
	std::shared_ptr< TTracker > c2ParabolicDishTracker = pluginManager.CreateTTracker( trackerFactoryName );
	if( c2ParabolicDishTracker == nullptr )
	{
		std::cout<<"Error defining '"<<trackerFactoryName<<"' tracker type."<<std::endl;
		return( -1 );
	}
	c2ParabolicDishTracker->SetName( "Tracker_Concentrator_2");
	( concentrator2Node->GetPart("childrenList" )->as< TNodesList>())->InsertItem( c2ParabolicDishTracker );


	std::cout<<"      concentrator 2 surface:  ConcentratorSurface"<<std::endl;
	if( !( c2ParabolicDishTracker->GetPart( "childrenList" )->as< TNodesList >() )->InsertItem( concentratorSurfaceNode ) )
	{
		std::cout<<"Error adding surface node to concentrator 2 tracker"<<std::endl;
		return( -1 );
	}



	std::cout<<"\tUpdating trackers..."<<std::endl;
	tonatiuhScene->UpdateTrackers( );

	std::cout<<"TNodesDocument saveDocument"<<std::endl;
	TNodesDocument saveDocument;
	saveDocument.SetRootNode( tonatiuhScene );
	if( !saveDocument.Write( argv[1] )  )
	{
		std::cout<<"Error during writing file"<<std::endl;
		return -1;
	}

	////Codigo para si se eliminan los nodos
	//std::cout<<"Removing scene node"<<std::endl;
	//tonatiuhScene->RemoveReference();
	//std::cout<<"Removed scene node"<<std::endl;
	//tonatiuhScene = 0;




***********************************/



/***********************************/
	std::cout<<"Tonatiuh 3.0 readDocument"<<std::endl;

	TNodesDocument readDocument;
	bool okRead = readDocument.Read( argv[1] );
	if( !okRead  )
	{
		std::cout<<"¡¡¡Error reading file!!!"<<std::endl;
		return ( -1 );
	}


	std::cout<<"Tonatiuh 3.0 sceneNode"<<std::endl;
	std::shared_ptr< TSceneNode > sceneNode = std::dynamic_pointer_cast< TSceneNode > ( readDocument.GetRootNode() );
	if( !sceneNode || sceneNode == 0 )
	{
		std::cout<<"¡¡¡Error reading file!!!"<<std::endl;
		return ( -2 );
	}

	std::cout<<"\tUpdating trackers..."<<std::endl;
	sceneNode->UpdateTrackers( );



/***********************************/

/************************************************************


	std::cout<<"Tonatiuh 3.0 saveDocument"<<std::endl;
	TNodesDocument saveDocument2;
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

/***********************************/


	//Change sun position
	std::cout<<"Change sun position:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();
	std::cout<<"\tCurrent values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


	std::cout<<"\tChanging sun position..."<<std::endl;
	sceneNode->ChangeSunPosition( 180*gc::Degree, 0*gc::Degree );


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


	//Creating the photon map
	std::cout<<"\tCreating the photon map"<<std::endl;
	std::unique_ptr< TPhotonMap > pPhotonMap = std::make_unique < TPhotonMap >();

	//Defining how export the stored data.
	std::cout<<"\t Defining how export the stored data."<<std::endl;
	std::unique_ptr< PhotonMapExportType > pPhotonMapExportType = pluginManager.CreatePhotonMapExportType( photonMapExportTypeNameList[0] );
	pPhotonMapExportType->SetSaveCoordinatesEnabled( true );
	pPhotonMapExportType->SetSavePreviousNextPhotonsID( false );

	std::cout<<"\t\tExportDirectory: "<< argv[3]<<std::endl;
	std::cout<<"\t\tExportFile: "<< argv[4]<<std::endl;

	if( argv[3] == NULL || argv[4] == NULL )
	{
		std::cout<<"No file to save results "<<std::endl;
		return ( -8 );
	}

	pPhotonMapExportType->SetSaveParameterValue( "ExportDirectory", argv[3] );
	pPhotonMapExportType->SetSaveParameterValue( "ExportFile", argv[4] );
	pPhotonMap->SetExportType( std::move( pPhotonMapExportType ) );

	raytracer.SetPhotonMap( pPhotonMap.get() );

	double tracedRays = 1000000;

	std::cout<<"Run "<<tracedRays<<" rays"<<std::endl;
	raytracer.Run( tracedRays );


	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;

	std::cout<<"END main"<<std::endl;




/************************************************************/

/************************************************************
	//Change sun position
	std::cout<<"Change sun position:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();
	std::cout<<"\tCurrent values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;
************************************************************/


	std::cout<<"END" <<std::endl;
	return ( 0 );
}



