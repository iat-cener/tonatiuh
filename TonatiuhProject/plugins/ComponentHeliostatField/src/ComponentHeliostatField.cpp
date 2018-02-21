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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <algorithm>

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "Point3D.h"
#include "NormalVector.h"

#include "PluginManager.h"
#include "TMaterial.h"
#include "TMaterialFactory.h"
#include "TSceneKit.h"
#include "TSeparatorKit.h"
#include "TShape.h"
#include "TShapeFactory.h"
#include "TShapeKit.h"
#include "TTracker.h"
#include "TTrackerFactory.h"

#include "ComponentHeliostatField.h"
#include "HeliostatFieldWizard.h"


bool comparePuntosPorX( std::pair< Point3D, Point3D > pA, std::pair< Point3D, Point3D > pB )
{
	return ( pA.first.x < pB.first.x );
}

bool comparePuntosPorZ( std::pair< Point3D, Point3D > pA, std::pair< Point3D, Point3D > pB )
{
	return ( pA.first.z < pB.first.z );
}


ComponentHeliostatField::ComponentHeliostatField( PluginManager* pPluginManager )
:m_pPluginManager( pPluginManager )
{

}

ComponentHeliostatField::~ComponentHeliostatField()
{

}

TSeparatorKit* ComponentHeliostatField::CreateField()
{
	//Define heliostat tracker for the field
	QVector< TTrackerFactory* > trackersFactoryList = m_pPluginManager->GetTrackerFactories();
	if( trackersFactoryList.size() == 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "Tracker plugins not found." ) );
		return 0;
	}

	QVector< QString > trackerNames;
	for( int i = 0; i < trackersFactoryList.size(); i++ )
		trackerNames<< trackersFactoryList[i]->TTrackerName();

	int selectedTracker = trackerNames.indexOf( QLatin1String( "Heliostat_tracker" ) );
	if( selectedTracker < 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "'Heliostat_tracker' not found." ) );
		return 0;
	}

	TTrackerFactory* heliostatTrackerFactory = trackersFactoryList[ selectedTracker ];

	//Define heliostat shape for the field
	QVector< TShapeFactory* > shapeFactoryList = m_pPluginManager->GetShapeFactories();
	if( shapeFactoryList.size() == 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "Shape plugins not found." ) );
		return 0;
	}

	QVector< QString > shapeNames;
	for( int i = 0; i < shapeFactoryList.size(); i++ )
		shapeNames<< shapeFactoryList[i]->TShapeName();

	//Define heliostat material for the field
	QVector< TMaterialFactory* > materialFactoryList = m_pPluginManager->GetMaterialFactories();
	if( materialFactoryList.size() == 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "Material plugins not found." ) );
		return 0;
	}

	QVector< QString > materialNames;
	for( int i = 0; i < materialFactoryList.size(); i++ )
		materialNames<< materialFactoryList[i]->TMaterialName();

	int selectedMaterial = materialNames.indexOf( QLatin1String( "Specular_Standard_Material" ) );
	if( selectedMaterial < 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "'Specular_Standard_Material' plugin not found." ) );
		return 0;
	}

	TMaterialFactory* materialFactory = materialFactoryList[ selectedMaterial ];

	HeliostatFieldWizard wizard;
	if( !wizard.exec() )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "The field has not been created." ) );
		return 0;
	}

	//COORDINATES
	int helCoord = wizard.GetHelCoordinates(); //1:file; 2:table

	std::vector< Point3D > hCenterList;
	if( helCoord == 1 )
	{
		QFile coordinatesFile( wizard.GetCoordinatesFile() );
		if( !coordinatesFile.open( QIODevice::ReadOnly ) )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Impossible to read the heliostat coordinates file." ) );
			return 0;
		}

		QTextStream coordIn( &coordinatesFile );

		while( !coordIn.atEnd() )
		{
			QString inLine = coordIn.readLine();
			QStringList heliostatL = inLine.split(QRegExp("[\\t,;]"), QString::SkipEmptyParts);
			if( heliostatL.count() >=3 )
				hCenterList.push_back( Point3D( heliostatL[0].toDouble(),
						heliostatL[1].toDouble(),
						heliostatL[2].toDouble() ) );
		}
	}
	else if( helCoord == 2 )
	{
		hCenterList = wizard.GetHeliostatCoordsTable();
	}

	//HELIOSTAT
	int heliostat = wizard.GetHeliostat(); //1:component; 2:new design

	TSeparatorKit* heliostatComponent;
	if( heliostat == 1)
	{
		QString heliostatComponentFileName = wizard.GetComponentFile();
		heliostatComponent = OpenHeliostatComponent( heliostatComponentFileName );
		if( !heliostatComponent )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Impossible to read the heliostat component." ) );
			return 0;
		}
	}

	QString heliostatShape = wizard.GetHeliostatShape();

	int selectedShape = shapeNames.indexOf( heliostatShape.toLatin1() );
	if( selectedShape < 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "Specified plugin not found." ) );
		return 0;
	}

	TShapeFactory* shapeFactory = shapeFactoryList[ selectedShape ];

	double heliostatWidth = wizard.GetHeliostatWidth();
	double heliostatHeight = wizard.GetHeliostatHeight();
	double heliostatRadius = wizard.GetHeliostatRadius(); //-1 for slant

	//Material
	TMaterial* materialNode = materialFactory->CreateTMaterial();
	double reflectivity = wizard.GetHeliostatReflectivity();
	double sigma = wizard.GetHeliostatSigma();

	//Aiming point
	int strategy = wizard.GetAimingStrategy(); //1: one-dimensional; 2: point; 3:table; 4:file
	double rElevation = wizard.GetReceiverElevation();
	double rRadius = wizard.GetReceiverRadius();
	Point3D aimingPoint = wizard.GetAimingStrategyPoint(); //(-1,-1,-1) for strategy != 2

	std::vector< Point3D > aimingPointList;
	if( strategy == 1 )
	{
		for( unsigned int i = 0; i < hCenterList.size(); i++ )
		{
			Vector3D hCenterNorm( hCenterList[i].x , 0 , hCenterList[i].z );
			aimingPointList.push_back( Point3D( Normalize(hCenterNorm).x * rRadius, rElevation, Normalize(hCenterNorm).z * rRadius ) );
		}
	}
	else if( strategy == 2 )
	{
		for( unsigned int i = 0; i < hCenterList.size(); i++ )
			aimingPointList.push_back( aimingPoint );
	}
	else if( strategy == 3 )
	{
		aimingPointList = wizard.GetAimingPointsTable();
	}
	else if( strategy == 4 )
	{
		QFile aimingPointFile( wizard.GetStrategyFile() );
		if( !aimingPointFile.open( QIODevice::ReadOnly ) )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Impossible to read the aiming points file." ) );
			return 0;
		}

		QTextStream aimingPointIn( &aimingPointFile );

		while( !aimingPointIn.atEnd() )
		{
			QString inLine = aimingPointIn.readLine();
			QStringList aimingPointL = inLine.split(QRegExp("[\\t,;]"), QString::SkipEmptyParts);
			if( aimingPointL.count() >=3 )
				aimingPointList.push_back( Point3D( aimingPointL[0].toDouble(),
						aimingPointL[1].toDouble(),
						aimingPointL[2].toDouble() ) );
		}

		if( aimingPointList.size() != hCenterList.size() )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Different file sizes: heliostat coordinates and aiming points." ) );
			return 0;
		}
	}

	if( aimingPointList.size() != hCenterList.size() )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "Different file sizes: heliostat coordinates and aiming points." ) );
		return 0;
	}

	//SoField* hReflectivityField = materialNode->getField( "m_reflectivity" );
	trt::TONATIUH_REAL* hReflectivityField = static_cast< trt::TONATIUH_REAL* > ( materialNode->getField( "m_reflectivity" ) );
	hReflectivityField->setValue(  reflectivity );

	trt::TONATIUH_REAL* hSigmaField = static_cast< trt::TONATIUH_REAL* > ( materialNode->getField( "m_sigmaSlope" ) );
	hSigmaField->setValue( sigma );

	SoSFEnum* distributionField = static_cast< SoSFEnum* > ( materialNode->getField( "m_distribution" ) );
	distributionField->setValue( "NORMAL" );


	SoType separatorType = SoType::fromName( SbName ( "TSeparatorKit" ) );

	TSeparatorKit* heliostatsNodeSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
	heliostatsNodeSeparator->setName( "Heliostatos" );
	heliostatsNodeSeparator->ref();

	CreateHeliostatZones( hCenterList, heliostatsNodeSeparator, heliostatTrackerFactory, shapeFactory, heliostat, heliostatComponent, heliostatWidth, heliostatHeight, heliostatRadius,
			materialNode, aimingPointList, 1 );

	return heliostatsNodeSeparator;

}


TSeparatorKit* ComponentHeliostatField::CreateField(QVector< QVariant >  argumentList)
{
	//Heliostat coordinates
	if( argumentList.count() != 15 )	return 0;

	QFile coordinatesFile( argumentList[0].toString() );
	if( !coordinatesFile.open( QIODevice::ReadOnly ) )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "Impossible to read the heliostat coordinates file." ) );
			return 0;
	}

	QTextStream coordIn( &coordinatesFile );

	std::vector< Point3D > hCenterList;
	while( !coordIn.atEnd() )
	{
		QString inLine = coordIn.readLine();
		QStringList heliostat = inLine.split(QRegExp("[\\t,;]"), QString::SkipEmptyParts);
		if( heliostat.count() >=3 )
			hCenterList.push_back( Point3D( heliostat[0].toDouble(),
					heliostat[1].toDouble(),
					heliostat[2].toDouble() ) );
	}


	//Heliostat

	int heliostat; //1:component; 2:new design
	TSeparatorKit* heliostatComponentNode;
	TShapeFactory* shapeFactory;
	QString heliostatShape;
	double heliostatWidth = 1.0;
	double heliostatHeight = 1.0;
	double heliostatRadius = 0.75;
	double reflectivity = 0.0;
	double sigma;

	if( !argumentList[1].toString().isEmpty() )
	{
		heliostat = 1;
		heliostatComponentNode = OpenHeliostatComponent( argumentList[1].toString() );
		if( !heliostatComponentNode )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Impossible to read the heliostat component." ) );
			return 0;
		}
	}
	else
	{
		heliostat = 2;

		heliostatShape = argumentList[2].toString( );
		QVector< TShapeFactory* > shapeFactoryList = m_pPluginManager->GetShapeFactories();
		if( shapeFactoryList.size() == 0 )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Shape plugins not found." ) );
			return 0;
		}

		QVector< QString > shapeNames;
		for( int i = 0; i < shapeFactoryList.size(); i++ )
			shapeNames<< shapeFactoryList[i]->TShapeName();

		int selectedShape = shapeNames.indexOf( heliostatShape.toLatin1() );
		if( selectedShape < 0 )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Specified shape not found." ) );
			return 0;
		}

		shapeFactory = shapeFactoryList[ selectedShape ];

		bool okWidth = false;
		heliostatWidth = argumentList[3].toDouble( &okWidth );

		bool okHeight = false;
		heliostatHeight = argumentList[4].toDouble( &okHeight );

		bool okReflectivity = false;
		reflectivity = argumentList[6].toDouble( &okReflectivity );

		bool okSigma = false;
		sigma = argumentList[7].toDouble( &okSigma );

		if( heliostatShape == QString( "Spherical_rectangle" ) )
		{
			bool okRadius = false;
			heliostatRadius = argumentList[5].toDouble( &okRadius );
			if( !okWidth || !okHeight || !okRadius || !okReflectivity || !okSigma )
			{
		        QMessageBox::warning( 0, QString( "Heliostat Field Layout" ),
		        		QString( "The heliostat design is not correctly defined:\n"
								"\t- Width\n"
								"\t- Height\n"
		        				"\t- Radius\n"
		        				"\t- Reflectivity\n"
		        				"\t- Sigma Slope") );

				return 0;
			}
		}
		else if( !okWidth || !okHeight || !okReflectivity || !okSigma )
		{
	        QMessageBox::warning( 0, QString( "Heliostat Field Layout" ),
	        		QString( "The heliostat design is not correctly defined:\n"
							"\t- Width\n"
							"\t- Height\n"
	        				"\t- Radius\n"
	        				"\t- Reflectivity\n"
	        				"\t- Sigma Slope") );

			return 0;
		}

	}


	//Aiming strategy (1-File, 2-One-Dimensional, 3-Point)
	bool okAimingStrategy = false;
	int strategy= argumentList[8].toInt( &okAimingStrategy );

	std::vector< Point3D > aimingPointList;
	if( strategy == 1 )
	{
		QFile aimingPointFile( argumentList[9].toString() );
		if( !aimingPointFile.open( QIODevice::ReadOnly ) )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Impossible to read the aiming points file." ) );
			return 0;
		}

		QTextStream aimingPointIn( &aimingPointFile );

		while( !aimingPointIn.atEnd() )
		{
			QString inLine = aimingPointIn.readLine();
			QStringList aimingPointL = inLine.split(QRegExp("[\\t,;]"), QString::SkipEmptyParts);
			if( aimingPointL.count() >=3 )
				aimingPointList.push_back( Point3D( aimingPointL[0].toDouble(),
						aimingPointL[1].toDouble(),
						aimingPointL[2].toDouble() ) );
		}

		if( aimingPointList.size() != hCenterList.size() )
		{
			QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
					QString( "Different file sizes: heliostat coordinates and aiming points." ) );
			return 0;
		}
	}
	else if( strategy == 2 )
	{
		bool okRElevation= false;
		double rElevation = argumentList[10].toDouble( &okRElevation );

		bool okRRadius= false;
		double rRadius = argumentList[11].toDouble( &okRRadius );

		if( !okRElevation || !okRRadius )
		{
	        QMessageBox::warning( 0, QString( "Heliostat Field Layout" ),
	        		QString( "The One-Dimensional strategy is not correctly defined:\n"
							"\t- Receiver elevation\n"
							"\t- Receiver radius") );

			return 0;
		}

		for( unsigned int i = 0; i < hCenterList.size(); i++ )
		{
			Vector3D hCenterNorm( hCenterList[i].x , 0 , hCenterList[i].z );
			aimingPointList.push_back( Point3D( Normalize(hCenterNorm).x * rRadius, rElevation, Normalize(hCenterNorm).z * rRadius ) );
		}
	}
	else if( strategy == 3 )
	{
		bool okAimingCoord = false;
		double xCoord = argumentList[12].toDouble( &okAimingCoord );
		double yCoord = argumentList[13].toDouble( &okAimingCoord );
		double zCoord = argumentList[14].toDouble( &okAimingCoord );
		if( !okAimingCoord )
		{
	        QMessageBox::warning( 0, QString( "Heliostat Field Layout" ),
	        		QString( "The aiming point strategy is not correctly defined:\n"
							"\t- x\n"
							"\t- y\n"
	        				"\t- z") );

			return 0;
		}
		Point3D aimingPoint( xCoord, yCoord, zCoord );

		for( unsigned int i = 0; i < hCenterList.size(); i++ )
			aimingPointList.push_back( aimingPoint );
	}
	else
	{
		return 0;
	}

	//Define heliostat tracker for the field
	QVector< TTrackerFactory* > trackersFactoryList = m_pPluginManager->GetTrackerFactories();
	if( trackersFactoryList.size() == 0 )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "Tracker plugins not found." ) );
		return 0;
	}

	QVector< QString > trackerNames;
	for( int i = 0; i < trackersFactoryList.size(); i++ )
		trackerNames<< trackersFactoryList[i]->TTrackerName();

	int selectedTracker = trackerNames.indexOf( QLatin1String( "Heliostat_tracker" ) );
	if( selectedTracker < 0 )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "'Heliostat_tracker' plugin not found." ) );
		return 0;
	}

	TTrackerFactory* heliostatTrackerFactory = trackersFactoryList[ selectedTracker ];

	//Define heliostat material for the field
	QVector< TMaterialFactory* > materialFactoryList = m_pPluginManager->GetMaterialFactories();
	if( materialFactoryList.size() == 0 )
	{	        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
			QString( "Material plugins not found." ) );
		return 0;
	}

	QVector< QString > materialNames;
	for( int i = 0; i < materialFactoryList.size(); i++ )
		materialNames<< materialFactoryList[i]->TMaterialName();

	int selectedMaterial = materialNames.indexOf( QLatin1String( "Specular_Standard_Material" ) );
	if( selectedMaterial < 0 )
	{
		QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
				QString( "'Specular_Standard_Material' plugin not found. " ) );
		return 0;
	}

	TMaterialFactory* materialFactory = materialFactoryList[ selectedMaterial ];

	//Material
	TMaterial* materialNode = materialFactory->CreateTMaterial();

	if( heliostat == 2 )
	{
		trt::TONATIUH_REAL* hReflectivityField = static_cast< trt::TONATIUH_REAL* > ( materialNode->getField( "m_reflectivity" ) );
		hReflectivityField->setValue( reflectivity );

		trt::TONATIUH_REAL* hSigmaField = static_cast< trt::TONATIUH_REAL* > ( materialNode->getField( "m_sigmaSlope" ) );
		hSigmaField->setValue( sigma );

		SoSFEnum* distributionField = static_cast< SoSFEnum* > ( materialNode->getField( "m_distribution" ) );
		distributionField->setValue(  "NORMAL" );
	}

	SoType separatorType = SoType::fromName( SbName ( "TSeparatorKit" ) );

	TSeparatorKit* heliostatsNodeSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
	heliostatsNodeSeparator->setName( "Heliostats" );
	heliostatsNodeSeparator->ref();

	CreateHeliostatZones( hCenterList, heliostatsNodeSeparator, heliostatTrackerFactory, shapeFactory, heliostat, heliostatComponentNode, heliostatWidth, heliostatHeight, heliostatRadius,
			materialNode, aimingPointList, 1 );

	return heliostatsNodeSeparator;
}


void ComponentHeliostatField::CreateHeliostatZones( std::vector< Point3D >  heliostatCenterList, TSeparatorKit* parentNode,
		TTrackerFactory* heliostatTrackerFactory,
		TShapeFactory* heliostatShapeFactory,
		int heliostat,
		TSeparatorKit* heliostatComponent,
		double heliostatWidth,
		double heliostatHeight,
		double heliostatRadius,
		TMaterial* materialNode,
		std::vector< Point3D > aimingPointList,
		int eje )
{
	SoType separatorType = SoType::fromName( SbName ( "TSeparatorKit" ) );
	SoType shapeKitType = SoType::fromName( SbName ( "TShapeKit" ) );

	SoNodeKitListPart* heliostatsNodePartList = static_cast< SoNodeKitListPart* >( parentNode->getPart( "childList", true ) );
	if( !heliostatsNodePartList ) return;

	int nHeliostatCenters = ( int ) heliostatCenterList.size();
	if( nHeliostatCenters < 8 )
	{

		for( int nHeliostat = 0; nHeliostat < nHeliostatCenters; nHeliostat++ )
		{
			TSeparatorKit* heliostatSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
			heliostatsNodePartList->addChild(heliostatSeparator);
			QString heliostatName = QString( QLatin1String( "Heliostat%1" ) ).arg( QString::number( nHeliostat ) );
			heliostatSeparator->setName( heliostatName.toStdString().c_str() );
			SoTransform* nodeTransform = dynamic_cast< SoTransform* >( heliostatSeparator->getPart( "transform", true ) );
			nodeTransform->translation.setValue( heliostatCenterList[nHeliostat].x,
					heliostatCenterList[nHeliostat].y,
					heliostatCenterList[nHeliostat].z );

			Point3D hCenter( heliostatCenterList[nHeliostat].x,
					heliostatCenterList[nHeliostat].y,
					heliostatCenterList[nHeliostat].z );

			SoNodeKitListPart*  heliostatPartList = static_cast< SoNodeKitListPart* >( heliostatSeparator->getPart( "childList", true ) );
			if( !heliostatPartList ) return;

			TSeparatorKit* heliostatTrackerNode = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
			heliostatPartList->addChild( heliostatTrackerNode );
			heliostatTrackerNode->setName( "HeliostatTrackerNode" );
			SoNodeKitListPart*  heliostaTrackerNodetPartList = static_cast< SoNodeKitListPart* >( heliostatTrackerNode->getPart( "childList", true ) );
			if( !heliostaTrackerNodetPartList ) return;

			TTracker* tracker = heliostatTrackerFactory->CreateTTracker();
			heliostatTrackerNode->setPart( "tracker", tracker );


			SoSFVec3f* aimingPointField = dynamic_cast< SoSFVec3f* > ( tracker->getField ( "aimingPoint" ) );

			if( aimingPointField )	aimingPointField->setValue( SbVec3f( aimingPointList[nHeliostat].x, aimingPointList[nHeliostat].y, aimingPointList[nHeliostat].z) );

			if( heliostat == 1 )
				heliostaTrackerNodetPartList->addChild( heliostatComponent );
			else if( heliostat == 2 )
			{
				TShapeKit* heliostatSurface = static_cast< TShapeKit* > ( shapeKitType.createInstance() );
				heliostaTrackerNodetPartList->addChild(heliostatSurface);

				TShape* shape = heliostatShapeFactory->CreateTShape();

				if( heliostatShapeFactory->TShapeName() == QString( "Spherical_rectangle" ) )
				{
					trt::TONATIUH_REAL* hRadiusField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "radius" ) );

					if( heliostatRadius < 0.0 )
						hRadiusField->setValue(  2* Distance( aimingPointList[nHeliostat], hCenter ) );
					else
						hRadiusField->setValue(  heliostatRadius );

					trt::TONATIUH_REAL* widthXField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "widthX" ) );
					widthXField->setValue(  heliostatWidth );

					trt::TONATIUH_REAL* widthZField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "widthZ" ) );
					widthZField->setValue(  heliostatHeight );
				}
				else if( heliostatShapeFactory->TShapeName() == QString( "Flat_Rectangle" ) )
				{
					trt::TONATIUH_REAL* widthXField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "width" ) );
					widthXField->setValue(  heliostatWidth );

					trt::TONATIUH_REAL* widthZField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "height" ) );
					widthZField->setValue(  heliostatHeight );
				}

				heliostatSurface->setPart("shape", shape);
				heliostatSurface->setPart("material", materialNode );
			}
		}
	}
	else
	{
		//TSeparatorKit* heliostatSeparator1 = new TSeparatorKit;
		TSeparatorKit* heliostatSeparator1 = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
		heliostatsNodePartList->addChild(heliostatSeparator1);
		QString heliostatName1 = QString( QLatin1String( "DivisionPor%1_1" ) ).arg( ( eje == 3 )? QString( 'Z' ): QString( 'X' ) );
		heliostatSeparator1->setName( heliostatName1.toStdString().c_str() );

		//TSeparatorKit* heliostatSeparator2 = new TSeparatorKit;
		TSeparatorKit* heliostatSeparator2 = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
		heliostatsNodePartList->addChild(heliostatSeparator2);
		QString heliostatName2 = QString( QLatin1String( "DivisionPor%1_2" ) ).arg( ( eje == 3 )? QString( 'Z' ): QString( 'X' ) );
		heliostatSeparator2->setName( heliostatName2.toStdString().c_str() );


		if( fabs( eje - 3 ) < 0.001 )
		{
			std::vector< std::pair < Point3D, Point3D > > coordsAndAimingPoint;
			for( unsigned int i=0; i < heliostatCenterList.size(); i++)
				coordsAndAimingPoint.push_back( std::make_pair( heliostatCenterList[i], aimingPointList[i] ) );

			std::sort( coordsAndAimingPoint.begin(), coordsAndAimingPoint.end(), comparePuntosPorZ );

			std::vector< Point3D >::iterator it;

			double zMin = coordsAndAimingPoint[0].first.z;
			double zMax = coordsAndAimingPoint[nHeliostatCenters-1].first.z;

			double splitPoint = zMin + ( 0.5 * ( zMax - zMin ) );
			std::vector< Point3D > hCenterListPart1;
			std::vector< Point3D > aimingPointListPart1;
			double position = 0;

			while( ( position < heliostatCenterList.size() )
					&& ( coordsAndAimingPoint[position].first.z < splitPoint ) )
			{
				hCenterListPart1.push_back( coordsAndAimingPoint[position].first );
				aimingPointListPart1.push_back( coordsAndAimingPoint[position].second );
				position++;
			}

			std::vector< Point3D > hCenterListPart2;
			std::vector< Point3D > aimingPointListPart2;
			while( position < heliostatCenterList.size() )
			{
				hCenterListPart2.push_back( coordsAndAimingPoint[position].first );
				aimingPointListPart2.push_back( coordsAndAimingPoint[position].second );
				position++;
			}
			CreateHeliostatZones( hCenterListPart1, heliostatSeparator1, heliostatTrackerFactory, heliostatShapeFactory, heliostat, heliostatComponent, heliostatWidth, heliostatHeight, heliostatRadius,
					materialNode, aimingPointListPart1, 1 );
			CreateHeliostatZones( hCenterListPart2, heliostatSeparator2, heliostatTrackerFactory, heliostatShapeFactory, heliostat, heliostatComponent, heliostatWidth, heliostatHeight, heliostatRadius,
					materialNode, aimingPointListPart2, 1 );
		}
		else
		{
			std::vector< std::pair < Point3D, Point3D > > coordsAndAimingPoint;
			for( unsigned int i=0; i < heliostatCenterList.size(); i++)
				coordsAndAimingPoint.push_back( std::make_pair( heliostatCenterList[i], aimingPointList[i] ) );

			std::sort( coordsAndAimingPoint.begin(), coordsAndAimingPoint.end(), comparePuntosPorX );

			double xMin = coordsAndAimingPoint[0].first.x;
			double xMax = coordsAndAimingPoint[nHeliostatCenters-1].first.x;

			double splitPoint = xMin + ( 0.5 * ( xMax - xMin ) );
			std::vector< Point3D > hCenterListPart1;
			std::vector< Point3D > aimingPointListPart1;
			double position = 0;
			while( ( position < heliostatCenterList.size() )
					&& ( coordsAndAimingPoint[position].first.x < splitPoint ) )
			{
				hCenterListPart1.push_back( coordsAndAimingPoint[position].first );
				aimingPointListPart1.push_back( coordsAndAimingPoint[position].second );
				position++;
			}


			std::vector< Point3D > hCenterListPart2;
			std::vector< Point3D > aimingPointListPart2;
			while( position < heliostatCenterList.size() )
			{
				hCenterListPart2.push_back( coordsAndAimingPoint[position].first );
				aimingPointListPart2.push_back( coordsAndAimingPoint[position].second );
				position++;
			}
			CreateHeliostatZones( hCenterListPart1, heliostatSeparator1, heliostatTrackerFactory, heliostatShapeFactory, heliostat, heliostatComponent, heliostatWidth, heliostatHeight, heliostatRadius,
					materialNode, aimingPointListPart1, 3 );
			CreateHeliostatZones( hCenterListPart2, heliostatSeparator2, heliostatTrackerFactory, heliostatShapeFactory, heliostat, heliostatComponent, heliostatWidth, heliostatHeight, heliostatRadius,
					materialNode, aimingPointListPart2, 3 );
		}
	}
}

TSeparatorKit* ComponentHeliostatField::OpenHeliostatComponent( QString fileName )
{
	if ( fileName.isEmpty() ) return 0;

	SoInput componentInput;
	if ( !componentInput.openFile( fileName.toLatin1().constData() ) )
	{
        QMessageBox::warning( 0, QString( "Scene Graph Structure" ),
        		QString( "Cannot open file %1:\n." ).arg( fileName ) );
		return 0;
	}

	SoSeparator* componentSeparator = SoDB::readAll( &componentInput );
	componentInput.closeFile();

	if ( !componentSeparator )
	{
        QMessageBox::warning( 0, QString( "Scene Graph Structure" ),
        		QString( "Error reading file %1:\n%2." )
                             .arg( fileName ) );
		return 0;
	}

	TSeparatorKit* componentRoot = static_cast< TSeparatorKit* >( componentSeparator->getChild(0) );
	componentRoot->ref();


   return componentRoot;

}


