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

#include "PluginManager.h"
#include "TSceneKit.h"
#include "TSeparatorKit.h"
#include "TTracker.h"
#include "TTrackerFactory.h"

#include "ComponentHeliostatField.h"
#include "HeliostatFieldWizard.h"


bool comparePuntosPorX( Point3D pA, Point3D pB )
{
	return ( pA.x < pB.x );
}

bool comparePuntosPorZ( Point3D pA, Point3D pB )
{
	return ( pA.z < pB.z );
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
        		QString( "No se ha encontrado plugins de tipo tracker." ) );
		return 0;
	}

	QVector< QString > trackerNames;
	for( int i = 0; i < trackersFactoryList.size(); i++ )
		trackerNames<< trackersFactoryList[i]->TTrackerName();

	int selectedTracker = trackerNames.indexOf( QLatin1String( "Heliostat_tracker" ) );
	if( selectedTracker < 0 )
	{
        QMessageBox::warning( 0, QString( "Campo Heliostatos" ),
        		QString( "No se ha encontrado el plugin de tipo 'Heliostat_tracker' " ) );
		return 0;
	}

	TTrackerFactory* heliostatTrackerFactory = trackersFactoryList[ selectedTracker ];


	HeliostatFieldWizard wizard;
	if( !wizard.exec() )
	{
		QMessageBox::information( 0, QString( "Campo Heliostatos" ),
				QString( "The field has not been created." ) );
		return 0;
	}

	//Aiming point
	Point3D aimingPoint = wizard.GetHeliostatsAimingPoint();

	//Read heliostat component

	//TSeparatorKit::initClass();

	TSeparatorKit* heliostatComponentNode = OpenHeliostatComponent( wizard.GetHeliostatComponentFile() );
	if( !heliostatComponentNode )
	{
		QMessageBox::information( 0, QString( "Campo Heliostatos" ),
				QString( "Error al leer el componente para los heliostatos." ) );
		return 0;
	}


	QFile coordinatesFile( wizard.GetCoordinatesFile() );
	if( !coordinatesFile.open( QIODevice::ReadOnly ) )
	{
		QMessageBox::information( 0, QString( "Campo Heliostatos" ),
				QString( "Error al leer el archivo de coordenadas de los heliostatos." ) );
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


	SoType separatorType = SoType::fromName( SbName ( "TSeparatorKit" ) );

	/*TSeparatorKit* rootSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
	//TSeparatorKit* rootSeparator = new TSeparatorKit;
	rootSeparator->setName( "CampoHeliostatos" );
	rootSeparator->ref();

	SoNodeKitListPart* rootPartList = static_cast< SoNodeKitListPart* >( rootSeparator->getPart( "childList", true ) );
	if( !rootPartList ) return 0;
	*/


	TSeparatorKit* heliostatsNodeSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
	//TSeparatorKit* heliostatsNodeSeparator = new TSeparatorKit;
	//rootPartList->addChild(heliostatsNodeSeparator);
	heliostatsNodeSeparator->setName( "Heliostatos" );
	heliostatsNodeSeparator->ref();



	CreateHeliostatZones( hCenterList, heliostatsNodeSeparator, heliostatTrackerFactory, aimingPoint, heliostatComponentNode, 1 );

	return heliostatsNodeSeparator;

}


void ComponentHeliostatField::CreateHeliostatZones( std::vector< Point3D >  heliostatCenterList, TSeparatorKit* parentNode,
		TTrackerFactory* heliostatTrackerFactory,
		Point3D aimingPoint,
		TSeparatorKit* heliostatComponentNode,
		int eje )
{

	SoType separatorType = SoType::fromName( SbName ( "TSeparatorKit" ) );

	SoNodeKitListPart* heliostatsNodePartList = static_cast< SoNodeKitListPart* >( parentNode->getPart( "childList", true ) );
	if( !heliostatsNodePartList ) return;

	int nHeliostatCenters = ( int ) heliostatCenterList.size();
	if( nHeliostatCenters < 8 )
	{

		for( int nHeliostat = 0; nHeliostat < nHeliostatCenters; nHeliostat++ )
		{

			//TSeparatorKit* heliostatSeparator = new TSeparatorKit;
			TSeparatorKit* heliostatSeparator = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
			heliostatsNodePartList->addChild(heliostatSeparator);
			QString heliostatName = QString( QLatin1String( "Heliostat%1" ) ).arg( QString::number( nHeliostat ) );
			heliostatSeparator->setName( heliostatName.toStdString().c_str() );
			SoTransform* nodeTransform = dynamic_cast< SoTransform* >( heliostatSeparator->getPart( "transform", true ) );
			nodeTransform->translation.setValue( heliostatCenterList[nHeliostat].x,
					heliostatCenterList[nHeliostat].y,
					heliostatCenterList[nHeliostat].z );

			SoNodeKitListPart*  heliostatPartList = static_cast< SoNodeKitListPart* >( heliostatSeparator->getPart( "childList", true ) );
			if( !heliostatPartList ) return;

			//TSeparatorKit* heliostatTrackerNode= new TSeparatorKit;
			TSeparatorKit* heliostatTrackerNode = static_cast< TSeparatorKit* > ( separatorType.createInstance() );
			heliostatPartList->addChild( heliostatTrackerNode );
			heliostatTrackerNode->setName( "HeliostatTrackerNode" );
			SoNodeKitListPart*  heliostaTrackerNodetPartList = static_cast< SoNodeKitListPart* >( heliostatTrackerNode->getPart( "childList", true ) );
			if( !heliostaTrackerNodetPartList ) return;

			TTracker* tracker = heliostatTrackerFactory->CreateTTracker();
			heliostatTrackerNode->setPart( "tracker", tracker );
			SoSFVec3f* aimingPointField = dynamic_cast< SoSFVec3f* > ( tracker->getField ( "aimingPoint" ) );
			if( aimingPointField )	aimingPointField->setValue( SbVec3f( aimingPoint.x, aimingPoint.y, aimingPoint.z ) );

			if( heliostatComponentNode )	heliostaTrackerNodetPartList->addChild( heliostatComponentNode );

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
			std::sort( heliostatCenterList.begin(), heliostatCenterList.end(), comparePuntosPorZ );

			std::vector< Point3D >::iterator it;

			double zMin = heliostatCenterList[0].z;
			double zMax = heliostatCenterList[nHeliostatCenters-1].z;

			double splitPoint = zMin + ( 0.5 * ( zMax - zMin ) );
			std::vector< Point3D > hCenterListPart1;
			double position = 0;
			while( ( position < heliostatCenterList.size() )
					&& ( heliostatCenterList[position].z < splitPoint ) )
			{
				hCenterListPart1.push_back( heliostatCenterList[position] );
				position++;
			}


			std::vector< Point3D > hCenterListPart2;
			while( position < heliostatCenterList.size() )
			{
				hCenterListPart2.push_back( heliostatCenterList[position] );
				position++;
			}
			CreateHeliostatZones( hCenterListPart1, heliostatSeparator1, heliostatTrackerFactory, aimingPoint, heliostatComponentNode, 1 );
			CreateHeliostatZones( hCenterListPart2, heliostatSeparator2, heliostatTrackerFactory, aimingPoint, heliostatComponentNode, 1 );

		}
		else
		{
			std::sort( heliostatCenterList.begin(), heliostatCenterList.end(), comparePuntosPorX );
			double xMin = heliostatCenterList[0].x;
			double xMax = heliostatCenterList[nHeliostatCenters-1].x;

			double splitPoint = xMin + ( 0.5 * ( xMax - xMin ) );
			std::vector< Point3D > hCenterListPart1;
			double position = 0;
			while( ( position < heliostatCenterList.size() )
					&& ( heliostatCenterList[position].x < splitPoint ) )
			{
				hCenterListPart1.push_back( heliostatCenterList[position] );
				position++;
			}


			std::vector< Point3D > hCenterListPart2;
			while( position < heliostatCenterList.size() )
			{
				hCenterListPart2.push_back( heliostatCenterList[position] );
				position++;
			}
			CreateHeliostatZones( hCenterListPart1, heliostatSeparator1, heliostatTrackerFactory, aimingPoint, heliostatComponentNode, 3 );
			CreateHeliostatZones( hCenterListPart2, heliostatSeparator2, heliostatTrackerFactory, aimingPoint, heliostatComponentNode, 3 );
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


