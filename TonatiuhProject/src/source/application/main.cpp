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

#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVariant>

#include "gc.h"
#include "PluginManager.h"
#include "RandomDeviateFactory.h"
#include "RayCasting.h"
#include "TCuboid.h"
#include "TGroupNode.h"
#include "TGroupNode.h"
#include "TMaterial.h"
#include "TMaterialFactory.h"
#include "TNodesDatabase.h"
#include "TNodesDocument.h"
#include "TParameterList.h"
#include "TPhotonMap.h"
#include "TSceneNode.h"
#include "TShapeFactory.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TSunNode.h"
#include "TSurfaceNode.h"
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

/*
void ChildrenBranch( TNode* branch )
{
	std::cout<<"ChildrenBranch "<<std::endl;

	if( !branch  ) return;
	std::cout<<"\t"<<branch->GetName().toStdString()<<" "<<branch->GetReferences()<<std::endl;

	if( const TNodesList* listNode = branch->as<TNodesList>() )
	{
		int numberOfChildren = listNode->Count();
		for( int l = 0; l < numberOfChildren; l++ )
		{
			if( listNode->Item( l ) && listNode->Item( l ) != 0 )
				ChildrenBranch( listNode->Item( l ) );
		}
	}


	if( const TContainerNode* containerNode = branch->as<TContainerNode>() )
	{
		int numberOfParts = containerNode->NumberOfParts();
		QStringList partNamesList = containerNode->GetPartNames();
		for( int p = 0; p < numberOfParts; p++ )	ChildrenBranch( containerNode->GetPart( partNamesList[p] )  );
	}

}
*/

int main ( int argc, char** argv )
{
	qInstallMessageHandler(myMessageOutput);

    QApplication a( argc, argv );
	a.setApplicationVersion( APP_VERSION );

	std::cout<<"Tonatiuh 3.0"<<std::endl;
	TNodesDatabase::Init();


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
	QVector< TShapeFactory* > shapeFactoryList =  pluginManager.GetShapeFactories();
	QVector< TSunshapeFactory* > sunshapeFactoryList = pluginManager.GetSunshapeFactories();
	QVector< TTrackerFactory* > trackerFactoryList = pluginManager.GetTrackerFactories();
	QVector< RandomDeviateFactory* > randomDeviateFactoryList = pluginManager.GetRandomDeviateFactories();


	/*
	TSceneNode* tonatiuhScene = new TSceneNode;
	std::cout<<"tonatiuhScene"<<std::endl;

	//Sun
	double azimuthRad = 180 * gc::Degree;
	TSunNode* sunNode = new TSunNode();
	tonatiuhScene->SetPart( QLatin1String( "light" ),  sunNode );
	sunNode->SetParameterValue( QString( "azimuth" ),  QString( "%1" ).arg( QString::number(azimuthRad) ) );

	TSunshape* sunshapeNode = sunshapeFactoryList[0]->CreateTSunshape();
	sunshapeNode->SetName( "sunshapeModel");
	sunNode->SetPart( QLatin1String( "sunshape" ),  sunshapeNode );


	//Parabolic dish
	std::cout<<"rootNode"<<std::endl;
	TGroupNode* rootNode = new TGroupNode();
	rootNode->SetName( "Eurodish_field");
	tonatiuhScene->SetPart( QLatin1String( "childrenList" ),  rootNode );
	std::cout<<"rootNode: "<<rootNode->GetName().toStdString()<<std::endl;


	//Concentrator 1
	TGroupNode* concentrator1Node = new TGroupNode();
	concentrator1Node->SetName( "Concentrator_1");
	concentrator1Node->SetParameterValue( QString( "translation" ),  "-5 0 0" );
	std::cout<<"concentratorNode: "<<concentrator1Node->GetName().toStdString()<<std::endl;
	//double rotationAngle = 0.5 * gc::Pi;
	//concentrator1Node->SetParameterValue( QString( "rotation" ),  QString( "0 0 -1 %1" ).arg( QString::number(rotationAngle) ) );
	if( !qobject_cast< TNodesList* >  ( rootNode->GetPart("childrenList" ))->InsertItem( concentrator1Node ) )
	{
		std::cout<<"Error adding concentrator 1 to root node"<<std::endl;
		return -1;
	}


	TTrackerNode* c1ParabolicDishTracker = trackerFactoryList[0]->CreateTTrackerNode();
	c1ParabolicDishTracker->SetName( "Tracker_Concentrator_1");
	qobject_cast< TNodesList* >  ( concentrator1Node->GetPart("childrenList" ))->InsertItem( c1ParabolicDishTracker );


	std::cout<<"TSurfaceNode"<<std::endl;
	TSurfaceNode* concentratorSurfaceNode = new TSurfaceNode();
	concentratorSurfaceNode->SetName( "ConcentratorSurface");
	qobject_cast< TNodesList* >  ( c1ParabolicDishTracker->GetPart("childrenList" ))->InsertItem( concentratorSurfaceNode );


	TShape* concentratorShape = shapeFactoryList[2]->CreateTShape();
	concentratorShape->SetName( "ConcentratorGeometry");
	concentratorShape->SetParameterValue( QString( "focusLength" ), 4.520 );
	concentratorShape->SetParameterValue( QString( "dishMinRadius" ),  0.0 );
	concentratorShape->SetParameterValue( QString( "dishMaxRadius" ),  4.25 );
	concentratorSurfaceNode->SetPart( QLatin1String( "shape" ),  concentratorShape );


	if( materialFactoryList.count() > 0 )
	{
		TMaterial* concentratorMaterial = materialFactoryList[0]->CreateTMaterial();
		concentratorMaterial->SetParameterValue( QLatin1String("reflectivity"), 0.925 );
		concentratorMaterial->SetParameterValue( QLatin1String("sigmaSlope"), 4.4270 );
		concentratorMaterial->SetParameterValue( QLatin1String("distribution"), QLatin1String( "PILLBOX" ) );
		concentratorSurfaceNode->SetPart( QLatin1String( "material" ),  concentratorMaterial );
	}

	//Receiver
	std::cout<<"Receiver"<<std::endl;
	TGroupNode* receiverNode = new TGroupNode();
	//receiverNode->SetParent( rootNode );
	std::cout<<"\treceiverNode: "<<receiverNode->GetName().toStdString()<<std::endl;
	receiverNode->SetName( "Receiver ");

	QString translationValue = receiverNode->GetParameterValue( QString( "translation" ) ).toString();
	receiverNode->SetParameterValue( QString( "translation" ),  "0 4.52 0" );
	//receiverNode->SetParameterValue( QString( "rotation" ),  QString( "0 0 -1 %1" ).arg( QString::number(rotationAngle ) ) );
	translationValue = receiverNode->GetParameterValue( QString( "translation" ) ).toString();
	qobject_cast< TNodesList* >  ( c1ParabolicDishTracker->GetPart("childrenList" ))->InsertItem( receiverNode );

	std::cout<<"TSurfaceNode"<<std::endl;
	TSurfaceNode* receiverSurfaceNode = new TSurfaceNode();
	std::cout<<"\treceiverSurfaceNode: "<<receiverSurfaceNode->GetName().toStdString()<<std::endl;
	receiverSurfaceNode->SetName( "ReceiverSurface");
	qobject_cast< TNodesList* >  ( receiverNode->GetPart("childrenList" ))->InsertItem( receiverSurfaceNode );


	TShape* receiverShape = shapeFactoryList[0]->CreateTShape();
	std::cout<<"\treceiverShape: "<<receiverShape->GetName().toStdString()<<std::endl;
	receiverShape->SetName( "ReceiverGeometry");
	receiverShape->SetParameterValue( QString( "radius" ),  0.30 );
	receiverSurfaceNode->SetPart( QLatin1String( "shape" ),  receiverShape );
	std::cout<<"\treceiverShape: "<<receiverShape->GetName().toStdString()<<std::endl;

	if( materialFactoryList.count() > 0 )
	{
		TMaterial* receiverMaterial = materialFactoryList[0]->CreateTMaterial();
		receiverMaterial->SetParameterValue( QLatin1String("reflectivity"), 0.0 );
		receiverSurfaceNode->SetPart( QLatin1String( "material" ),  receiverMaterial );
	}

	//Concentrator 2
	TGroupNode* concentrator2Node = new TGroupNode();
	concentrator2Node->SetName( "Concentrator2");
	concentrator2Node->SetParameterValue( QString( "translation" ),  QString( "5 0 0" ) );
	if( !qobject_cast< TNodesList* >  ( rootNode->GetPart("childrenList" ) )->InsertItem( concentrator2Node ) )
	{
		std::cout<<"Error adding concentrator 2 to root node"<<std::endl;
		return -1;
	}


	TTrackerNode* c2ParabolicDishTracker = trackerFactoryList[0]->CreateTTrackerNode();
	c2ParabolicDishTracker->SetName( "Tracker_Concentrator_2");
	qobject_cast< TNodesList* >  ( concentrator2Node->GetPart("childrenList" ))->InsertItem( c2ParabolicDishTracker );

	if( !qobject_cast< TNodesList* >  ( c2ParabolicDishTracker->GetPart("childrenList" ))->InsertItem( concentratorSurfaceNode ))
	{
		std::cout<<"Error adding tracker node to concentrator 2"<<std::endl;
		return -1;
	}


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
	*/

	TNodesDocument readDocument;
	bool okRead = readDocument.Read( argv[1] );
	if( !okRead  )
	{
		std::cout<<"Error reading file"<<std::endl;
		return -1;
	}



	TSceneNode* sceneNode = readDocument.GetRootNode()->as<TSceneNode>();
	if( !sceneNode || sceneNode == 0 )
	{
		std::cout<<"Error reading file"<<std::endl;
		return -2;
	}


	//Reference count
	//std::cout<<"sceneNode "<<sceneNode->GetName().toStdString()<<std::endl;
	//ChildrenBranch(sceneNode);


	/*
	//Codigo para verificar el orden de para aplicar las transformaciones
	Transform translation = Translate( 0,200,0 );
	Transform rotation = Rotate( 15 * gc::Degree, Vector3D( 1,0,0) );
	Transform scale = Scale( 0.5, 1, 1);

	Transform transformOTW = translation * rotation * scale;
	std::cout<<"p: "<<transformOTW(Point3D( 0, 0, 0 ) )<<std::endl;
	*/



	//Change sun position
	std::cout<<"Change sun position:"<<std::endl;
	TSunNode* sun = sceneNode->GetPart( QLatin1String( "light" ) )->as<TSunNode>();
	std::cout<<"\tCurrent values:"<<std::endl;
	std::cout<<"\t\t azimuth: "<<sun->GetAzimuth()<<std::endl;
	std::cout<<"\t\t zenith: "<<sun->GetZenith()<<std::endl;


	std::cout<<"\tChanging sun position..."<<std::endl;
	sun->ChangeSunPosition( 180*gc::Degree, 37.5*gc::Degree );
	//sun->ChangeSunPosition( 180*gc::Degree, 0.0*gc::Degree );


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

	RayCasting raytracer;
	std::cout<<"raytracer"<<std::endl;
	if( !raytracer.SetScene( sceneNode ) )
	{

		std::cout<<"ERROR in the scene"<<std::endl;
		return ( -4 );
	}

	std::cout<<"raytracer raytracer.SetRandomNumberGenerator( pRand )"<<std::endl;
	raytracer.SetRandomNumberGenerator( pRand );
	raytracer.SetPhotonMap( pPhotonMap );
	raytracer.Run( 10000000 );

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
		out<<photonsList[p]->pos.x<<"\t";
		out<<photonsList[p]->pos.y<<"\t";
		out<<photonsList[p]->pos.z<<"\n";
	}
	simulationsFile.close();


	//Codigo para si se eliminan los nodos
	std::cout<<"Removing scene node"<<std::endl;
	sceneNode->RemoveReference();
	std::cout<<"Removed scene node"<<std::endl;
	sceneNode = 0;

	return ( 0 );
}



