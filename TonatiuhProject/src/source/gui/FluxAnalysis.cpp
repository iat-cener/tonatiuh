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
***************************************************************************/

#include <cmath>

#include <QFileDialog>
#include <QFutureWatcher>
#include <QMutex>
#include <QPair>
#include <QProgressDialog>
#include <QtConcurrentMap>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>

#include "FluxAnalysis.h"
#include "TSceneKit.h"
#include "SceneModel.h"
#include "InstanceNode.h"
#include "RandomDeviate.h"
#include "TPhotonMap.h"
#include "gc.h"
#include "RayTracer.h"
#include "RayTracerNoTr.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "Transform.h"
#include "trf.h"
#include "TShape.h"
#include "TShapeKit.h"
#include "TTransmissivity.h"

/******************************************
 * FluxAnalysis
 *****************************************/

/*!
 * Create FluxAnalysis object
 */
FluxAnalysis::FluxAnalysis( TSceneKit* currentScene, SceneModel& currentSceneModel, InstanceNode* rootSeparatorInstance,
		int sunWidthDivisions, int sunHeightDivisions, RandomDeviate* randomDeviate ):
m_pCurrentScene( currentScene ),
m_pCurrentSceneModel( &currentSceneModel ),
m_pRootSeparatorInstance( rootSeparatorInstance ),
m_sunWidthDivisions( sunWidthDivisions ),
m_sunHeightDivisions( sunHeightDivisions ),
m_pRandomDeviate( randomDeviate ),
m_pPhotonMap( 0 ),
m_surfaceURL( "" ),
m_tracedRays( 0 ),
m_wPhoton( 0 ),
m_photonCounts( 0 ),
m_heightDivisions( 0 ),
m_widthDivisions( 0 ),
m_xmin( 0 ),
m_xmax( 0 ),
m_ymin( 0 ),
m_ymax( 0 ),
m_maximumPhotons( 0 ),
m_maximumPhotonsXCoord( 0 ),
m_maximumPhotonsYCoord( 0 ),
m_maximumPhotonsError( 0 ),
m_totalPower( 0 )
{

}

/*!
 * Destroy FluxAnalysis object
 */
FluxAnalysis::~FluxAnalysis()
{
	clearPhotonMap();

	if( m_photonCounts )
	{
		for( int h = 0; h < m_heightDivisions; h++ )
		{
			delete[] m_photonCounts[h];
		}

		delete[] m_photonCounts;
	}
}

/*
 * Returns the type of the surface
 */
QString FluxAnalysis::GetSurfaceType( QString nodeURL )
{
	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( nodeURL );
	if( !nodeIndex.isValid()  )		return QLatin1String( "" );

	InstanceNode* instanceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );
	if( !instanceNode || instanceNode == 0 )	return QLatin1String( "" );

	TShapeKit* shapeKit = static_cast< TShapeKit* > ( instanceNode->GetNode() );
	if( !shapeKit || shapeKit == 0 )	return QLatin1String( "" );

	TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return QLatin1String( "" );

	return ( shape->getTypeId().getName().getString() );
}

/*
 * Check if it the selected surface is suitable (cylinder, flat disk or flat rectangle) for the analysis
 */
bool FluxAnalysis::CheckSurface()
{
	QString surfaceType = GetSurfaceType( m_surfaceURL );

	if( ( surfaceType != "ShapeFlatRectangle" ) &&
		( surfaceType != "ShapeFlatDisk" ) &&
		( surfaceType != "ShapeCylinder" ) )
		return false;

	return true;
}

/*
 * Check the surface side.
 */
bool FluxAnalysis::CheckSurfaceSide()
{
	QString surfaceType = GetSurfaceType( m_surfaceURL );

	if( surfaceType == "ShapeFlatRectangle" )
	{
		if( ( m_surfaceSide != "FRONT" ) && ( m_surfaceSide != "BACK" ) )
			return false;
	}
	else if( surfaceType == "ShapeFlatDisk" )
	{
		if( ( m_surfaceSide != "FRONT" ) && ( m_surfaceSide != "BACK" ) )
			return false;
	}
	else if( surfaceType == "ShapeCylinder" )
	{
		if( ( m_surfaceSide != "INSIDE" ) && ( m_surfaceSide != "OUTSIDE" ) )
			return false;
	}

	return true;
}

/*
 * Fun flux analysis
 */
void FluxAnalysis::RunFluxAnalysis( QString nodeURL, QString surfaceSide, unsigned long nOfRays, bool increasePhotonMap, int heightDivisions, int widthDivisions )
{
	m_surfaceURL = nodeURL;
	m_surfaceSide = surfaceSide;

	//Delete a photonCounts
	if( m_photonCounts && m_photonCounts != 0 )
	{
		for( int h = 0; h < m_heightDivisions; h++ )
		{
			delete[] m_photonCounts[h];
		}

		delete[] m_photonCounts;
	}
	m_photonCounts = 0;
	m_heightDivisions = heightDivisions;
	m_widthDivisions = widthDivisions;

	//Check if there is a scene
	if ( !m_pCurrentScene )  return;

	//Check if there is a transmissivity defined
	TTransmissivity* transmissivity = 0;
	if ( !m_pCurrentScene->getPart( "transmissivity", false ) )	transmissivity = 0;
	else
		transmissivity = static_cast< TTransmissivity* > ( m_pCurrentScene->getPart( "transmissivity", false ) );

	//Check if there is a rootSeparator InstanceNode
	if( !m_pRootSeparatorInstance ) return;

	InstanceNode* sceneInstance = m_pRootSeparatorInstance->GetParent();
	if ( !sceneInstance )  return;

	//Check if there is a light and is properly configured
	if ( !m_pCurrentScene->getPart( "lightList[0]", false ) )return;
	TLightKit* lightKit = static_cast< TLightKit* >( m_pCurrentScene->getPart( "lightList[0]", false ) );

	InstanceNode* lightInstance = sceneInstance->children[0];
	if ( !lightInstance ) return;

	if( !lightKit->getPart( "tsunshape", false ) ) return;
	TSunShape* sunShape = static_cast< TSunShape * >( lightKit->getPart( "tsunshape", false ) );

	if( !lightKit->getPart( "icon", false ) ) return;
	TLightShape* raycastingSurface = static_cast< TLightShape * >( lightKit->getPart( "icon", false ) );

	if( !lightKit->getPart( "transform" ,false ) ) return;
	SoTransform* lightTransform = static_cast< SoTransform * >( lightKit->getPart( "transform" ,false ) );

	//Check if there is a random generator is defined.
	if( !m_pRandomDeviate || m_pRandomDeviate== 0 )	return;

	//Check if the surface and the surface side defined is suitable
	if( CheckSurface() == false || CheckSurfaceSide() == false ) return;

	//Create the photon map where photons are going to be stored
	if( !m_pPhotonMap  || !increasePhotonMap )
	{
		if( m_pPhotonMap ) 	m_pPhotonMap->EndStore( -1 );
		delete m_pPhotonMap;
		m_pPhotonMap = new TPhotonMap();
		m_pPhotonMap->SetBufferSize( HUGE_VAL );
		m_tracedRays = 0;
		m_wPhoton = 0;
		m_totalPower = 0;
	}

	QVector< InstanceNode* > exportSuraceList;
	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_surfaceURL );
	if( !nodeIndex.isValid()  )	return;

	InstanceNode* surfaceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );
	if( !surfaceNode || surfaceNode == 0 )	return;
	exportSuraceList.push_back( surfaceNode );

	//UpdateLightSize();
	TSeparatorKit* concentratorRoot = static_cast< TSeparatorKit* >( m_pCurrentScene->getPart( "childList[0]", false ) );
	if ( !concentratorRoot )	return;

	SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
	concentratorRoot->getBoundingBox( bbAction );

	SbBox3f box = bbAction->getXfBoundingBox().project();
	delete bbAction;
	bbAction = 0;

	BBox sceneBox;
	if( !box.isEmpty() )
	{
		sceneBox.pMin = Point3D( box.getMin()[0], box.getMin()[1], box.getMin()[2] );
		sceneBox.pMax = Point3D( box.getMax()[0], box.getMax()[1], box.getMax()[2] );
		if( lightKit ) lightKit->Update( sceneBox );
	}

	m_pCurrentSceneModel->UpdateSceneModel();

	//Compute bounding boxes and world to object transforms
	trf::ComputeSceneTreeMap( m_pRootSeparatorInstance, Transform( new Matrix4x4 ), true );

	m_pPhotonMap->SetConcentratorToWorld( m_pRootSeparatorInstance->GetIntersectionTransform() );

	QStringList disabledNodes = QString( lightKit->disabledNodes.getValue().getString() ).split( ";", QString::SkipEmptyParts );
	QVector< QPair< TShapeKit*, Transform > > surfacesList;
	trf::ComputeFistStageSurfaceList( m_pRootSeparatorInstance, disabledNodes, &surfacesList );
	lightKit->ComputeLightSourceArea( m_sunWidthDivisions, m_sunHeightDivisions, surfacesList );
	if( surfacesList.count() < 1 )	return;

	QVector< long > raysPerThread;
	int maximumValueProgressScale = 100;

	unsigned long  t1 = nOfRays/ maximumValueProgressScale;
	for( int progressCount = 0; progressCount < maximumValueProgressScale; ++ progressCount )
		raysPerThread<< t1;

	if( ( t1 * maximumValueProgressScale ) < nOfRays )	raysPerThread<< ( nOfRays - ( t1* maximumValueProgressScale) );

	Transform lightToWorld = tgf::TransformFromSoTransform( lightTransform );
	lightInstance->SetIntersectionTransform( lightToWorld.GetInverse() );

	// Create a progress dialog.
	QProgressDialog dialog;
	dialog.setLabelText( QString("Progressing using %1 thread(s)..." ).arg( QThread::idealThreadCount() ) );

	// Create a QFutureWatcher and conncect signals and slots.
	QFutureWatcher< void > futureWatcher;
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

	QMutex mutex;
	QMutex mutexPhotonMap;
	QFuture< void > photonMap;
	if( transmissivity )
		photonMap = QtConcurrent::map( raysPerThread, RayTracer( m_pRootSeparatorInstance,
							 lightInstance, raycastingSurface, sunShape, lightToWorld,
							 transmissivity,
							 *m_pRandomDeviate,
							 &mutex, m_pPhotonMap, &mutexPhotonMap,
							 exportSuraceList ) );
	else
		photonMap = QtConcurrent::map( raysPerThread, RayTracerNoTr( m_pRootSeparatorInstance,
						lightInstance, raycastingSurface, sunShape, lightToWorld,
						*m_pRandomDeviate,
						&mutex, m_pPhotonMap, &mutexPhotonMap,
						exportSuraceList ) );

	futureWatcher.setFuture( photonMap );

	// Display the dialog and start the event loop.
	dialog.exec();
	futureWatcher.waitForFinished();

	m_tracedRays += nOfRays;

	double irradiance = sunShape->GetIrradiance();
	double inputAperture = raycastingSurface->GetValidArea();
	m_wPhoton = double ( inputAperture * irradiance ) / m_tracedRays;

	UpdatePhotonCounts();
}

/*
 * Update photon counts for a specific grid divisions
 */
void FluxAnalysis::UpdatePhotonCounts( int heightDivisions, int widthDivisions )
{
	//Delete a photonCounts
	if( m_photonCounts && (m_photonCounts != 0) )
	{
		for( int h = 0; h < m_heightDivisions; h++ )
		{
			delete[] m_photonCounts[h];
		}

		delete[] m_photonCounts;
	}

	m_heightDivisions = heightDivisions;
	m_widthDivisions = widthDivisions;

	UpdatePhotonCounts();
}

/*
 * Update photon counts
 */
void FluxAnalysis::UpdatePhotonCounts()
{
	if( !m_pPhotonMap )	return;

	//Create a new photonCounts
	m_photonCounts = new int*[m_heightDivisions];
	for( int h = 0; h < m_heightDivisions; h++ )
	{
		m_photonCounts[h] = new int[m_widthDivisions];
		for( int w = 0; w < m_widthDivisions; w++ )
			m_photonCounts[h][w] = 0;
	}

	m_maximumPhotons = 0;
	m_maximumPhotonsXCoord = 0;
	m_maximumPhotonsYCoord = 0;
	m_maximumPhotonsError = 0;

	QString surfaceType = GetSurfaceType( m_surfaceURL );
	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_surfaceURL );
	InstanceNode* instanceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );

	if( surfaceType == "ShapeFlatRectangle" )
	{
		FluxAnalysisFlatRectangle( instanceNode );
	}
	else if( surfaceType == "ShapeFlatDisk" )
	{
		FluxAnalysisFlatDisk( instanceNode );
	}
	else if( surfaceType == "ShapeCylinder" )
	{
		FluxAnalysisCylinder( instanceNode );
	}
}

/*
 * Flux Analysis for cylinder surfaces.
 */
void FluxAnalysis::FluxAnalysisCylinder( InstanceNode* node )
{
	if( !node )	return;
	TShapeKit* surfaceNode = static_cast< TShapeKit* > ( node->GetNode() );
	if( !surfaceNode )	return;

	TShape* shape = static_cast< TShape* >( surfaceNode->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;

	trt::TONATIUH_REAL* radiusField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "radius" ) );
	double radius = radiusField->getValue();

	trt::TONATIUH_REAL* lengthField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "length" ) );
	double length = lengthField->getValue();

	trt::TONATIUH_REAL* phiMaxField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "phiMax" ) );
	double phiMax = phiMaxField->getValue();

	int widthDivisionsError = m_widthDivisions-1;
	int heightDivisionsError = m_heightDivisions-1;

	int** photonCountsError = new int*[heightDivisionsError];
	for(int i = 0; i < heightDivisionsError; ++i)
	{
		photonCountsError[i] = new int[widthDivisionsError];
		for( int w = 0; w < widthDivisionsError; w++ )
			photonCountsError[i][w] = 0;
	}

	int activeSideID = 1;
	if( m_surfaceSide == "INSIDE" )
		activeSideID = 0;

	Transform worldToObject = node->GetIntersectionTransform();

	m_xmin = 0.0;
	m_ymin = 0.0;
	m_xmax = phiMax  * radius;
	m_ymax = length;

	std::vector< Photon* > photonList = m_pPhotonMap->GetAllPhotons();
	int totalPhotons = 0;
	for( unsigned int p = 0; p < photonList.size(); p++ )
	{
		Photon* photon = photonList[p];
		if( photon->side == activeSideID )
		{
			totalPhotons++;
			Point3D photonLocalCoord = worldToObject( photon->pos );
			double phi  = atan2( photonLocalCoord.y, photonLocalCoord.x );
			if( phi < 0.0 ) phi += 2* gc::Pi;
			double arcLength = phi * radius;

			int xbin = floor( ( arcLength - m_xmin )/( m_xmax - m_xmin ) * m_widthDivisions ) ;
			int ybin = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax - m_ymin ) * m_heightDivisions );
			m_photonCounts[ybin][xbin] += 1;
			if( m_maximumPhotons < m_photonCounts[ybin][xbin] )
			{
				m_maximumPhotons = m_photonCounts[ybin][xbin];
				m_maximumPhotonsXCoord = xbin;
				m_maximumPhotonsYCoord = ybin;
			}

			int xbinE = floor( ( arcLength - m_xmin )/( m_xmax - m_xmin ) * widthDivisionsError ) ;
			int ybinE = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax - m_ymin ) * heightDivisionsError );
			photonCountsError[ybinE][xbinE] += 1;
			if( m_maximumPhotonsError < photonCountsError[ybinE][xbinE] )
			{
				m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
			}
		}
	}

	m_totalPower = totalPhotons * m_wPhoton;

	if( photonCountsError )
	{
		for( int h = 0; h < heightDivisionsError; h++ )
		{
			delete[] photonCountsError[h];
		}

		delete[] photonCountsError;
	}
}

/*
 * Flux Analysis for flat disk surfaces.
 */
void FluxAnalysis::FluxAnalysisFlatDisk( InstanceNode* node )
{
	if( !node )	return;
	TShapeKit* surfaceNode = static_cast< TShapeKit* > ( node->GetNode() );
	if( !surfaceNode )	return;

	TShape* shape = static_cast< TShape* >( surfaceNode->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;

	trt::TONATIUH_REAL* radiusField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "radius" ) );
	double radius = radiusField->getValue();

	int widthDivisionsError = m_widthDivisions-1;
	int heightDivisionsError = m_heightDivisions-1;

	int** photonCountsError = new int*[heightDivisionsError];
	for(int i = 0; i < heightDivisionsError; ++i)
	{
		photonCountsError[i] = new int[widthDivisionsError];
		for( int w = 0; w < widthDivisionsError; w++ )
			photonCountsError[i][w] = 0;
	}

	int activeSideID = 1;
	if( m_surfaceSide == "BACK" )
		activeSideID = 0;

	Transform worldToObject = node->GetIntersectionTransform();

	m_xmin = -radius;
	m_ymin = -radius;
	m_xmax = radius;
	m_ymax = radius;

	std::vector< Photon* > photonList = m_pPhotonMap->GetAllPhotons();
	int totalPhotons = 0;
	for( unsigned int p = 0; p < photonList.size(); p++ )
	{
		Photon* photon = photonList[p];
		if( photon->side == activeSideID )
		{
			totalPhotons++;
			Point3D photonLocalCoord = worldToObject( photon->pos );
			int xbin = floor( ( photonLocalCoord.x - m_xmin )/( m_xmax - m_xmin ) * m_widthDivisions );
			int ybin = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax - m_ymin ) * m_heightDivisions );
			m_photonCounts[ybin][xbin] += 1;
			if( m_maximumPhotons < m_photonCounts[ybin][xbin] )
			{
				m_maximumPhotons = m_photonCounts[ybin][xbin];
				m_maximumPhotonsXCoord = xbin;
				m_maximumPhotonsYCoord = ybin;
			}

			int xbinE = floor( ( photonLocalCoord.x - m_xmin )/( m_xmax - m_xmin ) * widthDivisionsError ) ;
			int ybinE = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax - m_ymin ) * heightDivisionsError );
			photonCountsError[ybinE][xbinE] += 1;
			if( m_maximumPhotonsError < photonCountsError[ybinE][xbinE] )
			{
				m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
			}
		}
	}

	m_totalPower = totalPhotons * m_wPhoton;

	if( photonCountsError )
	{
		for( int h = 0; h < heightDivisionsError; h++ )
		{
			delete[] photonCountsError[h];
		}

		delete[] photonCountsError;
	}
}

/*
 * Flux Analysis for flat rectangle surfaces.
 */
void FluxAnalysis::FluxAnalysisFlatRectangle( InstanceNode* node )
{
	if( !node )	return;

	TShapeKit* surfaceNode = static_cast< TShapeKit* > ( node->GetNode() );
	if( !surfaceNode )	return;

	TShape* shape = static_cast< TShape* >( surfaceNode->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;

	trt::TONATIUH_REAL* widthField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "width" ) );
	double surfaceWidth = widthField->getValue();

	trt::TONATIUH_REAL* heightField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "height" ) );
	double surfaceHeight= heightField->getValue();

	int widthDivisionsError = m_widthDivisions-1;
	int heightDivisionsError = m_heightDivisions-1;

	int** photonCountsError = new int*[heightDivisionsError];
	for(int i = 0; i < heightDivisionsError; ++i)
	{
		photonCountsError[i] = new int[widthDivisionsError];
		for( int w = 0; w < widthDivisionsError; w++ )
			photonCountsError[i][w] = 0;
	}

	int activeSideID = 1;
	if( m_surfaceSide == "BACK" )
		activeSideID = 0;

	Transform worldToObject = node->GetIntersectionTransform();

	m_xmin = -0.5 * surfaceHeight;
	m_ymin = -0.5 * surfaceWidth;
	m_xmax = 0.5 * surfaceHeight;
	m_ymax = 0.5 * surfaceWidth;

	std::vector< Photon* > photonList = m_pPhotonMap->GetAllPhotons();
	int totalPhotons = 0;
	for( unsigned int p = 0; p < photonList.size(); p++ )
	{
		Photon* photon = photonList[p];
		if( photon->side == activeSideID )
		{
			totalPhotons++;
			Point3D photonLocalCoord = worldToObject( photon->pos );
			int xbin = floor( ( photonLocalCoord.x - m_xmin )/( m_xmax - m_xmin ) * m_widthDivisions ) ;
			int ybin = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax - m_ymin ) * m_heightDivisions );

			m_photonCounts[ybin][xbin] += 1;
			if( m_maximumPhotons < m_photonCounts[ybin][xbin] )
			{
				m_maximumPhotons = m_photonCounts[ybin][xbin];
				m_maximumPhotonsXCoord = xbin;
				m_maximumPhotonsYCoord = ybin;
			}

			int xbinE = floor( ( photonLocalCoord.x - m_xmin )/( m_xmax-m_xmin ) * widthDivisionsError ) ;
			int ybinE = floor( ( photonLocalCoord.z - m_ymin )/( m_ymax-m_ymin ) * heightDivisionsError );
			photonCountsError[ybinE][xbinE] += 1;
			if( m_maximumPhotonsError < photonCountsError[ybinE][xbinE] )
			{
				m_maximumPhotonsError = photonCountsError[ybinE][xbinE];
			}
		}
	}

	m_totalPower = totalPhotons * m_wPhoton;

	if( photonCountsError )
	{
		for( int h = 0; h < heightDivisionsError; h++ )
		{
			delete[] photonCountsError[h];
		}

		delete[] photonCountsError;
	}
}

/*
 * Export the flux distribution
 */
void FluxAnalysis::ExportAnalysis( QString directory, QString fileName, bool saveCoords )
{
	if( m_pPhotonMap == 0 || !m_pPhotonMap ) return;

	if( directory.isEmpty() ) return;

	if( fileName.isEmpty() ) return;

	QFileInfo exportFileInfo( fileName );
	if( exportFileInfo.completeSuffix().compare( "txt" ) )	fileName.append( ".txt" );

	QFile exportFile( directory + "/" + fileName  );
	exportFile.open( QIODevice::WriteOnly );
	QTextStream out( &exportFile );

	double widthCell = ( m_xmax - m_xmin ) / m_widthDivisions;
	double heightCell = ( m_ymax - m_ymin ) / m_heightDivisions;
	double areaCell = widthCell * heightCell;

	if( saveCoords )
	{
		out<<"x(m)\ty(m)\tFlux(W/m2)"<<"\n";

		for( int i = 0; i < m_heightDivisions; i++ )
		{
			for( int j = 0; j < m_widthDivisions; j++ )
			{
				out<< m_xmin + widthCell/2 + j * widthCell  << "\t" << m_ymin + heightCell/2 + i * heightCell <<  "\t" << m_photonCounts[i][j] * m_wPhoton / areaCell << "\n";
			}
		}
	}
	else
	{
		for( int i = 0; i < m_heightDivisions; i++ )
		{
			for( int j = 0; j < m_widthDivisions; j++ )
			{
				out<< m_photonCounts[m_heightDivisions-1-i][j] * m_wPhoton / areaCell << "\t";
			}
			out<<"\n" ;
		}
	}
	exportFile.close();
}

/*
 * Returns m_photoCounts.
 */
int** FluxAnalysis::photonCountsValue()
{
	return m_photonCounts;
}

/*
 * Returns m_xmin value.
 */
double FluxAnalysis::xminValue()
{
	return m_xmin;
}

/*
 * Returns m_xmax value.
 */
double FluxAnalysis::xmaxValue()
{
	return m_xmax;
}

/*
 * Returns m_ymin value.
 */
double FluxAnalysis::yminValue()
{
	return m_ymin;
}

/*
 * Returns m_ymax value.
 */
double FluxAnalysis::ymaxValue()
{
	return m_ymax;
}

/*
 * Returns m_maximumPhotons value.
 */
int FluxAnalysis::maximumPhotonsValue()
{
	return m_maximumPhotons;
}

/*
 * Returns m_maximumPhotonsXCoord value.
 */
int FluxAnalysis::maximumPhotonsXCoordValue()
{
	return m_maximumPhotonsXCoord;
}

/*
 * Returns m_maximumPhotonsYCoord value.
 */
int FluxAnalysis::maximumPhotonsYCoordValue()
{
	return m_maximumPhotonsYCoord;
}

/*
 * Returns m_maximumPhotonsError value.
 */
int FluxAnalysis::maximumPhotonsErrorValue()
{
	return m_maximumPhotonsError;
}

/*
 * Returns m_wPhoton value.
 */
double FluxAnalysis::wPhotonValue()
{
	return m_wPhoton;
}

/*
 * Returns m_totalFlux value.
 */
double FluxAnalysis::totalPowerValue()
{
	return m_totalPower;
}

/*
 * Clear photon map
 */
void FluxAnalysis::clearPhotonMap()
{
	if( m_pPhotonMap ) 	m_pPhotonMap->EndStore( -1 );
	delete m_pPhotonMap;
	m_pPhotonMap = 0;
	m_tracedRays = 0;
	m_wPhoton = 0;
	m_totalPower = 0;
}
