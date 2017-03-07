/*
 * FluxAnalysisDialog.cpp
 *
 *  Created on: 08/10/2015
 *      Author: amutuberria
 */

#include <cmath>

#include <QFileDialog>
#include <QFutureWatcher>
#include <QIntValidator>
#include <QMessageBox>
#include <QMutex>
#include <QPair>
#include <QProgressDialog>
#include <QtConcurrentMap>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoTransform.h>

#include "FluxAnalysis.h"
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
		int sunWidthDivisions, int sunHeightDivisions,
		RandomDeviate* randomDeviate,  QWidget* parent  )
:QDialog( parent),
 m_currentSurfaceURL( "" ),
 m_pCurrentSceneModel( &currentSceneModel ),
 m_pGridWidthVal( 0 ),
 m_pGridHeightVal( 0 ),
 m_pNOfRays ( 0 ),
 m_fluxLabelString( QString( "Flux((unit power)/(unit length)^2)"))
{
	m_fluxAnalysis = new FluxAnalysis( currentScene, currentSceneModel, rootSeparatorInstance, sunWidthDivisions, sunHeightDivisions, randomDeviate );
	setupUi( this );

	QSize windowSize = size();
	QList<int> sizes;
    sizes<<windowSize.width() * 0.3 << windowSize.width() * 0.7;
    splitter->setSizes( sizes );

	QSize resultsWidgetSize = resultsWidget->size();
	int resultsWidgetSizeHeight = resultsWidgetSize.height();

	contourPlotWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	sectorsWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.34 );
	statisticalWidget->resize( resultsWidgetSize.width(), resultsWidgetSizeHeight * 0.25 );

	m_pGridWidthVal = new QIntValidator( 2, 999999999, this );
	gridWidthLine->setValidator( m_pGridWidthVal );
	m_pGridHeightVal = new QIntValidator (2, 999999999, this );
	gridHeightLine->setValidator( m_pGridHeightVal );

	m_pNOfRays = new QIntValidator( 1, 999999999, this );

	connect( selectButton, SIGNAL( clicked() ), this, SLOT( SelectSurface() ) );
	connect( sidesCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeCurrentSurfaceSide() ) );
	connect( surfaceEdit, SIGNAL( editingFinished() ), this, SLOT( ChangeCurrentSurface() ) );
	connect( runButton, SIGNAL( clicked()  ), this, SLOT( Run() ) );
	connect( hSectorXCoordSpin, SIGNAL( valueChanged( double ) ), this, SLOT( UpdateSectorPlotSlot() ) );
	connect( hSectorYCoordSpin, SIGNAL( valueChanged( double ) ), this, SLOT( UpdateSectorPlotSlot() ) );
	connect( gridWidthLine, SIGNAL( editingFinished() ), this, SLOT( UpdateAnalysis() ) );
	connect( gridHeightLine, SIGNAL( editingFinished( ) ), this, SLOT( UpdateAnalysis() ) );
	connect( lengthUnitLine, SIGNAL( editingFinished( ) ), this, SLOT( UpdateLabelsUnits() ) );
	connect( powerUnitLine, SIGNAL( editingFinished( ) ), this, SLOT( UpdateLabelsUnits() ) );
	connect( selectFileButton, SIGNAL( clicked() ), this, SLOT( SelectExportFile() ) );
	connect( exportButton, SIGNAL( clicked() ) , this, SLOT( ExportData() ) );
	connect( storeTypeCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SaveCoordsExport() ) );

	// configure axis rect:
	contourPlotWidget->setInteractions( QCP::iRangeDrag|QCP::iRangeZoom ); // this will also allow rescaling the color scale by dragging/zooming
	contourPlotWidget->axisRect()->setupFullAxesBox( true );

	contourPlotWidget->plotLayout()->insertRow( 0 );
	contourPlotWidget->plotLayout()->addElement( 0, 0, new QCPPlotTitle( contourPlotWidget, "Incident Flux Distribution" ) );
	contourPlotWidget->xAxis->setLabel( "X (unit length)" );
	contourPlotWidget->yAxis->setLabel( "Y (unit length)" );

	horizontaSectorPlot->plotLayout()->insertRow( 0 );
	horizontaSectorPlot->plotLayout()->addElement( 0, 0, new QCPPlotTitle(horizontaSectorPlot, "Horizontal Sector" ) );
	// give the axes some labels:
	horizontaSectorPlot->xAxis->setLabel( "Y (unit length)" );
	horizontaSectorPlot->yAxis->setLabel( "Flux ( (unit power) / (unit length)^2 )" );

	verticalSectorPlot->plotLayout()->insertRow( 0 );
	verticalSectorPlot->plotLayout()->addElement( 0, 0, new QCPPlotTitle( verticalSectorPlot, "Vertical Sector" ) );
	// give the axes some labels:
	verticalSectorPlot->xAxis->setLabel( "X (unit length)" );
	verticalSectorPlot->yAxis->setLabel( "Flux ( (unit power) / (unit length)^2 )" );

	// give the y-axes an initial range:
	verticalSectorPlot->yAxis->setRange( 0, 1.08 );
	horizontaSectorPlot->yAxis->setRange( 0, 1.08 );
}

/*!
 * Destroys dialog object
 */
FluxAnalysisDialog::~FluxAnalysisDialog()
{
	delete m_fluxAnalysis;
	delete m_pGridWidthVal;
	delete m_pGridHeightVal;
	delete m_pNOfRays;
}

/*!
 * Resizes results widget elements sizes when the dialog windows size is changed.
 */
void  FluxAnalysisDialog::resizeEvent( QResizeEvent* /*event*/ )
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
	if( !selectedSurfaceURL.isEmpty() && ( selectedSurfaceURL != m_currentSurfaceURL ) )
	{
		QString surfaceType = m_fluxAnalysis->GetSurfaceType( selectedSurfaceURL );
		if( !surfaceType.isEmpty() && ( surfaceType == "ShapeCylinder" || surfaceType == "ShapeFlatDisk" || surfaceType == "ShapeFlatRectangle" ) )
		{
			m_fluxAnalysis->clearPhotonMap();
			appendCheck->setChecked( false );
			appendCheck->setEnabled( false );
			ClearCurrentAnalysis();
			surfaceEdit->setText( selectedSurfaceURL );
			m_currentSurfaceURL = selectedSurfaceURL;
			UpdateSurfaceSides( m_currentSurfaceURL );
		}
		else QMessageBox::warning( this, tr( "Tonatiuh" ),
				tr( "The surface url defined is not valid." ) );
	}
	else surfaceEdit->setText( m_currentSurfaceURL );
}

/*
 * Clear the current analysis if the surface side is modified
 */
void FluxAnalysisDialog::ChangeCurrentSurfaceSide()
{
	m_fluxAnalysis->clearPhotonMap();
	appendCheck->setChecked( false );
	appendCheck->setEnabled( false );
	ClearCurrentAnalysis();
}

/*
 * Set the coords check box according to the store type
 */
void FluxAnalysisDialog::SaveCoordsExport()
{
	if ( storeTypeCombo->currentText() == "IMAGE.PNG" || storeTypeCombo->currentText() == "IMAGE.JPG" )
		{
			saveCoordsCheckBox->setChecked( false );
			saveCoordsCheckBox->setDisabled( true );
		}
	else saveCoordsCheckBox->setEnabled( true );
}

/*
 * Exports the flux distribution
 */
void FluxAnalysisDialog::ExportData()
{
	int** photonCounts = m_fluxAnalysis->photonCountsValue();
	if( !photonCounts || photonCounts == 0 )
	{
		QString message = QString( tr( "Nothing available to export, first run the simulation" ) );
		QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
		return;
	}

	if( fileDirEdit->text().isEmpty() )
	{
		QString message = QString( tr( "Directory not valid" ) );
		QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
		return;
	}

	if( fileNameEdit->text().isEmpty() )
	{
		QString message = QString( tr( "File name not valid" ) );
		QMessageBox::warning( this,  QLatin1String( "Tonatiuh" ), message );
		return;
	}

	QString exportDirectory( fileDirEdit->text() );
	QString storeType = storeTypeCombo->currentText();
	QString exportFileName = fileNameEdit->text();

	QFileInfo exportFileInfo( exportFileName );
	if( storeType == QLatin1String( "ASCII" ) )
	{
		if( saveCoordsCheckBox->isChecked() )
			m_fluxAnalysis->ExportAnalysis( exportDirectory, exportFileName, true );
		else
			m_fluxAnalysis->ExportAnalysis( exportDirectory, exportFileName, false );

	}
	else if( storeType == QLatin1String( "IMAGE.JPG" ) )
	{
		if( exportFileInfo.completeSuffix().compare( "jpg" ) )	exportFileName.append( ".jpg" );

		QFile exportFile( exportDirectory + "/" + exportFileName );
		exportFile.open( QIODevice::WriteOnly );
		contourPlotWidget->saveJpg( exportFile.fileName(), 0, 0, 1.0, -1 ); //(QString &  fileName, int  width = 0, int  height = 0, double  scale = 1.0, int  quality = -1  )
		exportFile.close();
	}
	else if( storeType == "IMAGE.PNG" )
	{
		if( exportFileInfo.completeSuffix().compare( "png" ) )	exportFileName.append( ".png" );
		QFile exportFile( exportDirectory + "/" + exportFileName );
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
void FluxAnalysisDialog::Run()
{
	QDateTime startTime = QDateTime::currentDateTime();

	//Check the surface
	if( m_currentSurfaceURL.isEmpty() )
	{
		QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
			tr( "Select a valid surface." ) );
		return;
	}

	//Check the number of rays
	QString nOfRays = nRaysLine->text();
	int pos = 0;
	if( m_pNOfRays->validate( nOfRays, pos ) != QValidator::Acceptable )
	{
		QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
			tr( "The number of rays to trace must be a positive value." ) );
		return;
	}

	//Check the grid division
	QString  widthDivisions = gridWidthLine->text();
	QValidator::State  widthState = m_pGridWidthVal->validate( widthDivisions, pos );

	QString  heightDivisions = gridHeightLine->text();
	QValidator::State  heightSate = m_pGridHeightVal->validate( heightDivisions, pos );

	if( ( widthState != QValidator::Acceptable ) || ( heightSate != QValidator::Acceptable ) )
	{
		QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
			tr( "The gird divisions size must be greater than 2." ) );
		return;
	}

	QString surfaceSide = sidesCombo->currentText();
	bool increasePhotonMap = ( appendCheck->isEnabled() && appendCheck->isChecked() );
	m_fluxAnalysis->RunFluxAnalysis( m_currentSurfaceURL, surfaceSide, nOfRays.toInt() , increasePhotonMap, heightDivisions.toInt(), widthDivisions.toInt() );

	UpdateAnalysis();
	appendCheck->setEnabled( true );

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;
}

/*
 * Calculate flux distribution and statistics
 */
void FluxAnalysisDialog::UpdateAnalysis()
{
	int pos = 0;
	QString  withValue = gridWidthLine->text();
	QValidator::State  widthState = m_pGridWidthVal->validate( withValue, pos );

	QString  heightValue = gridHeightLine->text();
	QValidator::State  heightSate = m_pGridHeightVal->validate( heightValue, pos );

	if( ( widthState != QValidator::Acceptable ) || ( heightSate != QValidator::Acceptable ) )
	{
		QMessageBox::warning( this, QLatin1String( "Tonatiuh" ),
			tr( "The gird divisions size must be greater than 2." ) );
		return;
	}

	m_fluxAnalysis->UpdatePhotonCounts( heightValue.toInt(), withValue.toInt() );

	int** photonCounts = m_fluxAnalysis->photonCountsValue();
	if( !photonCounts || photonCounts == 0 ) return;

	ClearCurrentAnalysis();

	double xmin = m_fluxAnalysis->xminValue();
	double ymin = m_fluxAnalysis->yminValue();
	double xmax = m_fluxAnalysis->xmaxValue();
	double ymax = m_fluxAnalysis->ymaxValue();
	double wPhoton = m_fluxAnalysis->wPhotonValue();
	double totalPower = m_fluxAnalysis->totalPowerValue();
	int widthDivisions = withValue.toInt();
	int heightDivisions = heightValue.toInt();
	double widthCell = ( xmax - xmin ) / widthDivisions;
	double heightCell = ( ymax - ymin ) / heightDivisions;
	double areaCell = widthCell * heightCell;
	double maximumFlux = ( m_fluxAnalysis->maximumPhotonsValue() * wPhoton) / areaCell;
	double minimumFlux = 0;
	double totalFlux = totalPower / areaCell;
	double averageFlux = totalFlux / ( widthDivisions * heightDivisions );
	double maxXCoord = xmin + ( m_fluxAnalysis->maximumPhotonsXCoordValue() + 0.5 ) * widthCell;
	double maxYCoord = ymin + ( m_fluxAnalysis->maximumPhotonsYCoordValue() + 0.5 ) * heightCell;
	double maximumFluxError = ( m_fluxAnalysis->maximumPhotonsErrorValue() * wPhoton) / ( ( ( xmax - xmin) / ( widthDivisions - 1 ) ) * ( ( ymax - ymin) / ( heightDivisions - 1 ) ) );
	double error = fabs( maximumFlux - maximumFluxError ) / maximumFlux;
	double gravityX = 0.0;
	double gravityY = 0.0;
	double E = 0;

	for ( int xIndex=0; xIndex < widthDivisions; ++xIndex )
	{
		for ( int yIndex=0; yIndex < heightDivisions; ++yIndex )
		{
			double cellFlux = photonCounts[yIndex][xIndex] * wPhoton / areaCell;
			if( minimumFlux > cellFlux )	minimumFlux = cellFlux;

			gravityX += cellFlux * ( xmin + ( xIndex + 0.5 ) * widthCell  );
			gravityY += cellFlux * ( ymin + ( yIndex + 0.5 ) * heightCell  );
			E += ( cellFlux - averageFlux ) * ( cellFlux - averageFlux );
		}
	}

	double standarDeviation = sqrt( E / ( widthDivisions * heightDivisions )  );
	double uniformity = standarDeviation / averageFlux;
	gravityX /= totalFlux;
	gravityY /= totalFlux;

	UpdateStatistics( totalPower, minimumFlux, averageFlux, maximumFlux, maxXCoord, maxYCoord, error, uniformity, gravityX, gravityY );
	UpdateFluxMapPlot( photonCounts, wPhoton, widthDivisions, heightDivisions, xmin, ymin, xmax, ymax );
	CreateSectorPlots( xmin, ymin, xmax, ymax );
	UpdateSectorPlots( photonCounts, wPhoton, widthDivisions, heightDivisions, xmin, ymin, xmax, ymax, maximumFlux );
}

/*
 * Updates the statistics
 */
void FluxAnalysisDialog::UpdateStatistics( double totalPower, double minimumFlux, double averageFlux, double maximumFlux,
		double maxXCoord, double maxYCoord, double error, double uniformity, double gravityX, double gravityY )
{
	totalPowerValue->setText( QString::number( totalPower ) );
	minimumFluxValue->setText( QString::number( minimumFlux ) );
	averageFluxValue->setText( QString::number( averageFlux ) );
	maximumFluxValue->setText( QString::number( maximumFlux ) );
	maxCoordinatesValue->setText( QString( QLatin1String( "%1 ; %2")).arg( QString::number( maxXCoord ), QString::number( maxYCoord ) ) );
	errorValue->setText( QString::number( error ) );
	uniformityValue->setText(QString::number( uniformity ) );
	centroidValue->setText( QString( QLatin1String( "%1 ; %2")).arg( QString::number( gravityX ), QString::number( gravityY ) ) );
}

/*
 * Updates the flux map plot
 */
void FluxAnalysisDialog::UpdateFluxMapPlot( int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax )
{
	//Delete previous colormap, scale
	contourPlotWidget->clearPlottables();
	contourPlotWidget->clearItems();

	//see how many elements there are
	int elementCount = contourPlotWidget->plotLayout()->elementCount();
	//loop over the elements
	for(int i = 0; i < elementCount; i++)
	{
	   //test to see if any of the layout elements are of QCPColorScale type
	    if( qobject_cast<QCPColorScale*>( contourPlotWidget->plotLayout()->elementAt( i ) ) )
	    	contourPlotWidget->plotLayout()->removeAt( i );
	    //collapse the empty elements
	    contourPlotWidget->plotLayout()->simplify();
	}

	// Create a QCPColorMap object to draw flux distribution
	QCPColorMap* colorMap = new QCPColorMap( contourPlotWidget->xAxis, contourPlotWidget->yAxis );
	contourPlotWidget->addPlottable( colorMap );

	colorMap->data()->setSize( widthDivisions, heightDivisions ); // we want the color map to have widthDivisions * heightDivisions data points
	colorMap->data()->setRange( QCPRange( xmin, xmax ), QCPRange( ymin, ymax ) ); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions

	//Assign flux data
	double widthCell = ( xmax - xmin ) / widthDivisions;
	double heightCell = ( ymax - ymin ) / heightDivisions;
	double areaCell = widthCell * heightCell;
	for ( int xIndex=0; xIndex < widthDivisions; ++xIndex )
	{
		for ( int yIndex=0; yIndex < heightDivisions; ++yIndex )
		{
			double cellFlux = photonCounts[yIndex][xIndex] * wPhoton / areaCell;
			colorMap->data()->setCell( xIndex, yIndex, cellFlux );
		}
	}

	// add a color scale:
	QCPColorScale* colorScale = new QCPColorScale( contourPlotWidget );
	contourPlotWidget->plotLayout()->addElement( 1, 1, colorScale ); // add it to the right of the main axis rect

	colorScale->setType( QCPAxis::atRight ); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
	colorMap->setColorScale( colorScale ); // associate the color map with the color scale
	colorScale->axis()->setLabel( m_fluxLabelString );

	// set the  contour plot color
	colorMap->setGradient( QCPColorGradient::gpSpectrum );

	// rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
	colorMap->rescaleDataRange();

	// forbid to drag and zoom the colorscale
	colorScale->setRangeZoom( false );
	colorScale->setRangeDrag( false );

	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
	QCPMarginGroup* marginGroup = new QCPMarginGroup( contourPlotWidget );
	contourPlotWidget->axisRect()->setMarginGroup( QCP::msBottom|QCP::msTop, marginGroup );
	colorScale->setMarginGroup( QCP::msBottom|QCP::msTop, marginGroup );

	// rescale axes and update plot view
	contourPlotWidget->rescaleAxes();
	contourPlotWidget->replot();
}

/*
 * Create sector plots
 */
void FluxAnalysisDialog::CreateSectorPlots( double xmin, double ymin, double xmax, double ymax )
{
	// Create a vertical and horizontal line for sectors
	QCPItemLine* tickVLine  = new  QCPItemLine( contourPlotWidget );
	hSectorXCoordSpin->setMinimum( xmin );
	hSectorXCoordSpin->setMaximum( xmax );
	hSectorXCoordSpin->setSingleStep( ( xmax - xmin ) / 10 );
	contourPlotWidget->addItem( tickVLine );

	tickVLine->start->setCoords( 0, ymin - 1 );
	tickVLine->end->setCoords( 0, ymax + 1 );
	tickVLine->setPen( QPen( QColor( 137, 140, 140 ), 1 ) );

	QCPItemLine* tickHLine = new QCPItemLine( contourPlotWidget );
	hSectorYCoordSpin->setMinimum( ymin );
	hSectorYCoordSpin->setMaximum( ymax );
	hSectorYCoordSpin->setSingleStep( (  ymax - ymin ) / 10 );
	contourPlotWidget->addItem( tickHLine );

	tickHLine->start->setCoords( xmin -1 ,  0 );
	tickHLine->end->setCoords( xmax + 1, 0 );
	tickHLine->setPen( QPen( QColor( 137, 140, 140 ), 1) );
}

/*
 * Updates the sector plots
 */
void FluxAnalysisDialog::UpdateSectorPlots( int** photonCounts, double wPhoton, int widthDivisions, int heightDivisions, double xmin, double ymin, double xmax, double ymax, double maximumFlux )
{
	QCPItemLine* tickVLine  = ( QCPItemLine* ) contourPlotWidget->item( 0 );
	QPointF pointVStart = tickVLine->start->coords();
	tickVLine->start->setCoords( hSectorXCoordSpin->value(), pointVStart.y() );
	QPointF pointVEnd= tickVLine->end->coords();
	tickVLine->end->setCoords( hSectorXCoordSpin->value(), pointVEnd.y() );
	tickVLine->setPen( QPen( QColor( 137, 140, 140 ), 1 ) );

	QCPItemLine* tickHLine  = ( QCPItemLine* ) contourPlotWidget->item( 1 );
	QPointF pointHStart = tickHLine->start->coords();
	tickHLine->start->setCoords( pointHStart.x(), hSectorYCoordSpin->value() );
	QPointF pointHEnd = tickHLine->end->coords();
	tickHLine->end->setCoords( pointHEnd.x(), hSectorYCoordSpin->value() );
	tickHLine->setPen( QPen( QColor( 137, 140, 140 ), 1 ) );

	contourPlotWidget->replot();

	//Delete previous plots
	verticalSectorPlot->clearPlottables();
	horizontaSectorPlot->clearPlottables();

	double xCoordSector = hSectorXCoordSpin->value();
	double yCoordSector = hSectorYCoordSpin->value();

	double widthCell = ( xmax - xmin ) / widthDivisions;
	double heightCell = ( ymax - ymin ) / heightDivisions;
	double areaCell = widthCell * heightCell;

	int xbin1Index = floor( ( xCoordSector - xmin ) / ( xmax - xmin ) * widthDivisions );
	if( xbin1Index >= widthDivisions ) xbin1Index = widthDivisions - 1;
	int ybin1Index = floor( ( yCoordSector - ymin ) / ( ymax - ymin ) * heightDivisions );
	if( ybin1Index >= heightDivisions ) ybin1Index = heightDivisions - 1;

	// generate some data:
	QVector<double> verticalXValues( heightDivisions ), verticalYValues( heightDivisions ); // initialize with entries 0..100
	for( int i = 0; i < heightDivisions; ++i)
	{
		verticalXValues[i] = ymin + ( i + 0.5 ) * heightCell;
		verticalYValues[i] = photonCounts[i][xbin1Index] * wPhoton / areaCell;
	}

	QVector<double> horizontalXValues( widthDivisions ), horizontalYValues( widthDivisions ); // initialize with entries 0..100
	for( int i = 0; i < widthDivisions; ++i)
	{
		horizontalXValues[i] = xmin + ( i + 0.5 ) * widthCell;
		horizontalYValues[i] = photonCounts[ybin1Index][i] * wPhoton / areaCell;
	}

	// create graph and assign data to it:
	verticalSectorPlot->addGraph();
	verticalSectorPlot->graph( 0 )->setData( verticalXValues, verticalYValues );
	// set axes ranges, so we see all data:
	verticalSectorPlot->xAxis->setRange( ymin, ymax );
	verticalSectorPlot->yAxis->setRange( 0, 1.2 * maximumFlux );
	//verticalSectorPlot->rescaleAxes();
	verticalSectorPlot->replot();

	// create graph and assign data to it:
	horizontaSectorPlot->addGraph();
	horizontaSectorPlot->graph( 0 )->setData( horizontalXValues, horizontalYValues );
	// set axes ranges, so we see all data:
	horizontaSectorPlot->xAxis->setRange( xmin, xmax );
	horizontaSectorPlot->yAxis->setRange( 0, 1.2 * maximumFlux );
	horizontaSectorPlot->replot();
}

/*
 * Updates the labels of the plots
 */
void FluxAnalysisDialog::UpdateLabelsUnits()
{
	QString lengthUnitString = lengthUnitLine->text();
	if( lengthUnitString.isEmpty() ) lengthUnitString = QString( "(unit length)" );

	QString powerUnitString = powerUnitLine->text();
	if( powerUnitString.isEmpty() ) powerUnitString = QString( "(unit power)" );

	contourPlotWidget->xAxis->setLabel( QString( "X (%1)" ).arg( lengthUnitString ) );
	contourPlotWidget->yAxis->setLabel( QString( "Y (%1)" ).arg( lengthUnitString ) );

	horizontaSectorPlot->xAxis->setLabel( QString( "Y (%1)" ).arg( lengthUnitString ) );
	verticalSectorPlot->xAxis->setLabel( QString( "X (%1)" ).arg( lengthUnitString ) );

	horizontaSectorPlot->yAxis->setLabel(QString( "Flux(%1/%2^2)" ).arg( powerUnitString, lengthUnitString ) );
	verticalSectorPlot->yAxis->setLabel(QString( "Flux(%1/%2^2)" ).arg( powerUnitString, lengthUnitString ) );

	 m_fluxLabelString = QString( "Flux(%1/%2^2)" ).arg( powerUnitString, lengthUnitString );

	 QCPColorMap* colorMapPlot = qobject_cast<QCPColorMap*>( contourPlotWidget->plottable() );

	//see how many elements there are
	int elementCount = contourPlotWidget->plotLayout()->elementCount();
	//loop over the elements
	for(int i = 0; i < elementCount; i++)
	   //test to see if any of the layout elements are of QCPColorScale type
	if( qobject_cast<QCPColorScale*>( contourPlotWidget->plotLayout()->elementAt( i ) ) )
		colorMapPlot->colorScale()->axis()->setLabel( m_fluxLabelString );

	contourPlotWidget->replot();
	horizontaSectorPlot->replot();
	verticalSectorPlot->replot();
}

/*
 * Update plots when changing x or y coord
 */
void FluxAnalysisDialog::UpdateSectorPlotSlot()
{
	int** photonCounts = m_fluxAnalysis->photonCountsValue();
	if( !photonCounts || photonCounts == 0 ) return;

	double xmin = m_fluxAnalysis->xminValue();
	double ymin = m_fluxAnalysis->yminValue();
	double xmax = m_fluxAnalysis->xmaxValue();
	double ymax = m_fluxAnalysis->ymaxValue();
	double wPhoton = m_fluxAnalysis->wPhotonValue();
	QString  withValue = gridWidthLine->text();
	QString  heightValue = gridHeightLine->text();
	int widthDivisions = withValue.toInt();
	int heightDivisions = heightValue.toInt();
	double widthCell = ( xmax - xmin ) / widthDivisions;
	double heightCell = ( ymax - ymin ) / heightDivisions;
	double areaCell = widthCell * heightCell;
	double maximumFlux = ( m_fluxAnalysis->maximumPhotonsValue() * wPhoton) / areaCell;
	UpdateSectorPlots( photonCounts, wPhoton, widthDivisions, heightDivisions, xmin, ymin, xmax, ymax, maximumFlux );
}

/*
 * Select directory
 */
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
	SelectSurfaceDialog selectSurfaceDialog( *m_pCurrentSceneModel, false, this );
	selectSurfaceDialog.SetShapeTypeFilters( QVector< QString >()<< QLatin1String( "ShapeFlatRectangle")
			<< QLatin1String( "ShapeFlatDisk")
			<< QLatin1String( "ShapeCylinder") );
	if( !selectSurfaceDialog.exec( ) )	return;

	QString selectedSurfaceURL = selectSurfaceDialog.GetSelectedSurfaceURL();

	if( !selectedSurfaceURL.isEmpty() && ( selectedSurfaceURL != m_currentSurfaceURL ) )
	{
		m_fluxAnalysis->clearPhotonMap();
		ClearCurrentAnalysis();
		surfaceEdit->setText( selectedSurfaceURL );
		m_currentSurfaceURL = selectedSurfaceURL;
		UpdateSurfaceSides( m_currentSurfaceURL );
	}
}

/*!
 * Clear current results and simulation.
 */
void FluxAnalysisDialog::ClearCurrentAnalysis()
{
	//Delete previous sector plots
	verticalSectorPlot->clearPlottables();
	verticalSectorPlot->yAxis->setRange( 0, 1.05 );
	verticalSectorPlot->replot();
	horizontaSectorPlot->clearPlottables();
	horizontaSectorPlot->yAxis->setRange( 0, 1.05 );
	horizontaSectorPlot->replot();
	//Delete previous colormap, scale
	contourPlotWidget->clearPlottables();
	//see how many elements there are
	int elementCount = contourPlotWidget->plotLayout()->elementCount();
	//loop over the elements
	for( int i = 0; i < elementCount; i++ )
	{
	    //test to see if any of the layout elements are of QCPColorScale type
	    if( qobject_cast<QCPColorScale* > ( contourPlotWidget->plotLayout()->elementAt( i ) ) )
	    	contourPlotWidget->plotLayout()->removeAt( i );
	    //collapse the empty elements
	    contourPlotWidget->plotLayout()->simplify();
	}
	contourPlotWidget->replot();

	hSectorXCoordSpin->setValue( 0.00 );
	hSectorXCoordSpin->setMinimum( 0.00 );
	hSectorXCoordSpin->setMaximum( 0.00 );
	hSectorYCoordSpin->setValue( 0.00 );
	hSectorYCoordSpin->setMinimum( 0.00 );
	hSectorYCoordSpin->setMaximum( 0.00 );

	totalPowerValue->setText( QString::number(  0.0 )  );

	minimumFluxValue->setText( QString::number( 0.0 )  );
	averageFluxValue->setText( QString::number( 0.0 )  );
	maximumFluxValue->setText( QString::number( 0.0 ) );

	maxCoordinatesValue->setText( QLatin1String( " ; ") );
	errorValue->setText( QString::number( 0.0 ) );
	uniformityValue->setText( QString::number( 0.0 ) );
	centroidValue->setText( QLatin1String( " ; " ) );
}

/*!
 * Updates the sidesCombo to select the surface side
 */
void FluxAnalysisDialog::UpdateSurfaceSides( QString selectedSurfaceURL )
{
	sidesCombo->clear();

	QString surfaceType = m_fluxAnalysis->GetSurfaceType( selectedSurfaceURL );

	if( !surfaceType.isEmpty() )
	{
		if( surfaceType == QLatin1String( "ShapeFlatRectangle" ) )
		{
			sidesCombo->addItem( QLatin1String( "FRONT" ) );
			sidesCombo->addItem( QLatin1String( "BACK" ) );
		}
		else if( surfaceType == QLatin1String( "ShapeFlatDisk" ) )
		{
			sidesCombo->addItem( QLatin1String( "FRONT" ) );
			sidesCombo->addItem( QLatin1String( "BACK" ) );
		}
		else if( surfaceType == QLatin1String( "ShapeCylinder" ) )
		{
			sidesCombo->addItem( QLatin1String( "INSIDE" ) );
			sidesCombo->addItem( QLatin1String( "OUTSIDE" ) );
		}
	}
}
