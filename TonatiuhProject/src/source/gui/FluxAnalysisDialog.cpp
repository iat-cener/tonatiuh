/*
 * FluxAnalysisDialog.cpp
 *
 *  Created on: 08/10/2015
 *      Author: amutuberria
 */

#include <cmath>

#include <QFutureWatcher>
#include <QMessageBox>
#include <QMutex>
#include <QPair>
#include <QProgressDialog>
#include <QtConcurrentMap>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>

#include "FluxAnalysisDialog.h"
#include "gc.h"
#include "InstanceNode.h"
#include "RandomDeviate.h"
#include "RayTracer.h"
#include "RayTracerNoTr.h"
#include "SceneModel.h"
#include "SelectSurfaceDialog.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "Transform.h"
#include "trf.h"
#include "TSceneKit.h"
#include "TShape.h"
#include "TShapeKit.h"
#include "TTransmissivity.h"


/******************************************
 * FluxAnalysisDialog
 *****************************************/
/*!
 * Create dialog object
 */
FluxAnalysisDialog::FluxAnalysisDialog( TSceneKit* currentScene, SceneModel& currentSceneModel,
		InstanceNode* rootSeparatorInstance,
		int widthDivisions, int heightDivisions,
		RandomDeviate* randomDeviate,  QWidget* parent  )
:QDialog( parent),
 m_currentSurfaceURL( "---" ),
 m_pCurrentScene( currentScene ),
 m_pCurrentSceneModel( &currentSceneModel ),
 m_pPhotonMap( 0 ),
 m_pRandomDeviate( randomDeviate),
 m_pRootSeparatorInstance( rootSeparatorInstance ),
 m_sunHeightDivisions( heightDivisions ),
 m_sunWidthDivisions( widthDivisions ),
 m_tracedRays ( 0 ),
 m_wPhoton( 0.0 ),
 m_xmin( 0.0 ),
 m_xmax( 0.0 ),
 m_ymin( 0.0 ),
 m_ymax( 0.0 ),
 m_heightDivisions( 0 ),
 m_widthDivisions( 0 ),
 m_photonCounts(0)
{
	setupUi( this );

	QSize windowSize = size();
	QList<int> sizes;
    sizes<<windowSize.width() * 0.3 <<windowSize.width() * 0.7;
    splitter->setSizes ( sizes );

	QSize resultsWidgetSize = resultsWidget->size();
	int resultsWidgetSizeHeight = resultsWidgetSize.height();

	contourPlotWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	sectorsWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	statisticalWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.25 );


	connect( selectButton, SIGNAL( clicked() ), this, SLOT( SelectSurface() ) );
	connect( sidesCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeCurrentSurfaceSide() ) );
	connect( surfaceEdit, SIGNAL( editingFinished() ), this, SLOT( ChangeCurrentSurface() ) );
	connect( runButton, SIGNAL( clicked()  ), this, SLOT( RunFluxAnalysis() ) );
	connect( hSectorXCoordSpin, SIGNAL( valueChanged( double ) ), this, SLOT( UpdateSectorPlots() ) );
	connect( hSectorYCoordSpin, SIGNAL( valueChanged( double ) ), this, SLOT( UpdateSectorPlots() ) );
	connect( gridWidthSpin, SIGNAL( valueChanged( int ) ), this, SLOT( UpdateAnalysis() ) );
	connect( gridHeightSpin, SIGNAL( valueChanged( int ) ), this, SLOT( UpdateAnalysis() ) );
	connect( selectFileButton, SIGNAL( clicked() ), this, SLOT( SelectExportFile() ) );
	connect( exportButton, SIGNAL( clicked() ) , this, SLOT( ExportData() ) );


	// configure axis rect:
	contourPlotWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
	contourPlotWidget->axisRect()->setupFullAxesBox(true);

	contourPlotWidget->plotLayout()->insertRow(0);
	contourPlotWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(contourPlotWidget, "Flux Distribution"));
	contourPlotWidget->xAxis->setLabel("X (unit length)");
	contourPlotWidget->yAxis->setLabel("Y (unit length)");


	horizontaSectorPlot->plotLayout()->insertRow(0);
	horizontaSectorPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(horizontaSectorPlot, "Horizontal Sector"));
	// give the axes some labels:
	horizontaSectorPlot->xAxis->setLabel("");
	horizontaSectorPlot->yAxis->setLabel("Flux");

	verticalSectorPlot->plotLayout()->insertRow(0);
	verticalSectorPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(verticalSectorPlot, "Vertical Sector"));
	// give the axes some labels:
	verticalSectorPlot->xAxis->setLabel("");
	verticalSectorPlot->yAxis->setLabel("Flux");

}


/*!
 * Create dialog object
 */
FluxAnalysisDialog::~FluxAnalysisDialog()
{
	delete m_pPhotonMap;

	if( m_photonCounts )
	{
		for( int h = 0; h < m_heightDivisions; h++ )
		{
			delete[] m_photonCounts[h];
		}

		delete[] m_photonCounts;
	}
}

/*!
 * Resizes results widget elements sizes when the dialog windows size is changed.
 */
void  FluxAnalysisDialog::resizeEvent(QResizeEvent* /*event*/)
{
	QSize resultsWidgetSize = resultsWidget->size();
	int resultsWidgetSizeHeight = resultsWidgetSize.height();


	contourPlotWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	sectorsWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	statisticalWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.25 );

}

/*!
 *Changes current surface to the surface node defined in surface line.
 */

void FluxAnalysisDialog::ChangeCurrentSurface()
{
	QString selectedSurfaceURL = surfaceEdit->text();
	if( selectedSurfaceURL != QLatin1String( "---" ) && selectedSurfaceURL != QLatin1String( "" ) )
	{
		QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_currentSurfaceURL );
		if( nodeIndex.isValid()  )
		{

			InstanceNode* instanceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );
			if( instanceNode && instanceNode != 0 )
			{

				UpdateSurfaceSides( selectedSurfaceURL );
				return;
			}
		}
		QMessageBox::warning( this, tr( "Tonatiuh" ),
									 tr( "The surface url defined for is not valid." ) );
		UpdateSurfaceSides( QLatin1String( "---" ) );
	}

}


void FluxAnalysisDialog::ChangeCurrentSurfaceSide()
{
	//Create the photon map where photons are going to be stored
	if( m_pPhotonMap  )
	{
		ClearCurrentAnalysis();
	}


}

void FluxAnalysisDialog::ExportData()
{
	if ( m_pPhotonMap == 0 || !m_pPhotonMap )
		{
			QString message = QString( tr( "Nothing available to export, first run the simulation" ) );
			QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
			return;
		}

	if ( fileDirEdit->text().isEmpty() )
	{
		QString message = QString( tr( "Directory not valid" ) );
		QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
		return;
	}

	if ( fileNameEdit->text().isEmpty() )
	{
		QString message = QString( tr( "File name not valid" ) );
		QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
		return;
	}

	QString exportDirectory( fileDirEdit->text() );
	QString storeType = storeTypeCombo->currentText();
	QString exportFileName = fileNameEdit->text();

	if ( storeType == "ASCII" )
	{
		QFile exportFile( exportDirectory + "/" + exportFileName + ".txt" );
		exportFile.open( QIODevice::WriteOnly );
		QTextStream out( &exportFile );

		double widthCell = (m_xmax-m_xmin) / m_widthDivisions;
		double heightCell = (m_ymax-m_ymin) / m_heightDivisions;
		double areaCell = widthCell * heightCell;

		for ( int i = 0; i<m_heightDivisions; i++)
		{
			for (int j = 0; j<m_widthDivisions; j++ )
			{
				out<< m_photonCounts[i][j] * m_wPhoton / areaCell << " ";
			}
		out<< "\n" ;
		}
		exportFile.close();
	}
	else if ( storeType == "IMAGE.JPG" )
	{
		QFile exportFile( exportDirectory + "/" + exportFileName + ".jpg" );
		exportFile.open( QIODevice::WriteOnly );
		contourPlotWidget->saveJpg( exportFile.fileName(), 0, 0, 1.0, -1 ); //(QString &  fileName, int  width = 0, int  height = 0, double  scale = 1.0, int  quality = -1  )
		exportFile.close();
	}
	else if ( storeType == "IMAGE.PNG" )
	{
		QFile exportFile( exportDirectory + "/" + exportFileName + ".png" );
		exportFile.open( QIODevice::WriteOnly );
		contourPlotWidget->savePng( exportFile.fileName(), 0, 0, 1.0, -1 ); //(QString &  fileName, int  width = 0, int  height = 0, double  scale = 1.0, int  quality = -1  )
		exportFile.close();
	}

	QString message = QString( tr( "Export done successfully" ) );
	QMessageBox::information( this,  QLatin1String( "Tonatiuh" ), message );

}


/*!
 * Runs flux analysis for current defined surface.
 */
void FluxAnalysisDialog::RunFluxAnalysis()
{

	if( m_photonCounts )
	{
		for( int h = 0; h < m_heightDivisions; h++ )
		{
			delete[] m_photonCounts[h];
		}

		delete[] m_photonCounts;

		m_heightDivisions = 0;
		m_widthDivisions =0;
		m_maximumFlux = 0.0;
		m_photonCounts = 0;
	}

	if( m_currentSurfaceURL == QLatin1String( "---" ) )
		{
			QString message = QString( tr( "Select a surface" ) );
			QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
			return;
		}

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

	//Create the photon map where photons are going to be stored


	if( !m_pPhotonMap  || ( appendCheck->isEnabled() &&  !appendCheck->isChecked() ) )
	{
		if( m_pPhotonMap ) 	m_pPhotonMap->EndStore( -1 );
		delete m_pPhotonMap;
		m_pPhotonMap = new TPhotonMap();
		m_pPhotonMap->SetBufferSize( HUGE_VAL );
		m_tracedRays = 0;
		m_wPhoton = 0;
	}


	QVector< InstanceNode* > exportSuraceList;
	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_currentSurfaceURL );
	if( !nodeIndex.isValid()  )	return;

	//std::cout<<"FluxAnalysisDialog::RunFluxAnalysis surfaceNode: "<<std::endl;
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
	unsigned long raysPerIteration = nRaysSpin->value();
	unsigned long  t1 = raysPerIteration/ maximumValueProgressScale;
	for( int progressCount = 0; progressCount < maximumValueProgressScale; ++ progressCount )
		raysPerThread<< t1;

	if( ( t1 * maximumValueProgressScale ) < raysPerIteration )	raysPerThread<< ( raysPerIteration - ( t1* maximumValueProgressScale) );

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
		photonMap = QtConcurrent::map( raysPerThread, RayTracer(  m_pRootSeparatorInstance,
							 lightInstance, raycastingSurface, sunShape, lightToWorld,
							 transmissivity,
							 *m_pRandomDeviate,
							 &mutex, m_pPhotonMap, &mutexPhotonMap,
							 exportSuraceList ) );
	else
		photonMap = QtConcurrent::map( raysPerThread, RayTracerNoTr(  m_pRootSeparatorInstance,
						lightInstance, raycastingSurface, sunShape, lightToWorld,
						*m_pRandomDeviate,
						&mutex, m_pPhotonMap, &mutexPhotonMap,
						exportSuraceList ) );

	futureWatcher.setFuture( photonMap );

	// Display the dialog and start the event loop.
	dialog.exec();
	futureWatcher.waitForFinished();


	m_tracedRays += raysPerIteration;

	double irradiance = sunShape->GetIrradiance();
	double inputAperture = raycastingSurface->GetValidArea();
	m_wPhoton = double ( inputAperture * irradiance ) / m_tracedRays;


	/*
	TShapeKit* shapeKit = static_cast< TShapeKit* > ( surfaceNode->GetNode() );
	if( !shapeKit || shapeKit == 0 )	return;

	TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;


	if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatRectangle" ) )
	{
		FluxAnalysisFlatRectangle( surfaceNode );
	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatDisk" ) )
	{

		FluxAnalysisFlatDisk( instanceNode );
	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeCylinder" ) )
	{
		FluxAnalysisCylinder( surfaceNode );
	}
	*/
	UpdateAnalysis();

	appendCheck->setEnabled( true );


}



void FluxAnalysisDialog::SelectExportFile()
{
	QString path = QFileDialog::getExistingDirectory(this, "Choose a directory to save");
	fileDirEdit->setText( path );
}


/*!
 * Opens a dialog to select a surface from the system.
 * If the selected surface is ok
 */
void FluxAnalysisDialog::SelectSurface()
{
	QString currentSurface = surfaceEdit->text();

	SelectSurfaceDialog selectSurfaceDialog( *m_pCurrentSceneModel, false, this );
	selectSurfaceDialog.SetShapeTypeFilters( QVector< QString >()<< QLatin1String( "ShapeFlatRectangle")
			<< QLatin1String( "ShapeFlatDisk")
			<< QLatin1String( "ShapeCylinder") );
	if( !selectSurfaceDialog.exec( ) )	return;

	QString selectedSurfaceURL = selectSurfaceDialog.GetSelectedSurfaceURL();

	if( selectedSurfaceURL.isEmpty() )
	{
		selectedSurfaceURL = QLatin1String( "---" );
	}

	surfaceEdit->setText( selectedSurfaceURL );
	UpdateSurfaceSides( selectedSurfaceURL );
}

/*!
 * Repeates the analysis for current defined parameters.
 */
void FluxAnalysisDialog::UpdateAnalysis(  )
{

	if( !m_pPhotonMap || m_pPhotonMap == 0 )	return;


	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_currentSurfaceURL );
	if( !nodeIndex.isValid()  )		return;

	InstanceNode* instanceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );
	if( !instanceNode || instanceNode == 0 )	return;

	TShapeKit* shapeKit = static_cast< TShapeKit* > ( instanceNode->GetNode() );
	if( !shapeKit || shapeKit == 0 )	return;

	TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;


	Transform worldToObject = instanceNode->GetIntersectionTransform();

	if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatRectangle" ) )
	{
		//FluxAnalysisFlatRectangle( instanceNode );
		trt::TONATIUH_REAL* widthField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "width" ) );
		double surfaceWidth = widthField->getValue();

		trt::TONATIUH_REAL* heightField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "height" ) );
		double surfaceHeight= heightField->getValue();

		int activeSideID = 1;
		if( sidesCombo->currentText() == QLatin1String( "BACK" ) )
			activeSideID = 0;


		m_xmin = -0.5 * surfaceHeight;
		m_ymin = -0.5 * surfaceWidth;

		m_xmax = 0.5 * surfaceHeight;
		m_ymax = 0.5 * surfaceWidth;

		std::function< Point3D (Point3D, std::vector<double> ) > transformFunction = [] (Point3D pInLocal, std::vector<double> /*parameters*/  )
				{
					return ( Point3D( pInLocal.x, pInLocal.z, 0.0 ) );
				};

		std::vector<double> emptyList;
		FluxAnalysis( activeSideID, transformFunction, emptyList, worldToObject );


	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatDisk" ) )
	{
		//FluxAnalysisFlatDisk( instanceNode );
		int activeSideID = 1;
		if( sidesCombo->currentText() == QLatin1String( "BACK" ) )
			activeSideID = 0;

		trt::TONATIUH_REAL* radiusField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "radius" ) );
		double radius = radiusField->getValue();

		m_xmin = -radius;
		m_ymin = -radius;
		m_xmax = radius;
		m_ymax = radius;


		std::function< Point3D (Point3D, std::vector<double>) > transformFunction = [] (Point3D pInLocal, std::vector<double> /*parameters*/  )
				{
					return ( Point3D( pInLocal.x, pInLocal.z, 0.0 ) );
				};


		std::vector<double> emptyList;
		FluxAnalysis( activeSideID, transformFunction, emptyList, worldToObject );

	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeCylinder" ) )
	{

		//FluxAnalysisCylinder( instanceNode );

		trt::TONATIUH_REAL* radiusField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "radius" ) );
		double radius = radiusField->getValue();

		trt::TONATIUH_REAL* lengthField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "length" ) );
		double length = lengthField->getValue();

		trt::TONATIUH_REAL* phiMaxField = static_cast< trt::TONATIUH_REAL* > ( shape->getField( "phiMax" ) );
		double phiMax = phiMaxField->getValue();

		int activeSideID = 1;
		if( sidesCombo->currentText() == QLatin1String( "INSIDE" ) )
			activeSideID = 0;


		m_xmin = 0.0;
		m_ymin = 0.0;
		m_xmax = phiMax  * radius;
		m_ymax = length;

		/*
		Point3D photonLocalCoord = worldToObject( photon->pos );
					double phi  = atan2( photonLocalCoord.y, photonLocalCoord.x );
					if( phi < 0.0 ) phi += 2* gc::Pi;
					double arcLength = phi * radius;
					//std::cout<<"m_xmax: "<<m_xmax <<"arcLength: "<<arcLength<<std::endl;
					int xbin = floor( ( arcLength - m_xmin)/(m_xmax - m_xmin) * m_widthDivisions) ;
					int ybin = floor( (photonLocalCoord.z - m_ymin)/(m_ymax - m_ymin) * m_heightDivisions);
*/

		std::function< Point3D (Point3D, std::vector<double> ) > transformFunction = [] (Point3D pInLocal, std::vector<double> parameters )
				{
					double radius = parameters[0];
					double phi  = atan2( pInLocal.y, pInLocal.x );
					if( phi < 0.0 ) phi += 2* gc::Pi;
					double arcLength = phi * radius;

					return ( Point3D( arcLength, pInLocal.z, 0.0 ) );
				};

		std::vector<double> parametersList;
		parametersList.push_back( radius );

		FluxAnalysis( activeSideID, transformFunction, parametersList, worldToObject );

	}

}


/*!
 * Updates the sector plots
 */
void FluxAnalysisDialog::UpdateSectorPlots()
{

	if( !m_pPhotonMap )	return;

	QCPItemLine* tickVLine  = ( QCPItemLine* ) contourPlotWidget->item( 0 );
	QPointF pointVStart = tickVLine->start->coords();
	tickVLine->start->setCoords( hSectorXCoordSpin->value(), pointVStart.y() );
	QPointF pointVEnd= tickVLine->end->coords();
	tickVLine->end->setCoords( hSectorXCoordSpin->value(), pointVEnd.y() );
	tickVLine->setPen( QPen( QColor( 137, 140, 140), 1 ) );

	QCPItemLine* tickHLine  = ( QCPItemLine* ) contourPlotWidget->item( 1 );
	QPointF pointHStart = tickHLine->start->coords();
	tickHLine->start->setCoords( pointHStart.x(), hSectorYCoordSpin->value());
	QPointF pointHEnd = tickHLine->end->coords();
	tickHLine->end->setCoords( pointHEnd.x(), hSectorYCoordSpin->value() );
	tickHLine->setPen( QPen( QColor( 137, 140, 140), 1 ) );

	contourPlotWidget->replot();


	//Sectors plots

	//Delete previous colormap, scale
	verticalSectorPlot->clearPlottables();
	horizontaSectorPlot->clearPlottables();


	double xCoordSector = hSectorXCoordSpin->value();
	double yCoordSector = hSectorYCoordSpin->value();

	double widthCell = (m_xmax-m_xmin) / m_widthDivisions;
	double heightCell = (m_ymax-m_ymin) / m_heightDivisions;
	double areaCell = widthCell * heightCell;

	int xbin1Index = floor( ( xCoordSector - m_xmin)/(m_xmax-m_xmin) * m_widthDivisions);
	if( xbin1Index >= m_widthDivisions  ) xbin1Index = m_widthDivisions-1;
	int ybin1Index = floor( (yCoordSector - m_ymin)/(m_ymax-m_ymin) * m_heightDivisions);
	if( ybin1Index >= m_heightDivisions  ) ybin1Index = m_heightDivisions-1;

	// generate some data:
	QVector<double> verticalXValues(m_heightDivisions), verticalYValues(m_heightDivisions); // initialize with entries 0..100
	for( int i = 0; i < m_heightDivisions; ++i)
	{
		verticalXValues[i] = m_ymin + ( i + 0.5 ) * heightCell;
		verticalYValues[i] = m_photonCounts[i][xbin1Index] * m_wPhoton / areaCell;

	}

	QVector<double> horizontalXValues(m_widthDivisions), horizontalYValues(m_widthDivisions); // initialize with entries 0..100
	for( int i = 0; i < m_widthDivisions; ++i)
	{
		horizontalXValues[i] = m_xmin + ( i + 0.5 ) * widthCell;
		horizontalYValues[i] = m_photonCounts[ybin1Index][i] * m_wPhoton / areaCell;

	}

	// create graph and assign data to it:
	verticalSectorPlot->addGraph();
	verticalSectorPlot->graph(0)->setData(verticalXValues, verticalYValues);
	// set axes ranges, so we see all data:
	verticalSectorPlot->xAxis->setRange( m_ymin, m_ymax );
	verticalSectorPlot->yAxis->setRange(0, 1.2 * m_maximumFlux  );
	//verticalSectorPlot->rescaleAxes();
	verticalSectorPlot->replot();



	// create graph and assign data to it:
	horizontaSectorPlot->addGraph();
	horizontaSectorPlot->graph(0)->setData(horizontalXValues, horizontalYValues);
	// set axes ranges, so we see all data:
	horizontaSectorPlot->xAxis->setRange( m_xmin, m_xmax );
	horizontaSectorPlot->yAxis->setRange(0, 1.2 *m_maximumFlux);
	//horizontaSectorPlot->rescaleAxes( true );
	horizontaSectorPlot->replot();


}

/*!
 * Clear current results and simulation.
 */
void FluxAnalysisDialog::ClearCurrentAnalysis()
{

	if( m_pPhotonMap ) 	m_pPhotonMap->EndStore( -1 );
	delete m_pPhotonMap;
	m_pPhotonMap = 0;
	m_tracedRays = 0;
	m_wPhoton = 0;
	appendCheck->setEnabled( false );

	//Delete previous colormap, scale
	contourPlotWidget->clearPlottables();
	contourPlotWidget->replot();

	totalPowerValue->setText( QString::number(  0.0 )  );

	minimumFluxValue->setText(QString::number( 0.0 )  );
	averageFluxValue->setText(QString::number( 0.0 )  );
	maximumFluxValue->setText(QString::number( 0.0 ) );

	maxCoordinatesValue->setText( QLatin1String( " ; ") );
	errorValue->setText(QString::number( 0.0 ) );
	uniformityValue->setText(QString::number( 0.0 ) );
	centroidValue->setText( QLatin1String( " ; ") );

}


void FluxAnalysisDialog::FluxAnalysis( int activeSideID, std::function< Point3D( Point3D, std::vector<double>  ) > transformFunction, std::vector<double> parametersList, Transform wTO )
{
	m_widthDivisions = gridWidthSpin->value();
	m_heightDivisions = gridHeightSpin->value();


	m_photonCounts = new int*[m_heightDivisions];
	for( int h = 0; h < m_heightDivisions; h++ )
	{
		m_photonCounts[h] = new int[m_widthDivisions];
		for( int w = 0; w < m_widthDivisions; w++ )
			m_photonCounts[h][w] = 0;
	}


	int widthDivisionsError = m_widthDivisions-2;
	int heightDivisionsError = m_heightDivisions-2;



	std::vector< std::vector<int> > photonCountsError;
	photonCountsError.resize(heightDivisionsError);
	for(int i = 0 ; i < heightDivisionsError ; ++i)
	{
		photonCountsError[i].resize(widthDivisionsError);
		for(int j=0; j < widthDivisionsError; j++)
			photonCountsError[i][j]=0;
	}


	double maximumPhotons = 0;
	int maximumPhotonsXCoord = 0;
	int maximumPhotonsYCoord = 0;
	double maximumPhotonsError = 0;

	std::vector< Photon* > photonList = m_pPhotonMap->GetAllPhotons();
	for( unsigned int p = 0; p < photonList.size(); p++ )
	{
		Photon* photon = photonList[p];
		if( photon->side == activeSideID )
		{
			Point3D photonLocalCoord = transformFunction( wTO( photon->pos ), parametersList );

			int xbin = floor( ( photonLocalCoord.x - m_xmin)/(m_xmax - m_xmin) * m_widthDivisions);
			int ybin = floor( (photonLocalCoord.y - m_ymin)/(m_ymax - m_ymin) * m_heightDivisions);
			m_photonCounts[ybin][xbin] += 1;
			if( maximumPhotons < m_photonCounts[ybin][xbin] )
			{
				maximumPhotons = m_photonCounts[ybin][xbin];
				maximumPhotonsXCoord = xbin;
				maximumPhotonsYCoord = ybin;
			}


			int xbinE = floor( ( photonLocalCoord.x - m_xmin)/(m_xmax-m_xmin) * widthDivisionsError) ;
			int ybinE = floor( (photonLocalCoord.y - m_ymin)/(m_ymax-m_ymin) * heightDivisionsError);
			photonCountsError[ybinE][xbinE] += 1;
			if( maximumPhotonsError < photonCountsError[ybinE][xbinE] )
				maximumPhotonsError = photonCountsError[ybinE][xbinE];


		}
	}

	double widthCell = (m_xmax-m_xmin) / m_widthDivisions;
	double heightCell = (m_ymax-m_ymin) / m_heightDivisions;
	double areaCell = widthCell * heightCell;
	m_maximumFlux = ( maximumPhotons * m_wPhoton) / areaCell;

	//Delete previous colormap, scale
	contourPlotWidget->clearPlottables();
	contourPlotWidget->clearItems();


	// Create a QCPColorMap object to draw flux distribution
	QCPColorMap* colorMap = new QCPColorMap(contourPlotWidget->xAxis, contourPlotWidget->yAxis);
	contourPlotWidget->addPlottable(colorMap);

	int nx = m_widthDivisions;
	int ny = m_heightDivisions;
	colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
	colorMap->data()->setRange(QCPRange(m_xmin, m_xmax), QCPRange(m_ymin, m_ymax)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions

	// assign flux data
	//Calculate total flux, minimum flux, average flux
	double totalFlux = 0.0;
	double minimumFlux = gc::Infinity;
	double gravityX = 0.0;
	double gravityY = 0.0;
	for (int xIndex=0; xIndex< nx; ++xIndex)
	{
		for (int yIndex=0; yIndex<ny; ++yIndex)
		{
			double cellFlux = m_photonCounts[yIndex][xIndex] * m_wPhoton / areaCell;
			colorMap->data()->setCell(xIndex, yIndex, cellFlux );
			totalFlux += cellFlux;
			if( minimumFlux > cellFlux )	minimumFlux = cellFlux;

			gravityX += cellFlux * ( m_xmin + ( xIndex+ 0.5 ) * widthCell  );
			gravityY += cellFlux * ( m_ymin + ( yIndex+ 0.5 ) * heightCell  );

		}
	}
	double averageFlux = totalFlux / (m_widthDivisions * m_heightDivisions );
	gravityX /= totalFlux;
	gravityY /= totalFlux;

	// Create a vertical and horizontal line for sectors
	hSectorXCoordSpin->setMinimum( m_xmin );
	hSectorXCoordSpin->setMaximum( m_xmax );
	hSectorXCoordSpin->setSingleStep( (m_xmax-m_xmin ) /10 );
	hSectorXCoordSpin->setValue( 0.0 );

	hSectorYCoordSpin->setMinimum( m_ymin );
	hSectorYCoordSpin->setMaximum( m_ymax );
	hSectorYCoordSpin->setSingleStep( (m_ymax-m_ymin ) /10 );
	hSectorYCoordSpin->setValue( 0.0 );

	QCPItemLine* tickVLine  =new QCPItemLine( contourPlotWidget );
	contourPlotWidget->addItem( tickVLine );
	tickVLine->start->setCoords( 0, m_ymin - 1);
	tickVLine->end->setCoords( 0, m_ymax + 1);
	tickVLine->setPen( QPen( QColor( 137, 140, 140), 1 ) );

	QCPItemLine* tickHLine = new QCPItemLine( contourPlotWidget );
	contourPlotWidget->addItem( tickHLine );
	tickHLine->start->setCoords(m_xmin -1 ,  0 );
	tickHLine->end->setCoords( m_xmax + 1, 0 );
	tickHLine->setPen( QPen( QColor(137, 140, 140), 1));


	// add a color scale:
	QCPColorScale* colorScale = new QCPColorScale(contourPlotWidget);
	contourPlotWidget->plotLayout()->addElement(1, 1, colorScale); // add it to the right of the main axis rect

	colorScale->setType( QCPAxis::atRight ); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
	colorMap->setColorScale( colorScale ); // associate the color map with the color scale
	//colorScale->axis()->setLabel("Flux");


	// set the  contour plot color
	colorMap->setGradient(QCPColorGradient::gpSpectrum);

	// rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
	colorMap->rescaleDataRange();

	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
	QCPMarginGroup* marginGroup = new QCPMarginGroup(contourPlotWidget);
	contourPlotWidget->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
	colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);


	// rescale axes and update plot view
	contourPlotWidget->rescaleAxes();
	contourPlotWidget->replot();



	//Sectors plots

	UpdateSectorPlots();



	//Statistical data
	totalPowerValue->setText( QString::number(  photonList.size() * m_wPhoton)  );

	minimumFluxValue->setText(QString::number( minimumFlux )  );
	averageFluxValue->setText(QString::number( averageFlux )  );
	maximumFluxValue->setText(QString::number( m_maximumFlux ) );

	maxCoordinatesValue->setText( QString( QLatin1String( "%1 ; %2")).arg( QString::number( m_xmin + ( maximumPhotonsXCoord + 0.5 ) * widthCell ),
			QString::number( m_ymin + ( maximumPhotonsYCoord + 0.5 ) * heightCell  ) ) );

	double maximumFluxError = ( maximumPhotonsError * m_wPhoton) / ( ( (m_xmax-m_xmin) / widthDivisionsError ) * ( (m_ymax-m_ymin) / heightDivisionsError ) ) ;

	errorValue->setText( QString::number(  fabs( m_maximumFlux - maximumFluxError )/ m_maximumFlux )  );

	double E=0;
	for (int xIndex=0; xIndex< nx; ++xIndex)
	{
		for (int yIndex=0; yIndex<ny; ++yIndex)
		{
			double cellFlux = m_photonCounts[yIndex][xIndex] * m_wPhoton / areaCell;
			E += ( cellFlux - averageFlux )*(cellFlux - averageFlux);

		}
	}
	double standarDeviation = sqrt( E / (m_widthDivisions * m_heightDivisions ) );
	uniformityValue->setText(QString::number( standarDeviation /averageFlux )  );
	centroidValue->setText( QString( QLatin1String( "%1 ; %2")).arg( QString::number( gravityX ), QString::number( gravityY ) ) );


}

/*!
 *
 */
void FluxAnalysisDialog::UpdateSurfaceSides( QString selectedSurfaceURL )
{

	if(m_currentSurfaceURL == selectedSurfaceURL )	return;

	ClearCurrentAnalysis();

	m_currentSurfaceURL = selectedSurfaceURL;
	sidesCombo->clear();

	QModelIndex nodeIndex = m_pCurrentSceneModel->IndexFromNodeUrl( m_currentSurfaceURL );
	if( !nodeIndex.isValid()  )		return;

	InstanceNode* instanceNode = m_pCurrentSceneModel->NodeFromIndex( nodeIndex );
	if( !instanceNode || instanceNode == 0 )	return;

	TShapeKit* shapeKit = static_cast< TShapeKit* > ( instanceNode->GetNode() );
	if( !shapeKit || shapeKit == 0 )	return;

	TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );
	if( !shape || shape == 0 )	return;

	if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatRectangle" ) )
	{
		sidesCombo->addItem( QLatin1String( "FRONT" ) );
		sidesCombo->addItem( QLatin1String( "BACK" ) );
	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeFlatDisk" ) )
	{
		sidesCombo->addItem( QLatin1String( "FRONT" ) );
		sidesCombo->addItem( QLatin1String( "BACK" ) );
	}
	else if( shape->getTypeId().getName().getString() == QLatin1String( "ShapeCylinder" ) )
	{
		sidesCombo->addItem( QLatin1String( "INSIDE" ) );
		sidesCombo->addItem( QLatin1String( "OUTSIDE" ) );
	}

}
