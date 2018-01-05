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

#include <QLibrary>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVariant>

#include <TNodesDatabase.h>

#include "PluginManager.h"
#include "TMaterialFactory.h"
#include "TNodesDocument.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TTrackerFactory.h"
//#include <Vector3D.h>
/*
#include <nf.h>
#include <TCuboid.h>
#include "../../nodes/TGroupNode.h"
#include "../../nodes/TGroupNode.h"
#include "../../nodes/TMaterial.h"
#include "../../nodes/TNodesDatabase.h"
#include "../../nodes/TParameterList.h"
#include <TSceneNode.h>
#include "../../nodes/TSunNode.h"
#include "../../nodes/TSurfaceNode.h"
#include "gc.h"
#include "RandomDeviateFactory.h"
#include "RayCasting.h"
#include "TNodesDocument.h"
#include "TPhotonMap.h"
#include "Vector3D.h"
*/



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

	qInstallMessageHandler(myMessageOutput);

    QApplication a{ argc, argv };
	a.setApplicationVersion( APP_VERSION );

	std::cout<<"Tonatiuh 3.0"<<std::endl;
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




	QDir pluginsDirectory( qApp->applicationDirPath() );
	pluginsDirectory.cd( "plugins" );
	PluginManager pluginManager;
	QString err;
	pluginManager.LoadAvailablePlugins( pluginsDirectory, &err );
	if( !err.isEmpty() )
	{
		std::cerr<<err.toStdString()<<std::endl;
	}

	QVector< TMaterialFactory* > materialFactoryList =  pluginManager.GetMaterialFactories();
	std::vector< std::string > materialFactoryName;
	for( int p = 0; p < materialFactoryList.count(); p++ )
		materialFactoryName.push_back( materialFactoryList[p]->TMaterialName() );
	QVector< TShapeFactory* > shapeFactoryList =  pluginManager.GetShapeFactories();
	std::vector< std::string > shapeFactoryName;
	for( int p = 0; p < shapeFactoryList.count(); p++ )
		shapeFactoryName.push_back( shapeFactoryList[p]->TShapeName() );

	QVector< TSunshapeFactory* > sunshapeFactoryList = pluginManager.GetSunshapeFactories();
	std::vector< std::string >  sunshapeFactoryName;
	for( int p = 0; p < sunshapeFactoryList.count(); p++ )
		sunshapeFactoryName.push_back( sunshapeFactoryList[p]->TSunshapeName() );

	QVector< TTrackerFactory* > trackerFactoryList = pluginManager.GetTrackerFactories();
	std::vector< std::string > trackerFactoryName;
	for( int p = 0; p < trackerFactoryList.count(); p++ )
		trackerFactoryName.push_back( trackerFactoryList[p]->TTrackerNodeName() );
	QVector< RandomDeviateFactory* > randomDeviateFactoryList = pluginManager.GetRandomDeviateFactories();



	std::cout<<"Creating new tonatiuh scene"<<std::endl;
	TSceneNode* tonatiuhScene = static_cast<TSceneNode*>( TSceneNode::CreateInstance() );
	std::cout<<" - tonatiuhScene created."<<std::endl;

	//Sun
	std::cout<<"Creating new scene Sun"<<std::endl;
	double azimuthRad = 180 * gc::Degree;
	double zentihRad = 15 * gc::Degree;
	TSunNode* sunNode = new TSunNode();
	std::cout<<" - sunNode created."<<std::endl;
	tonatiuhScene->SetPart( "light",  sunNode );
	sunNode->SetParameterValue<double>( "azimuth", azimuthRad );
	std::cout<<" - sunNode parameters: azimuth "<<azimuthRad<<" && zenith "<<zentihRad<<std::endl;
	sunNode->SetParameterValue<double>( "zenith", zentihRad );

	std::cout<<" - finding the desired sunhape index in the sunshapes list"<<std::endl;
	int sunshapeIndex = std::distance( sunshapeFactoryName.begin(), std::find(sunshapeFactoryName.begin(), sunshapeFactoryName.end(), "Buie_Sunshape" ) );
	if( sunshapeIndex >= sunshapeFactoryList.size() )
	{
		std::cout<<"Error defining sunshape type."<<std::endl;
		return -1;
	}
	TSunshape* sunshapeNode = sunshapeFactoryList[0]->CreateTSunshape();
	sunshapeNode->SetName( "sunshapeModel");
	sunNode->SetPart( "sunshape",  sunshapeNode );
	if( !sunshapeNode->SetParameterValue<double>( "csr", 0.028 ) )
	{
		std::cout<<"Error defining crs value."<<std::endl;
		return -1;
	}



	//Parabolic dish
	std::cout<<"rootNode"<<std::endl;
	TGroupNode* rootNode = new TGroupNode();
	rootNode->SetName( "Eurodish_field");
	tonatiuhScene->SetPart( "childrenList",  rootNode );
	std::cout<<"rootNode: "<<rootNode->GetName()<<std::endl;

	//Concentrator 1
	TGroupNode* concentrator1Node = new TGroupNode();
	concentrator1Node->SetName( "Concentrator_1");
	Vector3D translationValue( -5, 0, 0 );
	std::any variantVector3D( translationValue );
	concentrator1Node->SetParameterValue<std::string>( "translation", "-5 0 0" );
	std::cout<<"concentratorNode: "<<concentrator1Node->GetName()<<std::endl;
	if( !( rootNode->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( concentrator1Node ) )
	{
		std::cout<<"Error adding concentrator 1 to root node."<<std::endl;
		return -1;
	}

	int trackerFactoryIndex = std::distance( trackerFactoryName.begin(), std::find(trackerFactoryName.begin(), trackerFactoryName.end(), "One Axis tracker" ) );
	if( trackerFactoryIndex >= trackerFactoryList.size() )
	{
		std::cout<<"Error defining concentrator tracker type."<<std::endl;
		return -1;
	}
	TTrackerNode* c1ParabolicDishTracker = trackerFactoryList[trackerFactoryIndex]->CreateTTrackerNode();
	c1ParabolicDishTracker->SetName( "Tracker_Concentrator_1");
	( concentrator1Node->GetPart("childrenList" )->as< TNodesList>())->InsertItem( c1ParabolicDishTracker );




	std::cout<<"TSurfaceNode"<<std::endl;
	TSurfaceNode* concentratorSurfaceNode = new TSurfaceNode();
	concentratorSurfaceNode->SetName( "ConcentratorSurface");
	( c1ParabolicDishTracker->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( concentratorSurfaceNode );

	int concentratorShapeFactoryIndex = std::distance( shapeFactoryName.begin(), std::find(shapeFactoryName.begin(), shapeFactoryName.end(), "Parabolic_dish" ) );
	if( concentratorShapeFactoryIndex >= shapeFactoryList.size() )
	{
		std::cout<<"Error defining concentrator shape."<<std::endl;
		return -1;
	}

	TShape* concentratorShape = shapeFactoryList[concentratorShapeFactoryIndex]->CreateTShape();
	concentratorShape->SetName( "ConcentratorGeometry");
	concentratorShape->SetParameterValue<double>( "focusLength", 4.520 );
	concentratorShape->SetParameterValue<double>( "dishMinRadius",  0.0 );
	concentratorShape->SetParameterValue<double>( "dishMaxRadius",  4.25 );
	concentratorSurfaceNode->SetPart( "shape",  concentratorShape );

	int concentratorMaterialIndex = std::distance( materialFactoryName.begin(), std::find(materialFactoryName.begin(), materialFactoryName.end(), "Specular_Standard_Material" ) );
	if( concentratorMaterialIndex >= materialFactoryList.size() )
	{
		std::cout<<"Error defining concentrator material."<<std::endl;
		return -1;
	}

	if( materialFactoryList.count() > 0 )
	{
		TMaterial* concentratorMaterial = materialFactoryList[concentratorMaterialIndex]->CreateTMaterial();
		concentratorMaterial->SetParameterValue<double>( "reflectivity", 0.925 );
		concentratorMaterial->SetParameterValue<double>( "sigmaSlope", 4.4270 );
		concentratorSurfaceNode->SetPart( "material",  concentratorMaterial );
	}

	//Receiver
	std::cout<<"Receiver"<<std::endl;
	TGroupNode* receiverNode = new TGroupNode();
	std::cout<<"\treceiverNode: "<<receiverNode->GetName()<<std::endl;
	receiverNode->SetName( "Receiver ");

	receiverNode->SetParameterValue<std::string>( "translation",  "0 4.52 0" );
	( c1ParabolicDishTracker->GetPart("childrenList" )->as< TNodesList>() )->InsertItem( receiverNode );

	std::cout<<"TSurfaceNode"<<std::endl;
	TSurfaceNode* receiverSurfaceNode = new TSurfaceNode();
	std::cout<<"\treceiverSurfaceNode: "<<receiverSurfaceNode->GetName()<<std::endl;
	receiverSurfaceNode->SetName( "ReceiverSurface" );
	( receiverNode->GetPart("childrenList" )->as< TNodesList >() )->InsertItem( receiverSurfaceNode );



	int receiverShapeIndex =distance( shapeFactoryName.begin(), find(shapeFactoryName.begin(), shapeFactoryName.end(), "Flat_disk" ) );
	TShape* receiverShape = shapeFactoryList[receiverShapeIndex]->CreateTShape();
	std::cout<<"\treceiverShape: "<<receiverShape->GetName()<<std::endl;
	receiverShape->SetName( "ReceiverGeometry");
	receiverShape->SetParameterValue<double>( "radius",  0.30 );
	receiverSurfaceNode->SetPart( "shape",  receiverShape );

	if( materialFactoryList.count() > 0 )
	{
		TMaterial* receiverMaterial = materialFactoryList[concentratorMaterialIndex]->CreateTMaterial();
		receiverMaterial->SetName( "ReceiverMaterial");
		std::cout<<"\treceiverMaterial: "<<receiverMaterial->GetName()<<std::endl;
		receiverMaterial->SetParameterValue<double>( "reflectivity", 0.0 );
		receiverSurfaceNode->SetPart( "material",  receiverMaterial );
	}

	//Concentrator 2
	std::cout<<"Concentrator 2"<<std::endl;
	TGroupNode* concentrator2Node = new TGroupNode();
	concentrator2Node->SetName( "Concentrator2");
	concentrator2Node->SetParameterValue<std::string>( "translation",  "5 0 0" );
	if( !( rootNode->GetPart( "childrenList" )->as< TNodesList >() )->InsertItem( concentrator2Node ) )
	{
		std::cout<<"Error adding concentrator 2 to root node"<<std::endl;
		return -1;
	}

	std::cout<<"\tConcentrator2 tracker"<<std::endl;
	TTrackerNode* c2ParabolicDishTracker = trackerFactoryList[trackerFactoryIndex]->CreateTTrackerNode();
	c2ParabolicDishTracker->SetName( "Tracker_Concentrator_2" );
	if( !( concentrator2Node->GetPart( "childrenList" )->as< TNodesList >() )->InsertItem( c2ParabolicDishTracker ) )
	{
		std::cout<<"Error adding tracker node to concentrator 2"<<std::endl;
		return ( -1 );
	}

	if( !( c2ParabolicDishTracker->GetPart( "childrenList" )->as< TNodesList >() )->InsertItem( concentratorSurfaceNode ) )
	{
		std::cout<<"Error adding surface node to concentrator 2 tracker"<<std::endl;
		return -1;
	}


	std::cout<<"TNodesDocument saveDocument"<<std::endl;
	TNodesDocument saveDocument;
	saveDocument.SetRootNode( tonatiuhScene );
	if( !saveDocument.Write( argv[1] )  )
	{
		std::cout<<"Error during writing file"<<std::endl;
		return -1;
	}

	std::cout<<"OK"<<std::endl;
	//Codigo para si se eliminan los nodos
	std::cout<<"Removing scene node"<<std::endl;
	tonatiuhScene->RemoveReference();
	std::cout<<"Removed scene node"<<std::endl;
	tonatiuhScene = 0;


	std::cout<<"Tonatiuh 3.0 readDocument"<<std::endl;

	TNodesDocument readDocument;
	bool okRead = readDocument.Read( argv[1] );
	if( !okRead  )
	{
		std::cout<<"Error reading file"<<std::endl;
		return -1;
	}


	std::cout<<"Tonatiuh 3.0 sceneNode"<<std::endl;
	TSceneNode* sceneNode = readDocument.GetRootNode()->as<TSceneNode>();
	if( !sceneNode || sceneNode == 0 )
	{
		std::cout<<"Error reading file"<<std::endl;
		return -2;
	}

	/*
	TNodesDocument saveDocument2;
	saveDocument2.SetRootNode( sceneNode );
	if( !saveDocument2.Write( argv[1] )  )
	{
		std::cout<<"Error during writing file"<<std::endl;
		return -1;
	}
	*/


	/*

	//Reference count
	//std::cout<<"sceneNode "<<sceneNode->GetName().toStdString()<<std::endl;
	//ChildrenBranch(sceneNode);
	 *
	 */


	/*
	//Codigo para verificar el orden de para aplicar las transformaciones
	Transform translation = Translate( 0,200,0 );
	Transform rotation = Rotate( 15 * gc::Degree, Vector3D( 1,0,0) );
	Transform scale = Scale( 0.5, 1, 1);

	Transform transformOTW = translation * rotation * scale;
	std::cout<<"p: "<<transformOTW(Point3D( 0, 0, 0 ) )<<std::endl;
	*/

	/*

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



	//Create the random generator
	RandomDeviate* pRand = 0;
	if( !pRand && randomDeviateFactoryList.count() < 1 )
		return ( -3 );
	pRand =  randomDeviateFactoryList[0]->CreateRandomDeviate();

	std::cout<<"pRand"<<std::endl;

	TPhotonMap* pPhotonMap = new TPhotonMap;
	std::cout<<"pPhotonMap"<<std::endl;

	QDateTime startTime = QDateTime::currentDateTime();


	RayCasting raytracer;
	std::cout<<"raytracer"<<std::endl;
	if( !raytracer.SetScene( sceneNode ) )
	{

		std::cout<<"ERROR in the scene"<<std::endl;
		return ( -4 );
	}

	raytracer.SetRandomNumberGenerator( pRand );
	raytracer.SetPhotonMap( pPhotonMap );
	raytracer.Run( 10000000 );

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;

	std::cout<<"END main"<<std::endl;



	QFile simulationsFile( argv[2] );
	if( !simulationsFile.open( QIODevice::WriteOnly ) )
	{
		std::cout<<"Cannot open file to save results: '"<< argv[2]<<std::endl;
		return ( -4 );
	}
	QTextStream out( &simulationsFile );

	std::vector< Photon* > photonsList = pPhotonMap->GetAllPhotons();
	for( unsigned int p = 0; p < photonsList.size(); p++ )
	{
		out<<photonsList[p]->posWorld.x<<"\t";
		out<<photonsList[p]->posWorld.y<<"\t";
		out<<photonsList[p]->posWorld.z<<"\n";

		//if( p < 50 )
		//	std::cout<<photonsList[p]->intersectedSurfaceURL.toStdString()  <<std::endl;
	}
	simulationsFile.close();


	//Codigo para si se eliminan los nodos
	std::cout<<"Removing scene node"<<std::endl;
	sceneNode->RemoveReference();
	std::cout<<"Removed scene node"<<std::endl;
	sceneNode = 0;

	*/

	//Change sun position
	std::cout<<"Change sun position:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( "light" )->as<TSunNode>();
	std::cout<<"\tCurrent values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;

	return ( 0 );
}



