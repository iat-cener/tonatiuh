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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QMutex>
#include <QPluginLoader>
#include <QProgressDialog>
#include <QSettings>
#include <QtConcurrentMap>
#include <QTime>
#include <QUndoStack>
#include <QUndoView>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>

#include "ActionInsertMaterial.h"
#include "ActionInsertShape.h"
#include "ActionInsertTracker.h"
#include "CmdChangeNodeName.h"
#include "CmdCopy.h"
#include "CmdCut.h"
#include "CmdDelete.h"
#include "CmdDeleteTracker.h"
#include "CmdInsertMaterial.h"
#include "CmdInsertSeparatorKit.h"
#include "CmdInsertShape.h"
#include "CmdInsertShapeKit.h"
#include "CmdInsertTracker.h"
#include "CmdLightKitModified.h"
#include "CmdLightPositionModified.h"
#include "CmdModifyParameter.h"
#include "CmdPaste.h"
#include "Document.h"
#include "ExportDialog.h"
#include "GraphicView.h"
#include "GridSettingsDialog.h"
#include "InstanceNode.h"
#include "LightDialog.h"
#include "MainWindow.h"
#include "PluginManager.h"
#include "ProgressUpdater.h"
#include "RandomDeviate.h"
#include "RandomDeviateFactory.h"
#include "Ray.h"
#include "RayTraceDialog.h"
#include "RayTracer.h"
#include "SceneModel.h"
#include "ScriptEditorDialog.h"
#include "SunPositionCalculatorDialog.h"
#include "TDefaultTracker.h"
#include "tgf.h"
#include "trf.h"
#include "TLightKit.h"
#include "TMaterial.h"
#include "TMaterialFactory.h"
#include "TPhotonMap.h"
#include "TPhotonMapFactory.h"
#include "Transform.h"
#include "TSeparatorKit.h"
#include "TShapeFactory.h"
#include "TShapeKit.h"
#include "TSunShapeFactory.h"
#include "TTracker.h"
#include "TTrackerFactory.h"
#include "UpdatesManager.h"



void startManipulator(void *data, SoDragger* dragger)
{
	MainWindow* mainwindow = static_cast< MainWindow* >( data );
	mainwindow->StartManipulation( dragger );
}

void finishManipulator(void *data, SoDragger* /*dragger*/ )
{
	MainWindow* mainwindow = static_cast< MainWindow* >( data );
	mainwindow->FinishManipulation( );
}

/*!
 * Creates a new MainWindow object.
 */
MainWindow::MainWindow( QString tonatiuhFile , QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags ),
m_currentFile( "" ),
m_recentFiles( "" ),
m_recentFileActions( 0 ),
m_document( 0 ),
m_commandStack( 0 ),
m_commandView( 0 ),
m_materialsToolBar( 0 ),
m_photonMapToolBar(0),
m_shapeToolBar( 0 ),
m_trackersToolBar( 0 ),
m_pluginManager( 0 ),
m_updateManager( 0 ),
m_sceneModel( 0 ),
m_selectionModel( 0 ),
m_rand( 0 ),
m_selectedRandomDeviate( -1 ),
m_photonMap( 0 ),
m_selectedPhotonMap( -1 ),
m_increasePhotonMap( false ),
m_lastExportFileName( "" ),
m_lastExportSurfaceUrl( "" ),
m_lastExportInGlobal( true ),
m_scriptDirectory("." ),
m_pRays( 0 ),
m_pGrid( 0 ),
m_coinNode_Buffer( 0 ),
m_manipulators_Buffer( 0 ),
m_tracedRays( 0 ),
m_raysPerIteration( 1000 ),
m_fraction( 10 ),
m_drawPhotons( false ),
m_gridXElements( 0 ),
m_gridZElements( 0 ),
m_gridXSpacing( 0 ),
m_gridZSpacing( 0 ),
m_graphicView( 0 ),
//sceneModelView( 0 ),
m_focusView( 0 )
{
	setupUi( this );
    SetupActions();
    SetupMenus();
    SetupDocument();
    SetupModels();
	SetupViews();
	SetupUpdateManager();
	SetupPluginsManager();
	SetupTriggers();
    ReadSettings();

    if( !tonatiuhFile.isEmpty() )	StartOver( tonatiuhFile );
}

/*!
 * Destroyes MainWindow object.
 */
MainWindow::~MainWindow()
{
    delete m_pluginManager;
    delete m_updateManager;
    delete m_sceneModel;
	delete m_document;
	delete m_commandStack;
	delete m_commandView;
	delete m_rand;
	delete[] m_recentFileActions;
	delete m_photonMap;
}

/*!
 * Finish the manipulation of the current selected node.
 */
void MainWindow::FinishManipulation( )
{
	QModelIndex currentIndex = sceneModelView->currentIndex();
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( m_sceneModel->NodeFromIndex(currentIndex)->GetNode() );

	SoTransform* nodeTransform = static_cast< SoTransform* >( coinNode->getPart( "transform", true ) );

	QUndoCommand* command = new QUndoCommand();

	QString translationValue = QString( "%1 %2 %3" ).arg( QString::number( nodeTransform->translation.getValue()[0] ),
														QString::number( nodeTransform->translation.getValue()[1] ),
														QString::number( nodeTransform->translation.getValue()[2] ) );
	new CmdModifyParameter( nodeTransform, QString( "translation" ), translationValue, m_sceneModel, command );

	/*dragAndDrop->setText("Drag and Drop node");
	new CmdCut( node, m_coinNode_Buffer, m_sceneModel, dragAndDrop );

	new CmdPaste( tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDrop );*/
	m_commandStack->push( command );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );

	//CmdModifyParameter* command =new CmdModifyParameter( nodeTransform, coinNode, "transform" );
	/*CmdParameterModified* parameterModified;
	QString type = coinNode->getTypeId().getName().getString() ;
	if ( type == "TLightKit" )	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "tshapekit.transform" );
	else	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "transform" );
	m_commandStack->push( parameterModified );*/


}

/*!
 * Starts manipulating current selected node with \a dragger.
 */
void MainWindow::StartManipulation( SoDragger* dragger )
{
	SoSearchAction* coinSearch = new SoSearchAction();
	coinSearch->setNode( dragger );
	coinSearch->setInterest( SoSearchAction::FIRST);

	coinSearch->apply( m_document->GetRoot() );

	SoPath* coinScenePath = coinSearch->getPath( );
	if( !coinScenePath ) tgf::SevereError( "PathFromIndex Null coinScenePath." );

	SoNodeKitPath* nodePath = static_cast< SoNodeKitPath* > ( coinScenePath );
	if( !nodePath ) tgf::SevereError( "PathFromIndex Null nodePath." );


	nodePath->truncate(nodePath->getLength()-1 );
	SoBaseKit* coinNode =  static_cast< SoBaseKit* > ( nodePath->getTail() );

	QModelIndex nodeIndex = m_sceneModel->IndexFromPath( *nodePath );
	m_selectionModel->setCurrentIndex( nodeIndex , QItemSelectionModel::ClearAndSelect );

	SoNode* manipulator = coinNode->getPart( "transform", true );
	m_manipulators_Buffer = new QStringList();

	SoFieldList fieldList;
    int totalFields = manipulator->getFields( fieldList );

    SoField* pField = 0;
    SbName fieldName;
    SbString fieldValue = "null";


	for( int index = 0; index < totalFields; ++index )
	{
		pField = fieldList.get( index );
		if( pField )
		{

    		pField->get( fieldValue );
			m_manipulators_Buffer->push_back(QString( fieldValue.getString() ) );
		}
	}
	delete coinSearch;
}

/*!
 * Shows a dialog to allow define current light position with a position calculator.
 */
void MainWindow::CalculateSunPosition()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene->getPart( "lightList[0]", false ) ) return;
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );


	QDateTime currentTime;
	double longitude;
	double latitude;

	lightKit->GetPositionData( &currentTime, &longitude, &latitude );

	SunPositionCalculatorDialog sunposDialog;
	sunposDialog.ChangePosition( currentTime, longitude, latitude );

	connect( &sunposDialog, SIGNAL( changeSunLight( QDateTime*, double, double ) ) , this, SLOT( ChangeSunPosition( QDateTime*, double, double ) ) );

	sunposDialog.exec();
}

/*!
 *Defines Tonatiuh model light paramenters with a dilog window.
 */
void MainWindow::DefineSunLight()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene ) return;

	TLightKit* currentLight = 0;
	if( coinScene->getPart( "lightList[0]", false ) )	currentLight = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );

	QVector< TShapeFactory* > shapeFactoryList = m_pluginManager->GetShapeFactories();

	QVector< TShapeFactory* > tFlatShapeFactoryList;
	for( int i = 0; i < shapeFactoryList.size(); ++i  )
		if( shapeFactoryList[i]->IsFlat() )	tFlatShapeFactoryList<<shapeFactoryList[i];

	QVector< TSunShapeFactory* > tSunShapeFactoryList = m_pluginManager->GetSunShapeFactories();

	LightDialog dialog( currentLight, tFlatShapeFactoryList, tSunShapeFactoryList );
	if( dialog.exec() )
	{

		TLightKit* lightKit = dialog.GetTLightKit();
		if( !lightKit ) return;

		lightKit->setName( "Light" );

		CmdLightKitModified* command = new CmdLightKitModified( lightKit, coinScene, *m_sceneModel );
		m_commandStack->push( command );

		UpdateLightDimensions();

		parametersView->UpdateView();
		m_document->SetDocumentModified( true );

		actionCalculateSunPosition->setEnabled( true );

	}
}

/*!
 *If actionDisplay_rays is checked the 3D view shows rays representation. Otherwise the representation is hidden.
 */
void MainWindow::DisplayRays( bool display )
{
	if( display && (m_pRays ) )
		m_document->GetRoot()->insertChild( m_pRays, 0 );
	else if( !display )
		if ( m_pRays->getRefCount( ) > 0 )	m_document->GetRoot()->removeChild( 0 );

}

/*!
 * Writes the photons stored at the photon map at user defined file. Creates a dialog to define the export paramenters.
 */
void MainWindow::ExportPhotonMap()
{
	if ( m_photonMap == NULL )
	{
		QMessageBox::information( this, "Tonatiuh", "No Photon Map stored", 1 );
	    return;
	}

	ExportDialog exportDialog( *m_sceneModel, m_lastExportSurfaceUrl, m_lastExportInGlobal, m_lastExportFileName, this );
	if( !exportDialog.exec() ) return;

	QString fileName = exportDialog.GetExportFileName();
	if( fileName.isEmpty() )
	{
		QMessageBox::information( this, "Tonatiuh", "No file defined to save Photon Map", 1 );
		return;
	}
	m_lastExportFileName = fileName;
	m_lastExportInGlobal = exportDialog.ExportPhotonsInGlobal();

	//Compute photon power
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene ) return;
	if( !coinScene->getPart( "lightList[0]", false ) ) return;
	TLightKit*lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );


	if( !lightKit->getPart( "tsunshape", false ) ) return;
	TSunShape* sunShape = static_cast< TSunShape * >( lightKit->getPart( "tsunshape", false ) );
	double irradiance = sunShape->GetIrradiance();

	if( !lightKit->getPart( "icon", false ) ) return;
	TShape* raycastingShape = static_cast< TShape * >( lightKit->getPart( "icon", false ) );
	double inputAperture = raycastingShape->GetArea();

	double wPhoton = ( inputAperture * irradiance ) / m_tracedRays;

	int okExport;
	if( exportDialog.ExportAllPhotonMap() )
		okExport = trf::ExportAll( m_lastExportFileName, wPhoton, m_photonMap );
	else
	{
		QString nodeURL = exportDialog.GetSelectedSurface();
		if( !nodeURL.isEmpty() )	m_lastExportSurfaceUrl = nodeURL;

		QModelIndex selectedNodeIndex = m_sceneModel->IndexFromNodeUrl( nodeURL );
		InstanceNode* selectedSurface = m_sceneModel->NodeFromIndex( selectedNodeIndex );
		if( !selectedSurface )
		{
			QMessageBox::warning( this, "Tonatiuh", "Selected node in not a valid export node." );
			return;
		}

		if( m_lastExportInGlobal )
			okExport = trf::ExportSurfaceGlobalCoordinates( m_lastExportFileName, selectedSurface, wPhoton, m_photonMap );
		else
			okExport = trf::ExportSurfaceLocalCoordinates( m_lastExportFileName, selectedSurface, wPhoton, m_photonMap );
	}
	if( !okExport )
	{
		QMessageBox::warning( this, "Tonatiuh", "An unexpected error has occurred exporting photon map to a file." );
		return;
	}

}

/*!
 * Shows a dialog to the user to open a existing tonatiuh file.
 */
void MainWindow::Open()
{
    if ( OkToContinue() )
    {
    	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
    	QString openDirectory = settings.value( "openDirectory", QString( "." ) ).toString();

        QString fileName = QFileDialog::getOpenFileName( this,
                               tr( "Open" ), openDirectory,
                               tr( "Tonatiuh files (*.tnh)" ) );

    	if( fileName.isEmpty() ) return;

    	QFileInfo file( fileName );
    	settings.setValue( "openDirectory", file.absolutePath() );
    	StartOver( fileName );
    }
}

/*!
 * Opens selected tonatiuh file from the recent file menu.
 */
void MainWindow::OpenRecentFile()
{
    if ( OkToContinue() )
    {
        QAction* action = qobject_cast<QAction *>( sender() );

        if ( action )
        {
        	QString fileName = action->data().toString();
        	StartOver( fileName );
        }
    }
}

/*!
 * Applies las reverted command action changes to Tonatiuh.
 */
void MainWindow::Redo()
{
    m_commandStack->redo();
    UpdateLightDimensions();
}

/*!
 * Changes to the node defined by \a node the value of the \a parameter to \a value.
 */
void MainWindow::SetParameterValue( SoNode* node, QString paramenterName, QString newValue )
{
	CmdModifyParameter* command = new CmdModifyParameter( node, paramenterName, newValue, m_sceneModel );
	if ( m_commandStack ) m_commandStack->push( command );
	m_sceneModel->UpdateSceneModel();

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );
}

/*!
 * This property holds whether the SunPositionCalculator action is enabled.
 * If the action is disabled, it does not disappear from menu, but it is displayed
 * in a way which indicates that they are unavailable.
 */
void MainWindow::SetSunPositionCalculatorEnabled( int enabled )
{
	actionCalculateSunPosition->setEnabled( enabled );
}

/*!
 * Shows a windows with the applied commands.
 * The most recently executed command is always selected.
 * When a different command is selected the model returns to the state after selected command was applied.
 */
void MainWindow:: ShowCommandView()
{
    m_commandView->show();
}

/*!
 * Shows selected node right menu.
 */
void MainWindow::ShowMenu( const QModelIndex& index)
{
	if( !index.isValid() ) return;
	m_selectionModel->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect );

	SoNode* coinNode = m_sceneModel->NodeFromIndex(index)->GetNode();
	SoType type = coinNode->getTypeId();

	QMenu popupmenu(this);

   	popupmenu.addAction( actionCut );
   	popupmenu.addAction( actionCopy );
  	popupmenu.addAction( actionPasteCopy );
  	popupmenu.addAction( actionPasteLink );
  	popupmenu.addAction( actionDelete );

	QMenu transformMenu( "Convert to", &popupmenu );

	if( type.isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		popupmenu.addAction( transformMenu.menuAction() );
		transformMenu.addAction( tr("SoCenterballManip"),  this, SLOT(SoTransform_to_SoCenterballManip()));
		transformMenu.addAction( tr("SoHandleBoxManip"), this, SLOT(SoTransform_to_SoHandleBoxManip()));
		transformMenu.addAction( tr("SoJackManip"), this, SLOT(SoTransform_to_SoJackManip()));
		transformMenu.addAction( tr("SoTabBoxManip"), this, SLOT(SoTransform_to_SoTabBoxManip()));
		transformMenu.addAction( tr("SoTrackballManip"),  this, SLOT(SoTransform_to_SoTrackballManip()));
		transformMenu.addAction( tr("SoTransformBoxManip"), this, SLOT(SoTransform_to_SoTransformBoxManip()));
		transformMenu.addAction( tr("SoTransformerManip"), this, SLOT(SoTransform_to_SoTransformerManip()));

		TSeparatorKit* coinKit = dynamic_cast< TSeparatorKit* > ( coinNode );
		SoTransform* transform = static_cast< SoTransform* >( coinKit->getPart("transform", true) );
		SoType transformType = transform->getTypeId();

      	//Manipuladores
		if ( transformType.isDerivedFrom(SoTransformManip::getClassTypeId()) )	transformMenu.addAction( tr("SoTransform"), this, SLOT(SoManip_to_SoTransform()) );

	}

	//Mostramos el menu contextual
	popupmenu.exec( QCursor::pos() );

}

/*!
 * Shows a dialog with ray tracer options and modifies the ray tracer parameters if changes are done.
 */
void MainWindow::ShowRayTracerOptionsDialog()
{
	double oldSelectedRandomDeviate = m_selectedRandomDeviate;
	QVector< RandomDeviateFactory* > randomDeviateFactoryList = m_pluginManager->GetRandomDeviateFactories();
	QVector< TPhotonMapFactory* > photonmapFactoryList = m_pluginManager->GetPhotonMapFactories();

	RayTraceDialog* options = new RayTraceDialog( m_raysPerIteration, randomDeviateFactoryList, m_fraction, m_drawPhotons, photonmapFactoryList, m_selectedRandomDeviate, m_selectedPhotonMap, m_increasePhotonMap, this );
	options->exec();

	SetRaysPerIteration( options->GetNumRays() );
	m_selectedRandomDeviate = options->GetRandomDeviateFactoryIndex();
	if( oldSelectedRandomDeviate != m_selectedRandomDeviate )
	{
		delete m_rand;
		m_rand = 0;
	}

	m_fraction = options->GetRaysFactionToDraw();
	m_drawPhotons = options->DrawPhotons();

	m_selectedPhotonMap =options->GetPhotonMapFactoryIndex();
	m_increasePhotonMap = options->IncreasePhotonMap();

}

/*!
 * Reverts a change to Tonatiuh model. The model is returne to the previous state before the command is applied.
 */
void MainWindow::Undo()
{
    m_commandStack->undo();
    UpdateLightDimensions();
}

//View menu actions
void MainWindow::on_actionAxis_toggled()
{
	m_graphicView[0]->ViewCoordinateSystem( actionAxis->isChecked() );
	m_graphicView[1]->ViewCoordinateSystem( actionAxis->isChecked() );
	m_graphicView[2]->ViewCoordinateSystem( actionAxis->isChecked() );
	m_graphicView[3]->ViewCoordinateSystem( actionAxis->isChecked() );
}
/**
 * Action slot to show/hide a grid with the scene dimensions.
 */
void MainWindow::on_actionGrid_toggled()
{
	if( actionGrid->isChecked() )
	{
		InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
		if ( !sceneInstance )  return;
		SoNode* rootNode = sceneInstance->GetNode();
		SoPath* nodePath = new SoPath( rootNode );
		nodePath->ref();

		SbViewportRegion region = m_graphicView[m_focusView]->GetViewportRegion();
		SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( region ) ;
		if(nodePath)	bbAction->apply(nodePath);

		SbXfBox3f box= bbAction->getXfBoundingBox();
		delete bbAction;
		nodePath->unref();

		m_gridXElements = 10;
	    m_gridZElements = 10;
	    m_gridXSpacing = 10;
	    m_gridZSpacing = 10;

		if( !box.isEmpty() )
		{
			SbVec3f min, max;
			box.getBounds(min, max);

			m_gridXSpacing = ( 2 *  std::max( fabs( max[0] ), fabs( min[0] ) ) + 5  ) / m_gridXElements;
			m_gridZSpacing = ( 2 *  std::max( fabs( max[2] ), fabs( min[2] ) ) + 5 ) / m_gridZElements;

		}

		m_pGrid = new SoSeparator;
		m_pGrid->ref();
		m_pGrid->addChild( CreateGrid( m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing ) );
		m_document->GetRoot()->addChild(m_pGrid);
	}
	else
	{
		m_document->GetRoot()->removeChild( m_pGrid );
		m_pGrid->removeAllChildren();
		if ( m_pGrid->getRefCount() > 1 ) tgf::SevereError("on_actionGridSettings_triggered: m_pGrid referenced in excess ");
		m_pGrid->unref();
		m_pGrid = 0;

	}

}

/**
 * Action slot to open grid settings dialog.
 */
void MainWindow::on_actionGridSettings_triggered()
{
	GridSettingsDialog* gridDialog = new GridSettingsDialog( m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing );
	if( gridDialog->exec() )
	{
		m_document->GetRoot()->removeChild(m_pGrid);
		m_pGrid->removeAllChildren();
		if ( m_pGrid->getRefCount() > 1 ) tgf::SevereError("on_actionGridSettings_triggered: m_pGrid referenced in excess ");
		m_pGrid->unref();
		m_pGrid = 0;

		m_gridXElements = gridDialog->GetXDimension();
		m_gridZElements = gridDialog->GetZDimension();

		if( gridDialog->IsSizeDefined() )
		{
			m_gridXSpacing = gridDialog->GetXSpacing();
			m_gridZSpacing = gridDialog->GetZSpacing();
		}
		else
		{
			InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
			if ( !sceneInstance )  return;
			SoNode* rootNode = sceneInstance->GetNode();
			SoPath* nodePath = new SoPath( rootNode );
			nodePath->ref();

			SbViewportRegion region = m_graphicView[m_focusView]->GetViewportRegion();
			SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( region ) ;
			if(nodePath)	bbAction->apply(nodePath);

			SbXfBox3f box= bbAction->getXfBoundingBox();
			delete bbAction;
			nodePath->unref();

		    m_gridXSpacing = 10;
		    m_gridZSpacing = 10;

			if( !box.isEmpty() )
			{
				SbVec3f min, max;
				box.getBounds(min, max);

				m_gridXSpacing = ( 2 *  std::max( fabs( max[0] ), fabs( min[0] ) ) + 5  ) / m_gridXElements;
				m_gridZSpacing = ( 2 *  std::max( fabs( max[2] ), fabs( min[2] ) ) + 5 ) / m_gridZElements;

			}

		}
		m_pGrid = CreateGrid( m_gridXElements, m_gridZElements, m_gridXSpacing, m_gridZSpacing );
		m_pGrid->ref();
		m_document->GetRoot()->addChild(m_pGrid);
	}
	delete gridDialog;
}

void MainWindow::on_actionBackground_toggled()
{
	SoVRMLBackground* vrmlBackground = static_cast< SoVRMLBackground* > ( m_document->GetRoot()->getChild( 0 ) );

	if( actionBackground->isChecked() )
	{
		float gcolor[][3] = { {0.9843, 0.8862, 0.6745}, {0.7843, 0.6157, 0.4785} };
		float gangle= 1.570f;

		vrmlBackground->groundColor.setValues( 0, 6, gcolor );
		vrmlBackground->groundAngle.setValue( gangle );
		float scolor[][3] = { {0.0157, 0.0235, 0.4509}, {0.5569, 0.6157, 0.8471} };
		float sangle= 1.570f;
		vrmlBackground->skyColor.setValues( 0,6,scolor );
		vrmlBackground->skyAngle.setValue( sangle );
	}
	else
	{
		float color[][3] = { {0.1, 0.1, 0.1}, {0.1, 0.1, 0.1} };
		float angle= 1.570f;
		vrmlBackground->groundColor.setValues( 0, 6, color );
		vrmlBackground->groundAngle.setValue( angle );
		vrmlBackground->skyColor.setValues( 0,6,color );
		vrmlBackground->skyAngle.setValue( angle );
	}
}

void MainWindow::on_actionEdit_Mode_toggled()
{
	if ( !actionEdit_Mode->isChecked() )
	{
		m_graphicView[1]->hide();
		m_graphicView[2]->hide();
		m_graphicView[3]->hide();
		m_focusView = 0;
	}
	else
	{
		m_graphicView[1]->show();
		m_graphicView[2]->show();
		m_graphicView[3]->show();
	}
}

void MainWindow::on_actionSunPlane_triggered()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if ( !coinScene )  return;

	//Check if there is a light and is properly configured
	if ( !coinScene->getPart( "lightList[0]", false ) )	return;
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );
	if ( !lightKit ) return;

	SoTransform* lightTransform = static_cast< SoTransform* > ( lightKit->getPart( "transform", true ) );
	Transform lightToWorld = tgf::TransformFromSoTransform( lightTransform );
	Point3D camPosition = lightToWorld( Point3D( 0.0, 0.0, 0.0 ) );
	Vector3D camOrientation= lightToWorld( Vector3D( 0.0, -1.0, 0.0 ) );

	double t = 1.0;
	if( camPosition.y != 0.0 && camOrientation.y != 0.0 ) t = -camPosition.y / camOrientation.y;
	Point3D targetPoint = camPosition + camOrientation * t;


	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( camPosition.x, camPosition.y, camPosition.z ) );
	cam->pointAt( SbVec3f( targetPoint.x, targetPoint.y, targetPoint.z ) );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_action_X_Y_Plane_triggered()
{
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( 0, 0, 1 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 1, 0 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_action_X_Z_Plane_triggered()
{
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( 0, 1, 0 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 0, 1 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_action_Y_Z_Plane_triggered()
{
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( -1, 0, 0 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 1, 0 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_actionOpenScriptEditor_triggered()
{
	QVector< RandomDeviateFactory* > randomDeviateFactoryList = m_pluginManager->GetRandomDeviateFactories();
	QVector< TPhotonMapFactory* > photonmapFactoryList = m_pluginManager->GetPhotonMapFactories();

	ScriptEditorDialog editor( photonmapFactoryList, randomDeviateFactoryList, m_scriptDirectory, this );
	editor.exec();

	m_scriptDirectory = editor.GetCurrentDirectory();
}

/*!
 * Checks for Stephanie's updates and installs them.
 */
void MainWindow::on_actionCheckForUpdates_triggered()
{
	m_updateManager->CheckForUpdates( );
}

void MainWindow::on_actionAbout_triggered()
{
	//QMessageBox::aboutQt( this );

	QString appVersion = qApp->applicationVersion();
	QString aboutMessage("Tonatiuh\n"
			"Version: "+ appVersion + "\n"
			"\nPlease see http://www.gnu.org/licenses/gpl.html for an overview of GPLv3 licensing.\n"
			"\nSee http://code.google.com/p/tonatiuh/ for more information.");
	QMessageBox::about( this, QString( "About Toantiuh" ), aboutMessage );
}

/*!
 *
 */
void MainWindow::ChangeNodeName( const QModelIndex& index, const QString& newName)
{
	if( !index.isValid() )	return;

	InstanceNode* nodeInstance = m_sceneModel->NodeFromIndex( index );
	if( !nodeInstance ) return;
	if( ! nodeInstance->GetNode() )	return;

	CmdChangeNodeName* command = new CmdChangeNodeName( index, newName, m_sceneModel );
	QString commandText = QString( "Node name changed to: %1").arg( newName );
	command->setText(commandText);
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );

}

/*!
 * Copies current node to the clipboard.
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy( )
{
	if( !m_selectionModel->hasSelection() ) return;
	if( m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return;
	if( m_selectionModel->currentIndex().parent() == sceneModelView->rootIndex() ) return;


	CmdCopy* command = new CmdCopy( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
	return;
}

/*!
 * Copies the node defined with the \a nodeURL to the clipboard.
 *
 * The current node cannot be the model root node or concentrator node.
 */
void MainWindow::Copy( QString nodeURL )
{
	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeURL );

	if( !nodeIndex.isValid() )	return;
	if( nodeIndex == sceneModelView->rootIndex() ) return;
	if( nodeIndex.parent() == sceneModelView->rootIndex() ) return;

	CmdCopy* command = new CmdCopy( nodeIndex, m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );

}

/*!
 * Creates a new group node as a selected node child.
 */
void MainWindow::CreateGroupNode()
{
	QModelIndex parentIndex;
	if (( !sceneModelView->currentIndex().isValid() ) || ( sceneModelView->currentIndex() == sceneModelView->rootIndex()))
		parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
	else
		parentIndex = sceneModelView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	if( !parentInstance ) return;

	SoNode* coinNode = parentInstance->GetNode();
	if( !coinNode ) return;


	if ( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		TSeparatorKit* separatorKit = new TSeparatorKit();

		CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit( separatorKit, QPersistentModelIndex(parentIndex), m_sceneModel );
		cmdInsertSeparatorKit->setText( "Insert SeparatorKit node" );
		m_commandStack->push( cmdInsertSeparatorKit );

		int count = 1;
		QString nodeName = QString( "TSeparatorKit%1").arg( QString::number( count) );
		while ( !m_sceneModel->SetNodeName( separatorKit, nodeName ) )
		{
			count++;
			nodeName = QString( "TSeparatorKit%1").arg( QString::number( count) );
		}
		UpdateLightDimensions();

		m_document->SetDocumentModified( true );

	}
}

/*!
 * Creates a \a materialType material node from the as current selected node child.
 *
 * If the current node is not a surface type node or \a materialType is not a valid type, the material node will not be created.
 */
void MainWindow::CreateMaterial( QString materialType )
{
	QVector< TMaterialFactory* > factoryList = m_pluginManager->GetMaterialFactories();
	if( factoryList.size() == 0 )	return;

	QVector< QString > materialNames;
	for( int i = 0; i < factoryList.size(); i++ )
		materialNames<< factoryList[i]->TMaterialName();

	int selectedMaterial = materialNames.indexOf( materialType );
	if( selectedMaterial < 0 )	return;

	CreateMaterial( factoryList[ selectedMaterial ] );
}

/*!
 * Creates a \a shapeType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a shapeType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateShape( QString shapeType )
{
	QVector< TShapeFactory* > factoryList = m_pluginManager->GetShapeFactories();
	if( factoryList.size() == 0 )	return;

	QVector< QString > shapeNames;
	for( int i = 0; i < factoryList.size(); i++ )
		shapeNames<< factoryList[i]->TShapeName();

	int selectedShape = shapeNames.indexOf( shapeType );
	if( selectedShape < 0 )	return;

	CreateShape( factoryList[ selectedShape ] );

}

/*!
 * Creates a surface node as selected node child.
 */
void MainWindow::CreateSurfaceNode()
{
	QModelIndex parentIndex;
	if (( ! sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex()))
		parentIndex = m_sceneModel->index (0,0, sceneModelView->rootIndex());
	else
		parentIndex = sceneModelView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	if( !parentInstance ) return;

		SoNode* selectedCoinNode = parentInstance->GetNode();
	if( !selectedCoinNode ) return;

	if ( selectedCoinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		TShapeKit* shapeKit = new TShapeKit;

		CmdInsertShapeKit* insertShapeKit = new CmdInsertShapeKit( parentIndex, shapeKit, m_sceneModel );
		m_commandStack->push( insertShapeKit );

		int count = 1;
		QString nodeName = QString( "TShapeKit%1").arg( QString::number( count) );
		while ( !m_sceneModel->SetNodeName( shapeKit, nodeName ) )
		{
			count++;
			nodeName = QString( "TShapeKit%1").arg( QString::number( count) );
		}
		UpdateLightDimensions();
		m_document->SetDocumentModified( true );
	}
}


/*!
 * Creates a \a trackerType shape node from the as current selected node child.
 *
 * If the current node is not a valid parent node or \a trackerType is not a valid type, the shape node will not be created.
 *
 */
void MainWindow::CreateTracker( QString trackerType )
{
	QVector< TTrackerFactory* > factoryList = m_pluginManager->GetTrackerFactories();
	if( factoryList.size() == 0 )	return;

	QVector< QString > trackerNames;
	for( int i = 0; i < factoryList.size(); i++ )
		trackerNames<< factoryList[i]->TTrackerName();

	int selectedTracker = trackerNames.indexOf( trackerType );
	if( selectedTracker < 0 )	return;

	CreateTracker( factoryList[ selectedTracker ] );
}

/*!
 * If there is a node selected and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut()
{
	if( !m_selectionModel->hasSelection() ) return;
	if( m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return;
	if( m_selectionModel->currentIndex().parent() == sceneModelView->rootIndex() ) return;

	CmdCut* command = new CmdCut( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );
}

/*!
 * If there \a nodeURL is a valid node url and this node is not the root node, cuts current selected node from the model. Otherwise, nothing is done.
 */
void MainWindow::Cut( QString nodeURL )
{
	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeURL );

	if( !nodeIndex.isValid() )	return;
	if( nodeIndex == sceneModelView->rootIndex() ) return;
	if( nodeIndex.parent() == sceneModelView->rootIndex() ) return;

	CmdCut* command = new CmdCut( nodeIndex, m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );

}

/*!
 * * Deletes selected node if there is a valid node to delete.
 */
void MainWindow::Delete( )
{
	if( !m_selectionModel->hasSelection() ) return;
	Delete( m_selectionModel->currentIndex() );
	if( m_selectionModel->hasSelection() )	m_selectionModel->clearSelection();

}

/*!
 * Creates a new delete command, where the node with the \a nodeURL was deleted.
 *
 * If \a nodeURL is not a valid node url, nothing is done.
 */
void MainWindow::Delete( QString nodeURL )
{
	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeURL );
	if( !nodeIndex.isValid() )	return;
	Delete( nodeIndex );
	if( m_selectionModel->isSelected ( nodeIndex ) ) m_selectionModel->clearSelection();

}

/*!
 *
 * Inserts an existing tonatiuh component into the tonatiuh model as a selected node child.
 *
 * The component is saved into \a componentFileName file.
 */
void MainWindow::InsertFileComponent( QString componentFileName )
{
	QModelIndex parentIndex;
	if (( !sceneModelView->currentIndex().isValid() ) || ( sceneModelView->currentIndex() == sceneModelView->rootIndex() ) )
			parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
	else
		parentIndex = sceneModelView->currentIndex();

	SoNode* coinNode = m_sceneModel->NodeFromIndex( parentIndex )->GetNode();
	if ( !coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) return;

	if ( componentFileName.isEmpty() ) return;

	SoInput componentInput;
	if ( !componentInput.openFile( componentFileName.toLatin1().constData() ) )
	{
		QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
				tr( "Cannot open file:\n%1." ).arg( componentFileName ) );
		return;
	}

	SoSeparator* componentSeparator = SoDB::readAll( &componentInput );
	componentInput.closeFile();

	if ( !componentSeparator )
	{
		QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
				tr( "Error reading file: \n%1." )
				.arg( componentFileName ) );
		return;
	}

	TSeparatorKit* componentRoot = static_cast< TSeparatorKit* >( componentSeparator->getChild(0) );
	CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit( componentRoot, QPersistentModelIndex(parentIndex), m_sceneModel );
	cmdInsertSeparatorKit->setText( "Insert SeparatorKit node" );
	m_commandStack->push( cmdInsertSeparatorKit );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );

}

/*!
 * Inserts an existing tonatiuh component into the tonatiuh model as a selected node child.
 *
 * A open file dialog is opened to select the file where the existing component is saved.
 */
void MainWindow::InsertUserDefinedComponent()
{
	QModelIndex parentIndex;
	 if (( !sceneModelView->currentIndex().isValid() ) || ( sceneModelView->currentIndex() == sceneModelView->rootIndex() ) )
		 parentIndex = m_sceneModel->index (0,0,sceneModelView->rootIndex());
	 else
		 parentIndex = sceneModelView->currentIndex();

	SoNode* coinNode = m_sceneModel->NodeFromIndex( parentIndex )->GetNode();

	if ( !coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) return;

	QString fileName = QFileDialog::getOpenFileName( this,
			                               tr( "Open Tonatiuh document" ), ".",
			                               tr( "Tonatiuh component (*.tcmp)" ) );

	if ( fileName.isEmpty() ) return;

	InsertFileComponent( fileName );
}

/*!
 * Starts new tonatiuh empty model.
 */

void MainWindow::New()
{
    if ( OkToContinue() ) StartOver( "" );
}

/*!
 * Opens \a fileName file is there is a valid tonatiuh file.
 */
void MainWindow::Open( QString fileName )
{
	if( fileName.isEmpty() )	return;
	StartOver( fileName );
}

/*!
 * Inserts a new node as a \a nodeURL node child. The new node is a copy to node saved into the clipboard.
 * The \a pasteType take "Shared" or "Copied" values.
 */
void MainWindow::Paste( QString nodeURL, QString pasteType)
{

	if( !m_coinNode_Buffer ) return;

	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeURL );
	if( !nodeIndex.isValid() )	return;

	if( pasteType == QString( "Shared" ) )
		Paste( nodeIndex, tgc::Shared );
	else
		Paste( nodeIndex, tgc::Copied );

}


/*!
 * Inserts a new node as a current node child. The new node is a copy to node saved into the clipboard.
 */
void MainWindow::PasteCopy()
{
	if( !m_selectionModel->hasSelection() )	return;
	Paste( m_selectionModel->currentIndex(), tgc::Copied );
}

/*!
 * Inserts a new node as a current node child. The new node is a link to node saved into the clipboard.
 */
void MainWindow::PasteLink()
{
	if( !m_selectionModel->hasSelection() )	return;
	Paste( m_selectionModel->currentIndex(), tgc::Shared );
}

/*!
 * Runs ray tracer to defined model and paramenters.
 */
void MainWindow::Run()
{
	QDateTime startTime = QDateTime::currentDateTime();

    // Initialize variables
	InstanceNode* rootSeparatorInstance = 0;
	InstanceNode* lightInstance = 0;
	SoTransform* lightTransform = 0;
	TSunShape* sunShape = 0;
	TShape* raycastingSurface = 0;

	if( ReadyForRaytracing( rootSeparatorInstance, lightInstance, lightTransform, sunShape, raycastingSurface ) )
	{
		//Compute bounding boxes and world to object transforms
		trf::ComputeSceneTreeMap( rootSeparatorInstance, Transform( new Matrix4x4 ) );

		QVector< double > raysPerThread;
		const int maximumValueProgressScale = 100;
		unsigned long  t1 = m_raysPerIteration / maximumValueProgressScale;
		for( int progressCount = 0; progressCount < maximumValueProgressScale; ++ progressCount )
			raysPerThread<< t1;

		if( ( t1 * maximumValueProgressScale ) < m_raysPerIteration )	raysPerThread<< ( m_raysPerIteration-( t1* maximumValueProgressScale) );

		Transform lightToWorld = tgf::TransformFromSoTransform( lightTransform );

		// Create a progress dialog.
		QProgressDialog dialog;
		dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount()));

		//ParallelRandomDeviate* m_pParallelRand = new ParallelRandomDeviate( *m_rand,140000 );
		// Create a QFutureWatcher and conncect signals and slots.
		QFutureWatcher< TPhotonMap* > futureWatcher;
		QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
		QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
		QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
		QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

		QMutex mutex;
		QFuture< TPhotonMap* > photonMap = QtConcurrent::mappedReduced( raysPerThread, RayTracer(  rootSeparatorInstance, lightInstance, raycastingSurface, sunShape, lightToWorld, *m_rand, &mutex, m_photonMap ), trf::CreatePhotonMap, QtConcurrent::UnorderedReduce );
		futureWatcher.setFuture( photonMap );

		// Display the dialog and start the event loop.
		dialog.exec();
		futureWatcher.waitForFinished();


		QDateTime time2 = QDateTime::currentDateTime();
		std::cout <<"time2: "<< startTime.secsTo( time2 ) << std::endl;

		m_tracedRays += m_raysPerIteration;
		ShowRaysIn3DView();

	}

	QDateTime endTime = QDateTime::currentDateTime();
	std::cout <<"Elapsed time: "<< startTime.secsTo( endTime ) << std::endl;
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved in the current file. Otherwise, returns \a false.
 *
 * If a current file is not defined, it calls to SaveAs funtios.
 *
 * \sa SaveAs, SaveFile.
 */
bool MainWindow::Save()
{
	if ( m_currentFile.isEmpty() ) return SaveAs();
	else return SaveFile( m_currentFile );
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved. Otherwise, returns \a false. A file dialog is created to select a file.
 *
 * \sa Save, SaveFile.
 */
bool MainWindow::SaveAs()
{

	QSettings settings( "NREL UTB CENER", "Tonatiuh" );

	QString tonatiuhFilter( "Tonatiuh files (*.tnh)" );
	QString saveDirectory = settings.value( "saveDirectory", QString( "." ) ).toString();

	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save" ), saveDirectory,
	                       tr( "Tonatiuh files (*.tnh)" ) );
	if( fileName.isEmpty() ) return false;

	QFileInfo file( fileName );
	settings.setValue( "saveDirectory", file.absolutePath() );
	return SaveFile( fileName );
}

/*!
 * Changes selected node to the node with \a nodeUrl. If model does not contains a node with defined url,
 * the selection will be null.
 */
void MainWindow::SelectNode( QString nodeUrl )
{
	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeUrl );
	m_selectionModel->setCurrentIndex( nodeIndex , QItemSelectionModel::ClearAndSelect );
}

/*!
 * Saves selected node subtree as a component in a file.
 * A dialog is opened to select a file name and its location.
 */
bool MainWindow::SaveComponent()
{
    if( !m_selectionModel->hasSelection() ) return false;
    if( m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return false;

    QModelIndex componentIndex = sceneModelView->currentIndex();

    SoNode* coinNode = m_sceneModel->NodeFromIndex( componentIndex )->GetNode();

    if ( !coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
    {
    	QMessageBox::warning( 0, tr( "Tonatiuh" ),
                                  tr( "Selected node in not valid  for component node" ) );
    	return false;
    }

    TSeparatorKit* componentRoot = dynamic_cast< TSeparatorKit* > ( coinNode );
    if( !componentRoot ) return false;


	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save Tonatiuh component" ), ".",
	                       tr( "Tonatiuh component (*.tcmp)" ) );
	if( fileName.isEmpty() ) return false;

    SoWriteAction SceneOuput;
    if ( !SceneOuput.getOutput()->openFile( fileName.toLatin1().constData() ) )
	{
        QMessageBox::warning( 0, tr( "Tonatiuh" ),
                              tr( "Cannot open file %1. " )
                            .arg( fileName ));
   		return false;
   	}

    QApplication::setOverrideCursor( Qt::WaitCursor );
   	SceneOuput.getOutput()->setBinary( false );
   	SceneOuput.apply( componentRoot );
   	SceneOuput.getOutput()->closeFile();
   	QApplication::restoreOverrideCursor();
	return true;
}

/*!
 * Sets \a nodeName as the current node name.
 */
void MainWindow::SetNodeName( QString nodeName )
{
	if( !m_selectionModel->hasSelection() ) return;
	if( m_selectionModel->currentIndex() == sceneModelView->rootIndex() ) return;

	ChangeNodeName( m_selectionModel->currentIndex(), nodeName );
}

/*!
 *Sets the photon map type, \a typeName, for ray tracing.
 */
int MainWindow::SetPhotonMapType( QString typeName )
{
	QVector< TPhotonMapFactory* > factoryList = m_pluginManager->GetPhotonMapFactories();
	if( factoryList.size() == 0 )	return 0;

	QVector< QString > photonMapNames;
	for( int i = 0; i < factoryList.size(); i++ )
		photonMapNames<< factoryList[i]->TPhotonMapName();

	m_selectedPhotonMap = photonMapNames.indexOf( typeName );
	if( m_selectedPhotonMap < 0 )	return 0;

	return 1;
}

/*!
 *	Sets \a rays as the number of rays to trace for each run action.
 */
void MainWindow::SetRaysPerIteration( unsigned long rays )
{
	m_raysPerIteration = rays;
}

/*!
 * Sets to the \a parameter of the node \a nodeUrl the value defined at \a value.
 */
void MainWindow::SetValue( QString nodeUrl, QString parameter, QString value )
{
	if( nodeUrl.isEmpty() || parameter.isEmpty() || value.isEmpty() )
	{
		QMessageBox::information( this, "Tonatiuh",
            "You must define a node url, a parameter name and a value.", 1 );
		return;
	}

	QModelIndex nodeIndex = m_sceneModel->IndexFromNodeUrl( nodeUrl );
	if( !nodeIndex.isValid() )
	{
		QMessageBox::information( this, "Tonatiuh",
            "Defined node url is not a valid url.", 1 );
		return;
	}

	InstanceNode* nodeInstance = m_sceneModel->NodeFromIndex( nodeIndex );
	if( !nodeInstance )	return;

	SoNode* node = nodeInstance->GetNode();
	if( !node )	return;
	if ( node->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		TSeparatorKit* separatorNode = static_cast< TSeparatorKit* >( node );
		SoTransform* nodeTransform = static_cast< SoTransform* >( separatorNode->getPart( "transform", true ) );
		node = nodeTransform;
	}


	SoField* parameterField = node->getField( parameter.toStdString().c_str() );
	if( !parameterField )
	{
		QMessageBox::information( this, "Tonatiuh",
            QString( "Defined node has not contains the parameter %1." ).arg( parameter), 1 );
		return;
	}
	SetParameterValue( node, parameter, value );
}


//Manipulators actions
void MainWindow::SoTransform_to_SoCenterballManip()
{
	//Transform to a SoCenterballManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoCenterballManip * manipulator = new SoCenterballManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoJackManip()
{
	//Transform to a SoJackManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoJackManip * manipulator = new SoJackManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoHandleBoxManip()
{
	//Transform to a SoHandleBoxManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoHandleBoxManip * manipulator = new SoHandleBoxManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTabBoxManip()
{
	//Transform to a SoTabBoxManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoTabBoxManip * manipulator = new SoTabBoxManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTrackballManip()
{
	//Transform to a SoTrackballManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoTrackballManip* manipulator = new SoTrackballManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTransformBoxManip()
{
	//Transform to a SoTransformBoxManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoTransformBoxManip * manipulator = new SoTransformBoxManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());

	coinNode->setPart("transform", manipulator);

	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTransformerManip()
{
	//Transform to a SoTransformerManip manipulator
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransform* transform = static_cast< SoTransform* >( coinNode->getPart( "transform", false ) );

	SoTransformerManip* manipulator = new SoTransformerManip;
	manipulator->rotation.setValue(transform->rotation.getValue());
  	manipulator->translation.setValue(transform->translation.getValue());
   	manipulator->scaleFactor.setValue(transform->scaleFactor.getValue());
   	manipulator->scaleOrientation.setValue(transform->scaleOrientation.getValue());
   	manipulator->center.setValue(transform->center.getValue());


	coinNode->setPart("transform", manipulator);
	ChangeSelection( currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );

}

void MainWindow::SoManip_to_SoTransform()
{
	//Transform manipulator to a SoTransform
	QModelIndex currentIndex = sceneModelView->currentIndex();

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex(currentIndex);
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( instanceNode->GetNode() );
	SoTransformManip* manipulator = static_cast< SoTransformManip* >( coinNode->getPart( "transform", false ) );
	if( !manipulator ) return;

   	SoTransform* transform = new SoTransform;
   	transform->rotation.setValue(manipulator->rotation.getValue());
   	transform->translation.setValue(manipulator->translation.getValue());
   	transform->scaleFactor.setValue(manipulator->scaleFactor.getValue());
   	transform->scaleOrientation.setValue(manipulator->scaleOrientation.getValue());
   	transform->center.setValue(manipulator->center.getValue());

	coinNode->setPart("transform", transform);
	ChangeSelection( currentIndex );

	m_document->SetDocumentModified( true );
}

//for graphicview signals
void MainWindow::selectionFinish( SoSelection* selection )
{
    if(selection->getNumSelected() == 0 ) return;

    SoPath* selectionPath = selection->getPath( 0 );
    if ( !selectionPath ) return;

    if ( !selectionPath->containsNode ( m_document->GetSceneKit() ) ) return;

    SoNodeKitPath* nodeKitPath = static_cast< SoNodeKitPath* >( selectionPath );
    if(nodeKitPath->getTail()->getTypeId().isDerivedFrom(SoDragger::getClassTypeId() ) ) return;

    QModelIndex nodeIndex = m_sceneModel->IndexFromPath( *nodeKitPath );
	if ( !nodeIndex.isValid() ) return;
	m_selectionModel->setCurrentIndex( nodeIndex , QItemSelectionModel::ClearAndSelect );

}

void MainWindow::ChangeSelection( const QModelIndex& current )
{
	InstanceNode* instanceSelected = m_sceneModel->NodeFromIndex( current );
    SoNode* selectedCoinNode = instanceSelected->GetNode();

    if (! selectedCoinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		SoBaseKit* parentNode = static_cast< SoBaseKit* >( instanceSelected->GetParent()->GetNode() );
		SbString partName = parentNode->getPartString( selectedCoinNode );

		if( partName.getLength() == 0 ) partName = "material";
		QStringList parts;
		parts<<QString( partName.getString() );

		parametersView->SelectionChanged( parentNode, parts );

	}
	else
	{
		SoBaseKit* selectedCoinNodeKit = static_cast< SoBaseKit* >( selectedCoinNode );

		QStringList parts;
		parametersView->SelectionChanged( selectedCoinNodeKit, parts );
	}
}


/*!
 * Creates a material node from the \a pTMaterialFactory as current selected node child.
 *
 * If the current node is not a surface type node, the material node will not be created.
 */
void MainWindow::CreateMaterial( TMaterialFactory* pTMaterialFactory )
{
	QModelIndex parentIndex = ( (! sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex() ) ) ?
								m_sceneModel->index( 0, 0, sceneModelView->rootIndex( )):
								sceneModelView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = parentInstance->GetNode();
	if( !parentNode->getTypeId().isDerivedFrom( SoShapeKit::getClassTypeId() ) ) return;

	TShapeKit* shapeKit = static_cast< TShapeKit* >( parentNode );
	TMaterial* material = static_cast< TMaterial* >( shapeKit->getPart( "material", false ) );

    if ( material )
    {
    	QMessageBox::information( this, "Tonatiuh Action",
	                          "This TShapeKit already contains a material node", 1);
	    return;
    }

	material = pTMaterialFactory->CreateTMaterial();
    QString typeName = pTMaterialFactory->TMaterialName();
    material->setName( typeName.toStdString().c_str() );

    CmdInsertMaterial* createMaterial = new CmdInsertMaterial( shapeKit, material, m_sceneModel );
    QString commandText = QString( "Create Material: %1").arg( pTMaterialFactory->TMaterialName().toLatin1().constData() );
    createMaterial->setText(commandText);
    m_commandStack->push( createMaterial );

    UpdateLightDimensions();
    m_document->SetDocumentModified( true );
}

/*!
 * Creates a shape node from the \a pTShapeFactory as current selected node child.
 *
 * If the current node is not a surface type node, the shape node will not be created.
 */
/*!
 * Creates a shape node from the \a pTTrackerFactory as current selected node child.
 *
 */
void MainWindow::CreateShape( TShapeFactory* pTShapeFactory )
{
    QModelIndex parentIndex = ((! sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex())) ?
								m_sceneModel->index (0,0,sceneModelView->rootIndex()) : sceneModelView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = parentInstance->GetNode();
	if( !parentNode->getTypeId().isDerivedFrom( SoShapeKit::getClassTypeId() ) ) return;

	TShapeKit* shapeKit = static_cast< TShapeKit* >( parentNode );
	TShape* shape = static_cast< TShape* >( shapeKit->getPart( "shape", false ) );

    if (shape)
    {
    	QMessageBox::information( this, "Tonatiuh Action",
	                          "This TShapeKit already contains a shape", 1);
    }
    else
    {
    	shape = pTShapeFactory->CreateTShape();
    	shape->setName( pTShapeFactory->TShapeName().toStdString().c_str() );
        CmdInsertShape* createShape = new CmdInsertShape( shapeKit, shape, m_sceneModel );
        QString commandText = QString( "Create Shape: %1" ).arg( pTShapeFactory->TShapeName().toLatin1().constData());
        createShape->setText( commandText );
        m_commandStack->push( createShape );

        UpdateLightDimensions();
        m_document->SetDocumentModified( true );
    }
}


void MainWindow::CreateTracker( TTrackerFactory* pTTrackerFactory )
{
	QModelIndex parentIndex = ((! sceneModelView->currentIndex().isValid() ) || (sceneModelView->currentIndex() == sceneModelView->rootIndex())) ?
									m_sceneModel->index (0,0,sceneModelView->rootIndex()):
									sceneModelView->currentIndex();

	SoSceneKit* scene = m_document->GetSceneKit();

	TTracker* tracker = pTTrackerFactory->CreateTTracker( );
	tracker->SetSceneKit( scene );

	CmdInsertTracker* command = new CmdInsertTracker( tracker, parentIndex, scene, m_sceneModel );
	m_commandStack->push( command );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );
}


//for treeview signals
void MainWindow::mousePressEvent( QMouseEvent * e )
{
	QPoint pos = e->pos();
	int x = pos.x();
	int y = pos.y()-64;

	QSplitter *pSplitter = findChild<QSplitter *>( "graphicHorizontalSplitter" );
	QRect mainViewRect = pSplitter->geometry();

	if( mainViewRect.contains( x, y ) )
	{
		QSplitter *pvSplitter1 = findChild<QSplitter *>( "graphicVerticalSplitter1" );
		QSplitter *pvSplitter2 = findChild<QSplitter *>( "graphicVerticalSplitter1" );
		QRect vViewRect1 = pvSplitter1->geometry();
		if(vViewRect1.contains(x,y))
		{
			QList<int> size(pvSplitter2->sizes());
			if(x < size[0])
				m_focusView = 0;
			if(x > size[0])
				m_focusView = 1;
		}
		else
		{
			QList<int> size(pvSplitter1->sizes());
			if(x < size[0])
				m_focusView = 2;
			if(x > size[0])
				m_focusView = 3;
		}
	}
}


void MainWindow::itemDragAndDrop( const QModelIndex& newParent,  const QModelIndex& node)
{
	if( node == sceneModelView->rootIndex() ) return;

	InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex( node );
	SoNode* coinNode = nodeInstnace->GetNode();
	//if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

	QUndoCommand* dragAndDrop = new QUndoCommand();
	dragAndDrop->setText("Drag and Drop node");
	new CmdCut( node, m_coinNode_Buffer, m_sceneModel, dragAndDrop );

	new CmdPaste( tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDrop );
	m_commandStack->push( dragAndDrop );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );

}

void MainWindow::itemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node)
{
	InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex( node );
	SoNode* coinNode = nodeInstnace->GetNode();
	//if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

	QUndoCommand* dragAndDropCopy = new QUndoCommand();
	dragAndDropCopy->setText("Drag and Drop Copy");
	new CmdCopy( node, m_coinNode_Buffer, m_sceneModel );
	new CmdPaste( tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDropCopy );
	m_commandStack->push( dragAndDropCopy );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );
}

//for sunposdialog signals
/*!
 * Changes the light position to the position computed from \a time, \a longitude and \a latitude.
 * The \a time is ut time and \a longitude and \a latitude are defined in degrees.
 */
void MainWindow::ChangeSunPosition( QDateTime* time, double longitude, double latitude )
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );
	if ( !lightKit )
	{
		QMessageBox::warning( this, "Toantiuh warning", tr( "Sun not defined in scene" ) );
	}
	else
	{
		CmdLightPositionModified* command = new CmdLightPositionModified( lightKit, *time, longitude, latitude );
		m_commandStack->push( command );

		UpdateLightDimensions();
		m_document->SetDocumentModified( true );
	}
	delete time;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if ( OkToContinue() )
    {
    	WriteSettings();
    	event->accept();
    }
    else event->ignore();
}

/*!
 * Creates a \a xDimension by \a zDimension grid and shows in the 3D view. Each grid cell is a \a xSpacing x \a zSpacing.
 */
SoSeparator* MainWindow::CreateGrid( int xDimension, int zDimension, double xSpacing, double zSpacing )
{

	double xWidth = xDimension * xSpacing;
	double zWidth = zDimension * zSpacing;

	int nVertex = xDimension * zDimension + 4;
    SbVec3f* vertex = new SbVec3f[nVertex];

    int nLines = xDimension + zDimension + 2;
    int* lines = new int[nLines];


    int vNum = 0;
    int lNum = 0;
    for( int xIndex = 0; xIndex <= xDimension; ++xIndex )
	{
		double xValue = -( xWidth / 2  ) + xIndex * xSpacing;
		vertex[vNum++].setValue( xValue, 0.0, zWidth/ 2 );
		vertex[vNum++].setValue( xValue, 0.0, -zWidth/ 2 );
		lines[lNum++] = 2;
	}

	vertex[vNum++].setValue( -xWidth / 2, 0.0, zWidth/ 2 );
	vertex[vNum++].setValue( xWidth / 2, 0.0, zWidth/ 2 );
    lines[lNum++] = 2;
	vertex[vNum++].setValue( -xWidth / 2, 0.0, -zWidth/ 2 );
	vertex[vNum++].setValue( xWidth / 2, 0.0, -zWidth/ 2 );
    lines[lNum++] = 2;

    for( int zIndex = 1; zIndex < zDimension; ++zIndex )
	{
		double zValue = -( zWidth / 2  ) + zIndex * zSpacing;
		vertex[vNum++].setValue( xWidth / 2 , 0.0, zValue );
		vertex[vNum++].setValue( -xWidth / 2 , 0.0, zValue );
	    lines[lNum++] = 2;
	}


	SoSeparator * grid = new SoSeparator;

	SoMaterial * mat = new SoMaterial;
	static float colors[2][3] = { {0.4f, 0.4f, 0.4f}, {0.6f, 0.6f, 0.6f} };
	mat->diffuseColor.setValues(0, 2, colors);
	grid->addChild(mat);

	SoCoordinate3 * line_coords = new SoCoordinate3;
	line_coords->point.setValues(0, nVertex, vertex);
	grid->addChild(line_coords);

	SoLineSet* lineset=new SoLineSet;
	lineset->numVertices.setValues(0,lNum,lines);
	grid->addChild(lineset);


    delete[] vertex;
    delete[] lines;
	return grid;

}


/*!
 * Creates a toolbar for insert material actions.
 */
QToolBar* MainWindow::CreateMaterialsTooBar( QMenu* pMaterialsMenu )
{
	QToolBar* pMaterialsToolBar = new QToolBar( pMaterialsMenu );
	if (! pMaterialsToolBar ) tgf::SevereError( "MainWindow::SetupToolBars: NULL pMaterialsToolBar" );
	pMaterialsToolBar->setObjectName( QString::fromUtf8("materialsToolBar" ) );
	pMaterialsToolBar->setOrientation( Qt::Horizontal );
	pMaterialsToolBar->setWindowTitle( QLatin1String( "Materials" ) );
	addToolBar( pMaterialsToolBar );
	return pMaterialsToolBar;
}

/*!
 * Creates a toolbar for insert trackers actions.
 */
QToolBar* MainWindow::CreateTrackerTooBar( QMenu* pTrackersMenu )
{
	QToolBar* pTrackersToolBar = new QToolBar( pTrackersMenu );
	if (! pTrackersToolBar ) tgf::SevereError( "MainWindow::SetupToolBars: NULL pTrackersToolBar" );
	pTrackersToolBar->setObjectName( QString::fromUtf8("materialsToolBar" ) );
	pTrackersToolBar->setOrientation( Qt::Horizontal );
	pTrackersToolBar->setWindowTitle( QLatin1String( "Trackers" ) );
	addToolBar( pTrackersToolBar );
	return pTrackersToolBar;
}

/*!
 * Creates a new delete command, where the \a index node was deleted.
  *
 * Returns \a true if the node was successfully deleted, otherwise returns \a false.
 */
bool MainWindow::Delete( QModelIndex index )
{

	if( !index.isValid()) return false;
	if( index == sceneModelView->rootIndex() ) return false;
	if( index.parent() == sceneModelView->rootIndex() ) return false;

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex( index );
	SoNode* coinNode = instanceNode->GetNode();

	if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )
	{
		CmdDeleteTracker* commandDelete = new CmdDeleteTracker( index, m_document->GetSceneKit(), *m_sceneModel );
		m_commandStack->push( commandDelete );
	}
	else
	{
		CmdDelete* commandDelete = new CmdDelete( index, *m_sceneModel );
		m_commandStack->push( commandDelete );
	}


	UpdateLightDimensions();
	m_document->SetDocumentModified( true );

	return true;
}

/*!
 * Return horizontalSplitter splitter object.
 */
QSplitter* MainWindow::GetHorizontalSplitterPointer()
{
    QSplitter* pSplitter = findChild< QSplitter* >( "horizontalSplitter" );
    if( !pSplitter ) tgf::SevereError( "MainWindow::GetSceneModelViewPointer: splitter not found" );
    return pSplitter;
}

/*!
 * Returns \a true if the application is ready to start with other model. Otherwise,
 * returns \a false.
 */
bool MainWindow::OkToContinue()
{
	if ( m_document->IsModified() )
	{
		int answer = QMessageBox::warning( this, tr( "Tonatiuh" ),
		                 tr( "The document has been modified.\n"
		                     "Do you want to save your changes?"),
		                 QMessageBox::Yes | QMessageBox::Default,
		                 QMessageBox::No,
		                 QMessageBox::Cancel | QMessageBox::Escape );

		if ( answer == QMessageBox::Yes ) return Save();
		else if ( answer == QMessageBox::Cancel ) return false;
	}
	return true;
}

/*!
 * Creates a new \a type paste command. The clipboard node is inserted as \a nodeIndex node
 * child.
 *
 * Returns \a true if the node was successfully pasted, otherwise returns \a false.
 */
bool MainWindow::Paste( QModelIndex nodeIndex, tgc::PasteType type )
{
	if( !nodeIndex.isValid() ) return false;
	if( !m_coinNode_Buffer ) return false;

	InstanceNode* ancestor = m_sceneModel->NodeFromIndex( nodeIndex );
	SoNode* selectedCoinNode = ancestor->GetNode();

	if ( !selectedCoinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) return false;
	if ( ( selectedCoinNode->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
	( m_coinNode_Buffer->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) ) )	return false;

	if( ancestor->GetNode() == m_coinNode_Buffer)  return false;
	while( ancestor->GetParent() )
	{
		if(ancestor->GetParent()->GetNode() == m_coinNode_Buffer )	return false;
		ancestor = ancestor->GetParent();
	}

	CmdPaste* commandPaste = new CmdPaste( type, m_selectionModel->currentIndex(), m_coinNode_Buffer, *m_sceneModel );
	m_commandStack->push( commandPaste );

	UpdateLightDimensions();
	m_document->SetDocumentModified( true );
	return true;

}

/*!
 * Returns the directory of where the plugins are saved.
 */
QDir MainWindow::PluginDirectory()
{
	// Returns the path to the top level plug-in directory.
	// It is assumed that this is a subdirectory named "plugins" of the directory in
	// which the running version of Tonatiuh is located.

    QDir directory( qApp->applicationDirPath() );
  	directory.cd( "plugins" );
	return directory;
}

/**
 * Restores application settings.
 **/
void MainWindow::ReadSettings()
{
    QSettings settings( "NREL UTB CENER", "Tonatiuh" );
    QRect rect = settings.value( "geometry", QRect(200, 200, 400, 400 ) ).toRect();
    move( rect.topLeft() );
    resize( rect.size() );

    setWindowState( Qt::WindowNoState );
    if( settings.value( "windowNoState", false ).toBool() )	setWindowState( windowState() ^ Qt::WindowNoState );
    if( settings.value( "windowMinimized", false ).toBool() )	setWindowState( windowState() ^ Qt::WindowMinimized );
    if( settings.value( "windowMaximized", true ).toBool() )	setWindowState( windowState() ^ Qt::WindowMaximized );
    if( settings.value( "windowFullScreen", false ).toBool() )	setWindowState( windowState() ^ Qt::WindowFullScreen );
    if( settings.value( "windowActive", false ).toBool() )	setWindowState( windowState() ^ Qt::WindowActive );
    m_recentFiles = settings.value( "recentFiles" ).toStringList();
    UpdateRecentFileActions();

}

/*!
 * Checks whether a ray tracing can be started with the current light and model.
 */
bool MainWindow::ReadyForRaytracing( InstanceNode*& rootSeparatorInstance, InstanceNode*& lightInstance, SoTransform*& lightTransform, TSunShape*& sunShape, TShape*& raycastingShape )
{
	//Check if there is a scene
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if ( !coinScene )  return false;

	//Check if there is a rootSeparator InstanceNode
	InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
	if ( !sceneInstance )  return false;
	rootSeparatorInstance = sceneInstance->children[1];
	if( !rootSeparatorInstance ) return false;

	//Check if there is a light and is properly configured
	if ( !coinScene->getPart( "lightList[0]", false ) )	return false;
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );

	lightInstance = sceneInstance->children[0];
	if ( !lightInstance ) return false;

	if( !lightKit->getPart( "tsunshape", false ) ) return false;
	sunShape = static_cast< TSunShape * >( lightKit->getPart( "tsunshape", false ) );

	if( !lightKit->getPart( "icon", false ) ) return false;
	raycastingShape = static_cast< TShape * >( lightKit->getPart( "icon", false ) );

	if( !lightKit->getPart( "transform" ,true ) ) return false;
	lightTransform = static_cast< SoTransform * >( lightKit->getPart( "transform" ,true ) );


	QVector< RandomDeviateFactory* > randomDeviateFactoryList = m_pluginManager->GetRandomDeviateFactories();
	QVector< TPhotonMapFactory* > photonmapFactoryList = m_pluginManager->GetPhotonMapFactories();

	//Check if there is a random generator selected;
	if( m_selectedRandomDeviate == -1 )
	{
		if( randomDeviateFactoryList.size() > 0 ) m_selectedRandomDeviate = 0;
		else	return false;
	}

	//Create the random generator
	if( !m_rand )	m_rand =  randomDeviateFactoryList[m_selectedRandomDeviate]->CreateRandomDeviate();

	//Check if there is a photon map type selected;
	if( m_selectedPhotonMap == -1 )
	{
		if( photonmapFactoryList.size() > 0 ) m_selectedPhotonMap = 0;
		else	return false;
	}

	//Create the photon map where photons are going to be stored
	if( !m_increasePhotonMap )
	{
		delete m_photonMap;
		m_photonMap = 0;
	}

	if( !m_photonMap )
	{
		QVector< TPhotonMapFactory* > photonmapFactoryList = m_pluginManager->GetPhotonMapFactories();

		m_photonMap = photonmapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
		m_tracedRays = 0;
	}

	return true;
}

/*!
 * Returns \a true if the tonatiuh model is correctly saved into the the given \a fileName. Otherwise, returns \a false.
 *
 * \sa Save, SaveAs.
 */
bool MainWindow::SaveFile( const QString& fileName )
{
 	if( !m_document->WriteFile( fileName ) )
	{
		statusBar()->showMessage( tr( "Saving canceled" ), 2000 );
		return false;
	}

	SetCurrentFile( fileName );
	statusBar()->showMessage( tr( "File saved" ), 2000 );
	return true;
}

/*!
 * Sets \a fileName files as current file.
 */
void MainWindow::SetCurrentFile( const QString& fileName )
{
	m_currentFile = fileName;
	m_document->SetDocumentModified( false );

	QString shownName = "Untitled";
	if ( !m_currentFile.isEmpty() )
	{
		shownName = StrippedName( m_currentFile );
		m_recentFiles.removeAll( m_currentFile );
		m_recentFiles.prepend( m_currentFile );
		UpdateRecentFileActions();
	}

	setWindowTitle( tr( "%1[*] - %2" ).arg( shownName ).arg( tr( "Tonatiuh" ) ) );
}

/*!
 * Creates actions form recent files.
 */
void MainWindow::SetupActions()
{
    m_recentFileActions = new QAction*[m_maxRecentFiles];
    for ( int i = 0; i < m_maxRecentFiles; ++i )
    {
    	m_recentFileActions[i] = new QAction( this );
    	m_recentFileActions[i]->setVisible( false );
    	connect( m_recentFileActions[i], SIGNAL( triggered() ),
    			                   this, SLOT( OpenRecentFile() ) );
    }
}

void MainWindow::SetupActionsInsertMaterial()
{
	QVector< TMaterialFactory* > materialsFactoryList = m_pluginManager->GetMaterialFactories();
	if( !( materialsFactoryList.size() > 0 ) )	return;

	QMenu* pMaterialsMenu = menuInsert->findChild< QMenu* >( "menuMaterial" );
	if( !pMaterialsMenu ) return;
	if( pMaterialsMenu->isEmpty() )
	{
		//Enable material menu
		pMaterialsMenu->setEnabled( true );
		m_materialsToolBar = CreateMaterialsTooBar( pMaterialsMenu );
	}

	for( int i = 0; i < materialsFactoryList.size(); ++i )
	{
		TMaterialFactory* pTMaterialFactory = materialsFactoryList[i];

		ActionInsertMaterial* actionInsertMaterial = new ActionInsertMaterial( pTMaterialFactory->TMaterialName(), this, pTMaterialFactory );
	    actionInsertMaterial->setIcon( pTMaterialFactory->TMaterialIcon() );

	    pMaterialsMenu->addAction( actionInsertMaterial );
		m_materialsToolBar->addAction( actionInsertMaterial );
		m_materialsToolBar->addSeparator();
	    connect( actionInsertMaterial, SIGNAL( triggered() ),
	    		 actionInsertMaterial, SLOT( OnActionInsertMaterialTriggered() ) );
		connect( actionInsertMaterial, SIGNAL( CreateMaterial( TMaterialFactory* ) ),
				                 this, SLOT( CreateMaterial( TMaterialFactory* ) ) );

	}

}

/**
 * Creates an action for the /a pTShapeFactory and adds to shape insert menu and toolbar.
 */
void MainWindow::SetupActionsInsertShape()
{
	QVector< TShapeFactory* > shapeFactoryList = m_pluginManager->GetShapeFactories();
	if( !( shapeFactoryList.size() > 0 ) )	return;

    QMenu* menuShape = menuInsert->findChild< QMenu* >( "menuShape" );
    if( !menuShape ) return;
   	if( menuShape->isEmpty() )
    {
   	  	//Enable Shape menu
   		menuShape->setEnabled( true );

    	//Create a new toolbar for trackers
    	m_shapeToolBar = new QToolBar( menuShape );
		if( m_shapeToolBar )
		{
			m_shapeToolBar->setObjectName( QString::fromUtf8( "shapeToolBar" ) );
			m_shapeToolBar->setOrientation( Qt::Horizontal );
	    	addToolBar( m_shapeToolBar );
		}
		else tgf::SevereError( "MainWindow::SetupToolBars: NULL m_trackersToolBar" );
    }
   	for( int i = 0; i < shapeFactoryList.size(); ++i )
   	{
   		TShapeFactory* pTShapeFactory = shapeFactoryList[i];
   		ActionInsertShape* actionInsertShape = new ActionInsertShape( pTShapeFactory->TShapeName(), this, pTShapeFactory );
   		actionInsertShape->setIcon( pTShapeFactory->TShapeIcon() );

   		menuShape->addAction( actionInsertShape );
   		m_shapeToolBar->addAction( actionInsertShape );
   		m_shapeToolBar->addSeparator();
   		connect( actionInsertShape, SIGNAL( triggered() ), actionInsertShape, SLOT( OnActionInsertShapeTriggered() ) );
   		connect( actionInsertShape, SIGNAL( CreateShape( TShapeFactory* ) ), this, SLOT( CreateShape(TShapeFactory*) ) );
   	}
}

void MainWindow::SetupActionsInsertTracker()
{
	QVector< TTrackerFactory* > trackerFactoryList = m_pluginManager->GetTrackerFactories();
	if( !( trackerFactoryList.size() > 0 ) )	return;

	QMenu* pTrackerMenu = menuInsert->findChild< QMenu* >( "menuTracker" );
	if( !pTrackerMenu ) return;
	if( pTrackerMenu->isEmpty() )
	{
		//Enable material menu
		pTrackerMenu->setEnabled( true );

		//Create a new toolbar for trackers
		m_trackersToolBar = CreateTrackerTooBar( pTrackerMenu );
	}

	for( int i = 0; i < trackerFactoryList.size(); ++i )
	{
		TTrackerFactory* pTTrackerFactory = trackerFactoryList[i];
		ActionInsertTracker* actionInsertTracker = new ActionInsertTracker( pTTrackerFactory->TTrackerName(), this, pTTrackerFactory );
		actionInsertTracker->setIcon( pTTrackerFactory->TTrackerIcon() );

		pTrackerMenu->addAction( actionInsertTracker );
		m_trackersToolBar->addAction( actionInsertTracker );
		m_trackersToolBar->addSeparator();
		connect( actionInsertTracker, SIGNAL( triggered() ), actionInsertTracker, SLOT( OnActionInsertTrackerTriggered() ) );
		connect( actionInsertTracker, SIGNAL( CreateTracker( TTrackerFactory* ) ), this, SLOT( CreateTracker(TTrackerFactory*) ) );
	}
}

/*!
 * Creates a view for visualize user done last actions.
 */
void MainWindow::SetupCommandView()
{
    m_commandStack = new QUndoStack(this);
	m_commandView = new QUndoView( m_commandStack );
	m_commandView->setWindowTitle( tr( "Command List" ) );
	m_commandView->setAttribute( Qt::WA_QuitOnClose, false );
    connect( m_commandStack, SIGNAL( canRedoChanged( bool ) ),
    		     actionRedo, SLOT( setEnabled( bool ) ) );
    connect( m_commandStack, SIGNAL( canUndoChanged( bool ) ),
    		     actionUndo, SLOT( setEnabled( bool ) ) );
}

/*!
 * Initializes tonatiuh document object.
 */
void MainWindow::SetupDocument()
{
    m_document = new Document();
    if ( m_document )
    	connect( m_document, SIGNAL( selectionFinish( SoSelection* ) ),
    			       this, SLOT(selectionFinish( SoSelection* ) ) );
    else tgf::SevereError( "MainWindow::SetupDocument: Fail to create new document" );
}

/*!
 * Initializates tonatiuh graphic view.
 */
void MainWindow::SetupGraphicView()
{

    SetupVRMLBackground();
	QSplitter* pSplitter = findChild<QSplitter *>( "horizontalSplitter" );

	QSplitter* graphicHorizontalSplitter = new QSplitter();
	graphicHorizontalSplitter->setObjectName( QString::fromUtf8("graphicHorizontalSplitter") );
    graphicHorizontalSplitter->setOrientation( Qt::Vertical );
    pSplitter->insertWidget( 0, graphicHorizontalSplitter );

	QList<int> sizes;
    sizes<<500<<200;
    pSplitter->setSizes ( sizes );


    QSplitter *graphicVerticalSplitter1 = new QSplitter();
    graphicVerticalSplitter1->setObjectName( QString::fromUtf8( "graphicVerticalSplitter1" ) );
    graphicVerticalSplitter1->setOrientation( Qt::Horizontal );
    graphicHorizontalSplitter->insertWidget( 0, graphicVerticalSplitter1 );

    QSplitter *graphicVerticalSplitter2 = new QSplitter();
    graphicVerticalSplitter2->setObjectName(QString::fromUtf8("graphicVerticalSplitter2"));
    graphicVerticalSplitter2->setOrientation(Qt::Horizontal);
    graphicHorizontalSplitter->insertWidget( 1, graphicVerticalSplitter2 );

    QList<int> height;
    height<<200<<200;
    graphicHorizontalSplitter->setSizes ( height );

    GraphicView* graphicView1 = new GraphicView(graphicVerticalSplitter1);
    graphicView1->setObjectName(QString::fromUtf8("graphicView1"));
    m_graphicView.push_back(graphicView1);
    GraphicView* graphicView2 = new GraphicView(graphicVerticalSplitter1);
    graphicView2->setObjectName(QString::fromUtf8("graphicView2"));
	m_graphicView.push_back(graphicView2);
    GraphicView* graphicView3 = new GraphicView(graphicVerticalSplitter2);
    graphicView3->setObjectName(QString::fromUtf8("graphicView3"));
	m_graphicView.push_back(graphicView3);
    GraphicView* graphicView4 = new GraphicView(graphicVerticalSplitter2);
    graphicView4->setObjectName(QString::fromUtf8("graphicView4"));
	m_graphicView.push_back(graphicView4);

    graphicVerticalSplitter1->addWidget(m_graphicView[0]);
    graphicVerticalSplitter1->addWidget(m_graphicView[1]);
    graphicVerticalSplitter2->addWidget(m_graphicView[2]);
    graphicVerticalSplitter2->addWidget(m_graphicView[3]);

    QList<int> widthSizes;
    widthSizes<<100<<100;
    graphicVerticalSplitter1->setSizes ( widthSizes );
    graphicVerticalSplitter2->setSizes ( widthSizes );

    if( graphicVerticalSplitter1 && graphicVerticalSplitter2 )
	{
		m_graphicView[0] = graphicVerticalSplitter1->findChild< GraphicView* >( "graphicView1" );
        if ( m_graphicView[0] != NULL )
        {
    	    m_graphicView[0]->resize( 600, 400 );
    	    m_graphicView[0]->SetSceneGraph( m_document->GetRoot( ) );
    	    m_graphicView[0]->setModel( m_sceneModel );
    	    m_graphicView[0]->setSelectionModel( m_selectionModel );
        }
        else tgf::SevereError( "MainWindow::InitializeGraphicView: graphicView[0] not found" );
        m_graphicView[1] = graphicVerticalSplitter1->findChild< GraphicView* >( "graphicView2" );
        if ( m_graphicView[1] != NULL )
        {
    	    m_graphicView[1]->resize( 600, 400 );
    	    m_graphicView[1]->SetSceneGraph( m_document->GetRoot( ) );
    	    m_graphicView[1]->setModel( m_sceneModel );
    	    m_graphicView[1]->setSelectionModel( m_selectionModel );
    	    m_focusView=1;
    	    on_action_X_Y_Plane_triggered();

        }
        else tgf::SevereError( "MainWindow::InitializeGraphicView: graphicView[1] not found" );
        m_graphicView[2] = graphicVerticalSplitter2->findChild< GraphicView* >( "graphicView3" );
        if ( m_graphicView[2] != NULL )
        {
    	    m_graphicView[2]->resize( 600, 400 );
    	    m_graphicView[2]->SetSceneGraph( m_document->GetRoot( ) );
    	    m_graphicView[2]->setModel( m_sceneModel );
    	    m_graphicView[2]->setSelectionModel( m_selectionModel );
    	    m_focusView=2;
    	    on_action_Y_Z_Plane_triggered();
        }
        else tgf::SevereError( "MainWindow::InitializeGraphicView: graphicView[2] not found" );
        m_graphicView[3] = graphicVerticalSplitter2->findChild< GraphicView* >( "graphicView4" );
        if ( m_graphicView[3] !=  NULL )
        {
    	    m_graphicView[3]->resize( 600, 400 );
    	    m_graphicView[3]->SetSceneGraph( m_document->GetRoot( ) );
    	    m_graphicView[3]->setModel( m_sceneModel );
    	    m_graphicView[3]->setSelectionModel( m_selectionModel );
    	    m_focusView=3;
    	    on_action_X_Z_Plane_triggered();
        }
	    else tgf::SevereError( "MainWindow::InitializeGraphicView: graphicView[3] not found" );
    }
	else tgf::SevereError( "MainWindow::InitializeGraphicView: verticalSplitter not found" );

	m_graphicView[1]->hide();
	m_graphicView[2]->hide();
	m_graphicView[3]->hide();
	m_focusView = 0;
}

/*!
 * Creates a menu for last used files
 */
void MainWindow::SetupMenus()
{
    for ( int i = 0; i < m_maxRecentFiles; ++i )
          menuRecent->addAction( m_recentFileActions[i] );
}

/*!
 * Initializes Tonatiuh models.
 */
void MainWindow::SetupModels()
{
    m_sceneModel = new SceneModel();
    m_sceneModel->SetCoinRoot( *m_document->GetRoot() );
    m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
    m_selectionModel = new QItemSelectionModel( m_sceneModel );

    connect( m_sceneModel, SIGNAL( LightNodeStateChanged( int ) ),
    		         this, SLOT( SetSunPositionCalculatorEnabled( int ) ) );
}

/*!
 *Initializes the tonatiuh objects parameters view.
 */
void MainWindow::SetupParametersView()
{
    connect ( parametersView, SIGNAL( valueModificated( SoNode*, QString, QString  ) ),
		                this, SLOT( SetParameterValue( SoNode*, QString, QString  ) ) );

	connect( m_selectionModel, SIGNAL( currentChanged ( const QModelIndex& , const QModelIndex& ) ),
			             this, SLOT( ChangeSelection( const QModelIndex& ) ) );
}

/*!
 * Initializes plugin manager and load available plugins.
 */
void MainWindow::SetupPluginsManager()
{
	m_pluginManager = new PluginManager;
	m_pluginManager->LoadAvailablePlugins( PluginDirectory() );

	SetupActionsInsertMaterial( );
	SetupActionsInsertShape();
	SetupActionsInsertTracker();

}

/*!
 * Initializates Tonatiuh objectes tree view.
 */
void MainWindow::SetupTreeView()
{

	sceneModelView->setModel( m_sceneModel );
	sceneModelView->setSelectionModel( m_selectionModel );


	connect( sceneModelView, SIGNAL( dragAndDrop( const QModelIndex&, const QModelIndex& ) ),
			 this, SLOT ( itemDragAndDrop( const QModelIndex&, const QModelIndex& ) ) );
	connect( sceneModelView, SIGNAL( dragAndDropCopy( const QModelIndex&, const QModelIndex& ) ),
			 this, SLOT ( itemDragAndDropCopy( const QModelIndex&, const QModelIndex& ) ) );
	connect( sceneModelView, SIGNAL( showMenu( const QModelIndex& ) ),
				 this, SLOT ( ShowMenu( const QModelIndex& ) ) );
	connect( sceneModelView, SIGNAL( nodeNameModificated( const QModelIndex&, const QString& ) ),
				 this, SLOT ( ChangeNodeName( const QModelIndex&, const QString& ) ) );

}

/*!
 * Defines slots function for main window signals.
 */
void MainWindow::SetupTriggers()
{
	//File actions
	connect( actionNew, SIGNAL( triggered() ), this, SLOT ( New() ) );
	connect( actionOpen, SIGNAL( triggered() ), this, SLOT ( Open() ) );
	connect( actionSave, SIGNAL( triggered() ), this, SLOT ( Save() ) );
	connect( actionSaveAs, SIGNAL( triggered() ), this, SLOT ( SaveAs() ) );
	connect( actionSaveComponent, SIGNAL( triggered() ), this, SLOT ( SaveComponent() ) );
	connect( actionClose, SIGNAL( triggered() ), this, SLOT ( close() ) );

	//Edit actions
	connect( actionUndo, SIGNAL( triggered() ), this, SLOT ( Undo() ) );
	connect( actionRedo, SIGNAL( triggered() ), this, SLOT ( Redo() ) );
	connect( actionUndoView, SIGNAL( triggered() ), this, SLOT ( ShowCommandView() ) );
	connect( actionCut, SIGNAL( triggered() ), this, SLOT ( Cut() ) );
	connect( actionCopy, SIGNAL( triggered() ), this, SLOT ( Copy() ) );
	connect( actionPasteCopy, SIGNAL( triggered() ), this, SLOT ( PasteCopy() ) );
	connect( actionPasteLink, SIGNAL( triggered() ), this, SLOT ( PasteLink() ) );
	connect( actionDelete, SIGNAL( triggered() ), this, SLOT ( Delete() ) );

	//Insert actions
	connect( actionNode, SIGNAL( triggered() ), this, SLOT ( CreateGroupNode() ) );
	connect( actionSurfaceNode, SIGNAL( triggered() ), this, SLOT ( CreateSurfaceNode() ) );
	connect( actionUserComponent, SIGNAL( triggered() ), this, SLOT ( InsertUserDefinedComponent() ) );

	//Sun Light menu actions
	connect( actionDefineSunLight, SIGNAL( triggered() ), this, SLOT ( DefineSunLight() ) );
	connect( actionCalculateSunPosition, SIGNAL( triggered() ), this, SLOT ( CalculateSunPosition() ) );

	//Ray trace menu actions
	connect( actionDisplayRays, SIGNAL( toggled( bool ) ), this, SLOT ( DisplayRays( bool ) ) );
	connect( actionRun, SIGNAL( triggered() ), this, SLOT ( Run() ) );
	connect( actionExportPhotonMap, SIGNAL( triggered() ), this, SLOT( ExportPhotonMap() ) );
	connect( actionRayTraceOptions, SIGNAL( triggered() ), this, SLOT( ShowRayTracerOptionsDialog() )  );
}

/*!
 * Initializates tonatiuh update manager.
 */
void MainWindow::SetupUpdateManager()
{
	m_updateManager = new UpdatesManager( qApp->applicationVersion() );
}

/*!
 * Starts MainWindow views.
 */
void MainWindow::SetupViews()
{
    SetupCommandView();
    SetupGraphicView();
   	SetupTreeView();
   	SetupParametersView();
}

/*!
 * Defines 3D view background.
 */
void MainWindow::SetupVRMLBackground()
{
    SoVRMLBackground* vrmlBackground = new SoVRMLBackground;
    float gcolor[][3] = { {0.9843, 0.8862, 0.6745},{ 0.7843, 0.6157, 0.4785 } };
    float gangle= 1.570f;
    vrmlBackground->groundColor.setValues( 0, 6, gcolor );
    vrmlBackground->groundAngle.setValue( gangle );
    float scolor[][3] = { {0.0157, 0.0235, 0.4509}, {0.5569, 0.6157, 0.8471} };
    float sangle= 1.570f;
    vrmlBackground->skyColor.setValues( 0,6,scolor );
    vrmlBackground->skyAngle.setValue( sangle );
	m_document->GetRoot()->insertChild( vrmlBackground, 0 );
}

/*!
 * Shows the rays and photons stored at the photon map in the 3D view.
 */
void MainWindow::ShowRaysIn3DView()
{
	actionDisplayRays->setEnabled( false );
	actionDisplayRays->setChecked( false );

	if( m_document->GetRoot()->getChild( 0 )->getName() == "Rays"  ) m_document->GetRoot()->removeChild( 0 );

	if( m_pRays )
	{
		m_pRays->removeAllChildren();
		if ( m_pRays->getRefCount() > 1 ) tgf::SevereError("ShowRaysIn3DView: m_pRays referenced in excess ");
		m_pRays->unref();
		m_pRays = 0;
	}

	if( m_fraction > 0.0 || m_drawPhotons )
	{
		m_pRays = new SoSeparator;
		m_pRays->ref();
		m_pRays->setName( "Rays" );

		if( m_drawPhotons )
		{
			SoSeparator* points = trf::DrawPhotonMapPoints(*m_photonMap);
			m_pRays->addChild(points);
		}
		if( m_fraction > 0.0 )
		{
			SoSeparator* currentRays = trf::DrawPhotonMapRays(*m_photonMap, m_tracedRays, m_fraction );
			if( currentRays )
			{
				m_pRays->addChild(currentRays);

				actionDisplayRays->setEnabled( true );
				actionDisplayRays->setChecked( true );
			}

		}
	}
}

/*!
 * Returns to the start origin state and starts with a new model defined in \a fileName.
 * If the file name is not defined, it starts with an empty scene.
 */
bool MainWindow::StartOver( const QString& fileName )
{
	InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
	InstanceNode* concentratorRoot = sceneInstance->children[ sceneInstance->children.size() -1 ];
	m_selectionModel->setCurrentIndex( m_sceneModel->IndexFromNodeUrl( concentratorRoot->GetNodeURL() ), QItemSelectionModel::ClearAndSelect );

	actionDisplayRays->setEnabled( false );
	actionDisplayRays->setChecked( false );

	if( m_document->GetRoot()->getChild( 0 )->getName() == "Rays"  ) m_document->GetRoot()->removeChild( 0 );

	if( m_pRays )
	{
		m_pRays->removeAllChildren();
		if ( m_pRays->getRefCount() > 1 ) tgf::SevereError("StartOver: m_pRays referenced in excess ");
		m_pRays->unref();
		m_pRays = 0;
	}

	m_commandStack->clear();
	m_sceneModel->Clear();

	SetSunPositionCalculatorEnabled( 0 );

	QStatusBar* statusbar = new QStatusBar;
	setStatusBar( statusbar );

    if( fileName.isEmpty() )
    {
    	m_document->New();
    	statusbar->showMessage( tr( "New file" ), 2000 );
    }
    else
    {
    	if( !m_document->ReadFile( fileName ) )
		{
			statusBar()->showMessage( tr( "Loading canceled" ), 2000 );
			return false;
		}
       statusbar->showMessage( tr( "File loaded" ), 2000 );
    }

    SetCurrentFile( fileName );
	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
	sceneInstance = m_sceneModel->NodeFromIndex( sceneModelView->rootIndex() );
	concentratorRoot = sceneInstance->children[ sceneInstance->children.size() -1 ];
	m_selectionModel->setCurrentIndex( m_sceneModel->IndexFromNodeUrl( concentratorRoot->GetNodeURL() ), QItemSelectionModel::ClearAndSelect );


    return true;
}

/*!
 * Returns the \a fullFileName file�s name, without path.
 */
QString MainWindow::StrippedName( const QString& fullFileName )
{
	return QFileInfo( fullFileName ).fileName();
}


/*!
 * Updates the light dimensions if the automatic light dimensions is defined.
 * Otherwise, nothing is done.
 */
void MainWindow::UpdateLightDimensions()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();

	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );
	if ( !lightKit )	return;

	if( !lightKit->automaticallyResizable.getValue() )	return;

	TSeparatorKit* concentratorRoot = static_cast< TSeparatorKit* >( coinScene->getPart( "childList[0]", true ) );
	if ( !concentratorRoot )	return;

	SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
	concentratorRoot->getBoundingBox( bbAction );

	SbBox3f box = bbAction->getXfBoundingBox().project();
	delete bbAction;

	Point3D pMin( box.getMin()[0], box.getMin()[1], box.getMin()[2] );
	Point3D pMax( box.getMax()[0], box.getMax()[1], box.getMax()[2] );

	BBox sceneBox( pMin, pMax );

	lightKit->ResizeToBBox( sceneBox );

}

/*!
 * Updates the recently opened files actions list.
 */
void MainWindow::UpdateRecentFileActions()
{
	QMutableStringListIterator iterator( m_recentFiles );
	while ( iterator.hasNext() )
	{
		if ( !QFile::exists( iterator.next() ) ) iterator.remove();
	}

	for ( int j = 0; j < m_maxRecentFiles; ++j )
	{
		if ( j < m_recentFiles.count() )
		{
			QString text = tr( "&%1 %2" )
			               .arg( j + 1 )
			               .arg( StrippedName( m_recentFiles[j] ) );
			m_recentFileActions[j]->setText( text );
			m_recentFileActions[j]->setData( m_recentFiles[j] );
			m_recentFileActions[j]->setVisible( true );
		}
		else m_recentFileActions[j]->setVisible( false );
	}
}

/*!
 * Saves application settings.
 */
void MainWindow::WriteSettings()
{
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	settings.setValue( "geometry", geometry() );

	Qt::WindowStates states = windowState();
	if( states.testFlag( Qt::WindowNoState ) )	settings.setValue( "windowNoState", true );
	else	settings.setValue( "windowNoState", false );
	if( states.testFlag( Qt::WindowMinimized ) )	settings.setValue( "windowMinimized", true );
	else	settings.setValue( "windowMinimized", false );
	if( states.testFlag( Qt::WindowMaximized ) )	settings.setValue( "windowMaximized", true );
	else	settings.setValue( "windowMaximized", false );
	if( states.testFlag( Qt::WindowFullScreen ) )	settings.setValue( "windowFullScreen", true );
	else	settings.setValue( "windowFullScreen", false );
	if( states.testFlag( Qt::WindowActive ) )	settings.setValue( "windowActive", true );
	else	settings.setValue( "windowActive", false );

    settings.setValue( "recentFiles", m_recentFiles );
}
