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
#include "CmdParameterModified.h"
#include "CmdPaste.h"
#include "Document.h"
#include "ExportDialog.h"
#include "GraphicView.h"
#include "GridSettingsDialog.h"
#include "InstanceNode.h"
#include "MainWindow.h"
#include "NodeNameDelegate.h"
#include "ProgressUpdater.h"
#include "RandomDeviate.h"
#include "RandomDeviateFactory.h"
#include "Ray.h"
#include "RayTraceDialog.h"
#include "RayTracer.h"
#include "SceneModel.h"
#include "LightDialog.h"
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

#include "Trace.h"

void createPhotonMap( TPhotonMap*& photonMap, QPair< TPhotonMap* , std::vector< Photon* > > photons )
{
	if( !photonMap )  photonMap = photons.first;

	std::vector<Photon*> photonsVector = photons.second;
	std::vector<Photon*>::iterator it;

	for( it = photonsVector.begin(); it<photonsVector.end() ; it++)
	{
		Photon* p = *it;
		photonMap->StoreRay( p );
	}

	//photonsVector.clear();
}

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

MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags ),
m_currentFile( 0 ),
m_recentFiles( 0 ),
m_recentFileActions( 0 ),
m_document( 0 ),
m_commandStack( 0 ),
m_commandView( 0 ),
m_materialsToolBar( 0 ),
m_photonMapToolBar(0),
m_shapeToolBar( 0 ),
m_trackersToolBar( 0 ),
m_RandomDeviateFactoryList( 0 ),
m_TPhotonMapFactoryList( 0 ),
m_TFlatShapeFactoryList( 0 ),
m_TSunshapeFactoryList( 0 ),
m_sceneModel( 0 ),
m_selectionModel( 0 ),
m_rand( 0 ),
m_selectedRandomDeviate( -1 ),
m_photonMap( 0 ),
m_selectedPhotonMap( -1 ),
m_increasePhotonMap( false ),
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
m_treeView( 0 ),
m_focusView( 0 )
{
	setupUi( this );
    SetupActions();
    SetupMenus();
    SetupDocument();
    SetupModels();
	SetupViews();
    LoadAvailablePlugins();
    ReadSettings();
}

MainWindow::~MainWindow()
{
	delete m_document;
	delete m_rand;
	delete[] m_recentFileActions;
	delete m_photonMap;
}

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

/**
 * Creates a menu for last used files
 **/
void MainWindow::SetupMenus()
{
    for ( int i = 0; i < m_maxRecentFiles; ++i )
          menuRecent->addAction( m_recentFileActions[i] );
}

void MainWindow::SetupDocument()
{
    m_document = new Document();
    if ( m_document )
    	connect( m_document, SIGNAL( selectionFinish( SoSelection* ) ),
    			       this, SLOT(selectionFinish( SoSelection* ) ) );
    else tgf::SevereError( "MainWindow::SetupDocument: Fail to create new document" );
}

void MainWindow::SetupModels()
{
    m_sceneModel = new SceneModel();
    m_sceneModel->SetCoinRoot( *m_document->GetRoot() );
    m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
    m_selectionModel = new QItemSelectionModel( m_sceneModel );

    connect( m_sceneModel, SIGNAL( LightNodeStateChanged( int ) ),
    		         this, SLOT( SetEnabled_SunPositionCalculator( int ) ) );
}

void MainWindow::SetupViews()
{
    SetupCommandView();
    SetupGraphicView();
   	SetupTreeView();
   	SetupParametersView();
}

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

void MainWindow::SetupTreeView()
{
	NodeNameDelegate* delegate = new NodeNameDelegate( m_sceneModel );
	m_treeView = GetSceneModelViewPointer();
	m_treeView->setItemDelegate( delegate );
	m_treeView->setModel( m_sceneModel );
	m_treeView->setSelectionModel( m_selectionModel );
	m_treeView->setDragEnabled(true);
	m_treeView->setAcceptDrops(true);
	m_treeView->setDropIndicatorShown(true);
	m_treeView->setDragDropMode(QAbstractItemView::DragDrop);
	m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
	m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_treeView->setRootIsDecorated(true);

	connect( m_treeView, SIGNAL( dragAndDrop( const QModelIndex&, const QModelIndex& ) ),
			 this, SLOT ( itemDragAndDrop( const QModelIndex&, const QModelIndex& ) ) );
	connect( m_treeView, SIGNAL( dragAndDropCopy( const QModelIndex&, const QModelIndex& ) ),
			 this, SLOT ( itemDragAndDropCopy( const QModelIndex&, const QModelIndex& ) ) );
	connect( m_treeView, SIGNAL( showMenu( const QModelIndex& ) ),
				 this, SLOT ( showMenu( const QModelIndex& ) ) );
}

QSplitter* MainWindow::GetHorizontalSplitterPointer()
{
    QSplitter* pSplitter = findChild< QSplitter* >( "horizontalSplitter" );
    if( !pSplitter ) tgf::SevereError( "MainWindow::GetSceneModelViewPointer: splitter not found" );
    return pSplitter;
}

SceneModelView* MainWindow::GetSceneModelViewPointer()
{
    QSplitter* pSplitter = GetHorizontalSplitterPointer();
    SceneModelView* pTreeView = pSplitter->findChild< SceneModelView* >( "treeView" );
    if ( !pTreeView ) tgf::SevereError( "MainWindow::GetSceneModelViewPointer: treeView not found" );
    return pTreeView;
}

ParametersView* MainWindow::GetParamtersViewPointer()
{
    QSplitter* pSplitter = GetHorizontalSplitterPointer();
    ParametersView* pParametersView = pSplitter->findChild< ParametersView* >( "parametersView" );
    if ( !pParametersView ) tgf::SevereError( "MainWindow::GetParamtersViewPointer: parametersView not found" );
    return pParametersView;
}

void MainWindow::SetupParametersView()
{
    ParametersView* petParamtersViewPointer();
    connect ( parametersView, SIGNAL(valueModificated( const QStringList&, SoBaseKit*, QString  ) ),
		                this, SLOT( parameterModified( const QStringList&, SoBaseKit*, QString  ) ) );
	connect( m_selectionModel, SIGNAL( currentChanged ( const QModelIndex& , const QModelIndex& ) ),
			             this, SLOT( selectionChanged( const QModelIndex& , const QModelIndex& ) ) );
}

void MainWindow::LoadAvailablePlugins( )
{
	QStringList filesList;
	BuildFileList( PluginDirectory(), filesList );
    foreach( QString fileName, filesList ) LoadTonatiuhPlugin( fileName );
}

void MainWindow::LoadTonatiuhPlugin( const QString& fileName )
{
 	QPluginLoader loader( fileName );
    QObject* plugin = loader.instance();
    if ( plugin != 0)
    {
    	if( plugin->inherits( "RandomDeviateFactory") ) LoadRandomDeviatePlugin( plugin );
    	if( plugin->inherits( "TShapeFactory"    ) ) LoadShapePlugin( plugin );
    	if( plugin->inherits( "TSunShapeFactory" ) ) LoadSunshapePlugin( plugin );
    	if( plugin->inherits( "TMaterialFactory" ) ) LoadMaterialPlugin( plugin );
    	if( plugin->inherits( "TPhotonMapFactory") ) LoadPhotonMapPlugin( plugin );
    	if( plugin->inherits( "TTrackerFactory"  ) ) LoadTrackerPlugin( plugin );
	}
}

void MainWindow::LoadRandomDeviatePlugin( QObject* plugin )
{
	RandomDeviateFactory* pRamdomDeviateFactory = qobject_cast<RandomDeviateFactory* >( plugin );
	if ( !pRamdomDeviateFactory ) tgf::SevereError( "MainWindow::LoadPlugins: Random Deviate plug-in not recognized" );
	m_RandomDeviateFactoryList.push_back( pRamdomDeviateFactory );
}

void MainWindow::LoadShapePlugin( QObject* plugin )
{
	TShapeFactory* pTShapeFactory = qobject_cast<TShapeFactory* >( plugin );
	if ( !pTShapeFactory ) tgf::SevereError( "MainWindow::LoadPlugins: Shape plug-in not recognized" );
	SetupActionInsertShape( pTShapeFactory );
	if( pTShapeFactory->IsFlat() )	m_TFlatShapeFactoryList.push_back( pTShapeFactory );
	pTShapeFactory->CreateTShape();
}

void MainWindow::LoadSunshapePlugin( QObject* plugin )
{
    TSunShapeFactory* pTSunShapeFactory = qobject_cast<TSunShapeFactory* >( plugin );
    if( !pTSunShapeFactory ) tgf::SevereError( "MainWindow::LoadPlugins: SunShape plug-in not recognized" );     	    	;
   	pTSunShapeFactory->CreateTSunShape( );
	m_TSunshapeFactoryList.push_back( pTSunShapeFactory );
}

void MainWindow::LoadMaterialPlugin( QObject* plugin )
{
	TMaterialFactory* pTMaterialFactory = qobject_cast<TMaterialFactory* >( plugin );
	if( !pTMaterialFactory )  tgf::SevereError( "MainWindow::LoadPlugins: Material plug-in not recognized" );
	SetupActionInsertMaterial( pTMaterialFactory );
	pTMaterialFactory->CreateTMaterial();
}

void MainWindow::LoadPhotonMapPlugin( QObject* plugin )
{
	TPhotonMapFactory* pTPhotonMapFactory = qobject_cast<TPhotonMapFactory* >( plugin );
	if( !pTPhotonMapFactory ) tgf::SevereError( "MainWindow::LoadPlugins: PhotonMap plug-in not recognized" );;
	m_TPhotonMapFactoryList.push_back( pTPhotonMapFactory );
}

void MainWindow::LoadTrackerPlugin( QObject* plugin )
{
    TTrackerFactory* pTTrackerFactory = qobject_cast< TTrackerFactory* >( plugin );
    if( !pTTrackerFactory ) tgf::SevereError( "MainWindow::LoadPlugins: Tracker plug-in not recognized" );
    SetupActionInsertTracker( pTTrackerFactory );
   	pTTrackerFactory->CreateTTracker( );
}

QDir MainWindow::PluginDirectory()
{
	// Returns the path to the top level plug-in directory.
	// It is assumed that this is a subdirectory named "plugins" of the directory in
	// which the running version of Tonatiuh is located.

    QDir directory( qApp->applicationDirPath() );
  	directory.cd( "plugins" );
	return directory;
}

void MainWindow::BuildFileList( QDir directory, QStringList& filesList )
{
	AddFilesToList( directory, filesList );

	QString directoryPath( directory.absolutePath().append( "/" ) );
    QStringList subdirectoriesList = directory.entryList( QDir::Dirs, QDir::Unsorted );

   for( int i = 0; i< subdirectoriesList.size(); ++i )
   {
    	QString subdirectoryName = subdirectoriesList[i];
   		if( ValidDirectoryName( subdirectoryName ) )
   			BuildFileList( QDir( directoryPath + subdirectoryName ), filesList );
   	}
}

void MainWindow::AddFilesToList( QDir directory, QStringList& filesList )
{
	QString directoryPath( directory.absolutePath().append( "/" ) );

    QStringList filenamesList = directory.entryList( QDir::Files, QDir::Unsorted );
    for( int i = 0; i < filenamesList.size(); ++i )
    	filesList << ( directoryPath + filenamesList[i] );
}

bool MainWindow::ValidDirectoryName( QString& directoryName  )
{
	return ( directoryName != "." ) && ( directoryName != ".." );
}

void MainWindow::SetupActionInsertMaterial( TMaterialFactory* pTMaterialFactory )
{
	ActionInsertMaterial* actionInsertMaterial = new ActionInsertMaterial( pTMaterialFactory->TMaterialName(), this, pTMaterialFactory );
    actionInsertMaterial->setIcon( pTMaterialFactory->TMaterialIcon() );
    QMenu* pMaterialsMenu = menuInsert->findChild< QMenu* >( "menuMaterial" );
    if( !pMaterialsMenu )
    {
    	pMaterialsMenu = CreateMaterialsMenu();
    	m_materialsToolBar = CreateMaterialsTooBar( pMaterialsMenu );
    }
    pMaterialsMenu->addAction( actionInsertMaterial );
	m_materialsToolBar->addAction( actionInsertMaterial );
	m_materialsToolBar->addSeparator();
    connect( actionInsertMaterial, SIGNAL( triggered() ),
    		 actionInsertMaterial, SLOT( OnActionInsertMaterialTriggered() ) );
	connect( actionInsertMaterial, SIGNAL( CreateMaterial( TMaterialFactory* ) ),
			                 this, SLOT( CreateMaterial( TMaterialFactory* ) ) );
}

QMenu* MainWindow::CreateMaterialsMenu( )
{
	QMenu* pMaterialsMenu = new QMenu( "Materials", menuInsert );
	if ( !pMaterialsMenu ) tgf::SevereError( "MainWindow::CreateMaterialsMenu: NULL pMaterialsMenu" );
	pMaterialsMenu->setObjectName( "menuMaterial" );
	menuInsert->addMenu( pMaterialsMenu );
	return pMaterialsMenu;
}

QToolBar* MainWindow::CreateMaterialsTooBar( QMenu* pMaterialsMenu )
{
	QToolBar* pMaterialsToolBar = new QToolBar( pMaterialsMenu );
	if (! pMaterialsToolBar ) tgf::SevereError( "MainWindow::SetupToolBars: NULL pMaterialsToolBar" );
	pMaterialsToolBar->setObjectName( QString::fromUtf8("materialsToolBar" ) );
	pMaterialsToolBar->setOrientation( Qt::Horizontal );
	addToolBar( pMaterialsToolBar );
	return pMaterialsToolBar;
}


/**
 * Creates an action for the /a pTShapeFactory and adds to shape insert menu and toolbar.
 */
void MainWindow::SetupActionInsertShape( TShapeFactory* pTShapeFactory )
{
    ActionInsertShape* actionInsertShape = new ActionInsertShape( pTShapeFactory->TShapeName(), this, pTShapeFactory );
    actionInsertShape->setIcon( pTShapeFactory->TShapeIcon() );
    QMenu* menuShape = menuInsert->findChild< QMenu* >( "shapeMenu" );
   	if( !menuShape )
    {
    	menuShape = new QMenu( "Shape", menuInsert );
    	menuShape->setObjectName( "shapeMenu" );
    	menuInsert->addMenu( menuShape );

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
	menuShape->addAction( actionInsertShape );
	m_shapeToolBar->addAction( actionInsertShape );
	m_shapeToolBar->addSeparator();
    connect( actionInsertShape, SIGNAL( triggered() ), actionInsertShape, SLOT( OnActionInsertShapeTriggered() ) );
	connect( actionInsertShape, SIGNAL( CreateShape( TShapeFactory* ) ), this, SLOT( CreateShape(TShapeFactory*) ) );
}

void MainWindow::SetupActionInsertTracker( TTrackerFactory* pTTrackerFactory )
{
	ActionInsertTracker* actionInsertTracker = new ActionInsertTracker( pTTrackerFactory->TTrackerName(), this, pTTrackerFactory );
    actionInsertTracker->setIcon( pTTrackerFactory->TTrackerIcon() );
    QMenu* menuTracker = menuInsert->findChild< QMenu* >( "trackerMenu" );
   	if( !menuTracker )
    {
    	menuTracker = new QMenu( "Tracker", menuInsert );
    	menuTracker->setObjectName( "trackerMenu" );
    	menuInsert->addMenu( menuTracker );

    	//Create a new toolbar for trackers
    	m_trackersToolBar = new QToolBar( menuTracker );
		if( m_trackersToolBar )
		{
			m_trackersToolBar->setObjectName( QString::fromUtf8("trackersToolBar" ) );
			m_trackersToolBar->setOrientation( Qt::Horizontal );
	    	addToolBar( m_trackersToolBar );
		}
		else tgf::SevereError( "MainWindow::SetupToolBars: NULL m_trackersToolBar" );

    }
	menuTracker->addAction( actionInsertTracker );
	m_trackersToolBar->addAction( actionInsertTracker );
	m_trackersToolBar->addSeparator();
    connect( actionInsertTracker, SIGNAL( triggered() ), actionInsertTracker, SLOT( OnActionInsertTrackerTriggered() ) );
	connect( actionInsertTracker, SIGNAL( CreateTracker( TTrackerFactory* ) ), this, SLOT( CreateTracker(TTrackerFactory*) ) );
}

void MainWindow::on_actionNew_triggered()
{
    if ( OkToContinue() ) StartOver( "" );
}


void MainWindow::on_actionOpen_triggered()
{
    if ( OkToContinue() )
    {
        QString fileName = QFileDialog::getOpenFileName( this,
                               tr( "Open Tonatiuh document" ), ".",
                               tr( "Tonatiuh files (*.tnh)" ) );
        if ( !fileName.isEmpty() ) StartOver( fileName );
    }
}

void MainWindow::on_actionSave_triggered()
{
    Save();
}

void MainWindow::on_actionSaveAs_triggered()
{
    SaveAs();
}


void MainWindow::on_actionSaveComponent_triggered()
{
    SaveComponent();
}

void MainWindow::on_actionPrint_triggered()
{
	//Action yet to be implemented
}

void MainWindow::on_actionClose_triggered()
{
	close();
}

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

// Edit menu actions
void MainWindow::on_actionUndo_triggered()
{
    m_commandStack->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    m_commandStack->redo();
}

void MainWindow:: on_actionUndoView_triggered()
{
    m_commandView->show();
}

void MainWindow::on_actionCut_triggered()
{
	Cut();
}

void MainWindow::on_actionCopy_triggered()
{
 	Copy();
}

void MainWindow::on_actionPaste_Copy_triggered()
{
	Paste( tgc::Copied );
}

void MainWindow::on_actionPaste_Link_triggered()
{
	Paste( tgc::Shared );
}

void MainWindow::on_actionDelete_triggered()
{
	Delete ();
}

// Insert menu actions
void MainWindow::on_actionNode_triggered()
{
	QModelIndex parentIndex;
    if (( !m_treeView->currentIndex().isValid() ) || ( m_treeView->currentIndex() == m_treeView->rootIndex()))
    	parentIndex = m_sceneModel->index (0,0,m_treeView->rootIndex());
	else
		parentIndex = m_treeView->currentIndex();

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

		m_document->SetDocumentModified( true );

	}

}

void MainWindow::on_actionShapeKit_triggered()
{
	QModelIndex parentIndex;
    if (( ! m_treeView->currentIndex().isValid() ) || (m_treeView->currentIndex() == m_treeView->rootIndex()))
    	parentIndex = m_sceneModel->index (0,0, m_treeView->rootIndex());
	else
		parentIndex = m_treeView->currentIndex();

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
	    m_document->SetDocumentModified( true );
	}
}


void MainWindow::on_actionUserComponent_triggered()
{
	QModelIndex parentIndex;
    if (( !m_treeView->currentIndex().isValid() ) || ( m_treeView->currentIndex() == m_treeView->rootIndex()))
    	parentIndex = m_sceneModel->index (0,0,m_treeView->rootIndex());
	else
		parentIndex = m_treeView->currentIndex();

	SoNode* coinNode = m_sceneModel->NodeFromIndex( parentIndex )->GetNode();

	if ( !coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) ) return;

	QString fileName = QFileDialog::getOpenFileName( this,
	                               tr( "Open Tonatiuh document" ), ".",
	                               tr( "Tonatiuh component (*.tcmp)" ) );

	if ( fileName.isEmpty() ) return;

	SoInput componentInput;
	if ( !componentInput.openFile( fileName.toLatin1().constData() ) )
	{
        QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
                              tr( "Cannot open file %1:\n%2." ).arg( fileName ) );
		return;
	}

	SoSeparator* componentSeparator = SoDB::readAll( &componentInput );
	componentInput.closeFile();

	if ( !componentSeparator )
	{
        QMessageBox::warning( 0, tr( "Scene Graph Structure" ),
                              tr( "Error reading file %1:\n%2." )
                             .arg( fileName ) );
		return;
	}

   TSeparatorKit* componentRoot = static_cast< TSeparatorKit* >( componentSeparator->getChild(0) );

   CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit( componentRoot, QPersistentModelIndex(parentIndex), m_sceneModel );
	cmdInsertSeparatorKit->setText( "Insert SeparatorKit node" );
	m_commandStack->push( cmdInsertSeparatorKit );

	m_document->SetDocumentModified( true );

}

//Sun Light menu actions
void MainWindow::on_actionDefine_SunLight_triggered()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene ) return;

	TLightKit* currentLight = 0;
	if( coinScene->getPart( "lightList[0]", false ) )	currentLight = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );


	LightDialog dialog( currentLight, m_TFlatShapeFactoryList, m_TSunshapeFactoryList );
	if( dialog.exec() )
	{

		TLightKit* lightKit = dialog.GetTLightKit();
		if( !lightKit ) return;

		lightKit->setName( "Light" );

 		CmdLightKitModified* command = new CmdLightKitModified( lightKit, coinScene, *m_sceneModel );
		m_commandStack->push( command );

		//Select lightKit
	    TLightKit* newLightKit = static_cast< TLightKit* >( coinScene->getPart("lightList[0]", false) );

	    SoSearchAction* coinSearch = new SoSearchAction();
		coinSearch->setNode( newLightKit );
		coinSearch->setInterest( SoSearchAction::FIRST);
		coinSearch->apply( m_document->GetRoot() );


		SoPath* coinScenePath = coinSearch->getPath( );
		if( !coinScenePath ) tgf::SevereError( "PathFromIndex Null coinScenePath." );
		SoNodeKitPath* lightPath = static_cast< SoNodeKitPath* > ( coinScenePath );
		if( !lightPath ) tgf::SevereError( "PathFromIndex Null nodePath." );

		QModelIndex lightIndex = m_sceneModel->IndexFromPath( *lightPath );
		m_selectionModel->clear();
		m_selectionModel->setCurrentIndex( lightIndex, QItemSelectionModel::ClearAndSelect );

		delete coinSearch;
		m_document->SetDocumentModified( true );

		actionCalculateSunPosition->setEnabled( true );
	}
}

void MainWindow::on_actionCalculateSunPosition_triggered()
{
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene->getPart("lightList[0]", false) ) return;
	TLightKit* lightKit = static_cast< TLightKit* >( coinScene->getPart( "lightList[0]", false ) );


	SunPositionCalculatorDialog* sunposDialog= new SunPositionCalculatorDialog( );

	QDateTime currentTime;
	double longitude;
	double latitude;

	lightKit->GetPositionData( &currentTime, &longitude, &latitude );
	sunposDialog->ChangePosition( currentTime, longitude, latitude );

	connect( sunposDialog, SIGNAL( changeSunLight( QDateTime*, double, double ) ) , this, SLOT( ChangeSunPosition( QDateTime*, double, double ) ) );
	sunposDialog->exec();

	delete sunposDialog;
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
	InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( m_treeView->rootIndex() );
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


	//Check if there is a random generator selected;
	if( m_selectedRandomDeviate == -1 )
	{
		if( m_RandomDeviateFactoryList.size() > 0 ) m_selectedRandomDeviate = 0;
		else	return false;
	}

	//Create the random generator
	if( !m_rand )	m_rand =  m_RandomDeviateFactoryList[m_selectedRandomDeviate]->CreateRandomDeviate();

	//Check if there is a photon map type selected;
	if( m_selectedPhotonMap == -1 )
	{
		if( m_TPhotonMapFactoryList.size() > 0 ) m_selectedPhotonMap = 0;
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
		m_photonMap = m_TPhotonMapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
		m_tracedRays = 0;
	}

	return true;
}

/*!
 * Shows the rays and photons stored at the photon map in the 3D view.
 */
void MainWindow::ShowRaysIn3DView()
{
	actionDisplay_rays->setEnabled( false );
	actionDisplay_rays->setChecked( false );

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
			m_pRays->addChild(currentRays);

			actionDisplay_rays->setEnabled( true );
			actionDisplay_rays->setChecked( true );
		}
	}
}

//Ray trace menu actions
void MainWindow::on_actionRayTraceRun_triggered()
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
		ComputeSceneTreeMap( rootSeparatorInstance, Transform( new Matrix4x4 ) );

		QVector< double > raysPerThread;
		const int maximumValueProgressScale = 25;
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
		QFuture< TPhotonMap* > photonMap = QtConcurrent::mappedReduced( raysPerThread, RayTracer(  rootSeparatorInstance, raycastingSurface, sunShape, lightToWorld, *m_rand, &mutex, m_photonMap ), createPhotonMap, QtConcurrent::UnorderedReduce );
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

void MainWindow::on_actionDisplay_rays_toggled()
{
	if ( actionDisplay_rays->isChecked() && (m_pRays ) )
	{
	  	m_document->GetRoot()->insertChild( m_pRays, 0 );
	}
	else if( !actionDisplay_rays->isChecked() )
	{
		if ( m_pRays->getRefCount( ) > 0 )	m_document->GetRoot()->removeChild( 0 );
	}
}

/*!
 * Writes the photons stored at the photon map at user defined file.
 */
void MainWindow::on_actionExport_PhotonMap_triggered()
{
	if ( m_photonMap == NULL )
	{
		QMessageBox::information( this, "Tonatiuh Action",
	                          "No Photon Map stored", 1);
	    return;
	}

	ExportDialog exportDialog( *m_sceneModel );
	if( !exportDialog.exec() ) return;

	QString fileName = exportDialog.GetExportFileName();
	if( fileName.isEmpty() )
	{
		QMessageBox::information( this, "Tonatiuh Action",
											  "No file defined to save Photon Map", 1);
		return;
	}

	QFile exportFile( fileName );

	 if(!exportFile.open( QIODevice::WriteOnly ) )
	 {
		 QMessageBox::information( this, "Tonatiuh Error",
								 "Tonatiuh can't open export file\n", 1);
			return;
	 }
	QDataStream out( &exportFile );

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

	out<< wPhoton;

	if( exportDialog.GetSelectedPhotons() == 0 )
	{
		QList< Photon* > photonsList = m_photonMap->GetAllPhotons();
		for (int i = 0; i < photonsList.size(); ++i)
		{
			Photon* node = photonsList[i];
			Point3D photon = node->pos;
			double id = node->id;
			double prev_id = ( node->prev ) ? node->prev->id : 0;
			double next_id = ( node->next ) ? node->next->id : 0;
			out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
		}

	}
	else
	{
		SoNodeKitPath* nodeKitPath = exportDialog.GetSelectedSurface();
		QModelIndex nodeKitIndex = m_sceneModel->IndexFromPath( *nodeKitPath );
		InstanceNode* selectedNode = m_sceneModel->NodeFromIndex( nodeKitIndex );
		if( !selectedNode->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) return;

		QList< Photon* > nodePhotonsList = m_photonMap->GetSurfacePhotons( selectedNode );

		if( nodePhotonsList.size() == 0 )
		{
			QMessageBox::information( this, "Tonatiuh Error",
										"There are not photons to export\n", 1);
			return;
		}

		if( exportDialog.GetCoordinateSystem() == 1 )
		{
			SoBaseKit* nodeKit =static_cast< SoBaseKit* > ( selectedNode->GetNode() );
			SoTransform* nodeTransform = static_cast< SoTransform* > ( nodeKit->getPart( "transform", true ) );

			SbMatrix nodeMatrix;
			nodeMatrix.setTransform( nodeTransform->translation.getValue(),
					nodeTransform->rotation.getValue(),
					nodeTransform->scaleFactor.getValue(),
					nodeTransform->scaleOrientation.getValue(),
					nodeTransform->center.getValue() );

			SbViewportRegion region = m_graphicView[0]->GetViewportRegion();
			SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction( region );
			getmatrixAction->apply( nodeKitPath );

			SbMatrix pathTransformation = getmatrixAction->getMatrix();
			pathTransformation = pathTransformation.multLeft( nodeMatrix );


			Transform objectToWorld( pathTransformation[0][0], pathTransformation[1][0], pathTransformation[2][0], pathTransformation[3][0],
								pathTransformation[0][1], pathTransformation[1][1], pathTransformation[2][1], pathTransformation[3][1],
								pathTransformation[0][2], pathTransformation[1][2], pathTransformation[2][2], pathTransformation[3][2],
								pathTransformation[0][3], pathTransformation[1][3], pathTransformation[2][3], pathTransformation[3][3] );

			Transform worldToObject = objectToWorld.GetInverse();
			for( int i = 0; i< nodePhotonsList.size(); ++i )
			{

				Photon* node = nodePhotonsList[i];
				Point3D photon = worldToObject( node->pos );
				double id = node->id;
				double prev_id = ( node->prev ) ? node->prev->id : 0;
				double next_id = ( node->next ) ? node->next->id : 0;
				out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
			}

		}else
		{
			for( int i = 0; i< nodePhotonsList.size(); ++i )
			{

				Photon* node = nodePhotonsList[i];
				Point3D photon = node->pos;
				double id = node->id;
				double prev_id = ( node->prev ) ? node->prev->id : 0;
				double next_id = ( node->next ) ? node->next->id : 0;
				out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
			}
		}



	}
	exportFile.close();
}

/**
 * Action slot to open a Ray Trace Options dialog.
 */
void MainWindow::on_actionRayTraceOptions_triggered()
{
	double oldSelectedRandomDeviate = m_selectedRandomDeviate;
	RayTraceDialog* options = new RayTraceDialog( m_raysPerIteration, m_RandomDeviateFactoryList, m_fraction, m_drawPhotons, m_TPhotonMapFactoryList, m_selectedRandomDeviate, m_selectedPhotonMap, m_increasePhotonMap, this );
	options->exec();

	m_raysPerIteration = options->GetNumRays();
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
		InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( m_treeView->rootIndex() );
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
			InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( m_treeView->rootIndex() );
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


void MainWindow::on_actionAbout_triggered()
{
	//QMessageBox::aboutQt( this );

	QString aboutMessage("Tonatiuh\n"
			"Version: 0.9.5\n"
			"\nPlease see http://www.gnu.org/licenses/gpl.html for an overview of GPLv3 licensing.\n"
			"\nSee http://code.google.com/p/tonatiuh/ for more information.");
	QMessageBox::about( this, QString( "About Toantiuh" ), aboutMessage );
}

//Create actions
void MainWindow::CreateMaterial( TMaterialFactory* pTMaterialFactory )
{
	QModelIndex parentIndex = ( (! m_treeView->currentIndex().isValid() ) || (m_treeView->currentIndex() == m_treeView->rootIndex() ) ) ?
								m_sceneModel->index( 0, 0, m_treeView->rootIndex( )):
								m_treeView->currentIndex();

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

    m_document->SetDocumentModified( true );
}

void MainWindow::CreateShape( TShapeFactory* pTShapeFactory )
{
    QModelIndex parentIndex = ((! m_treeView->currentIndex().isValid() ) || (m_treeView->currentIndex() == m_treeView->rootIndex())) ?
								m_sceneModel->index (0,0,m_treeView->rootIndex()) : m_treeView->currentIndex();

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

        m_document->SetDocumentModified( true );
    }
}

void MainWindow::CreateTracker( TTrackerFactory* pTTrackerFactory )
{
	QModelIndex parentIndex = ((! m_treeView->currentIndex().isValid() ) || (m_treeView->currentIndex() == m_treeView->rootIndex())) ?
									m_sceneModel->index (0,0,m_treeView->rootIndex()):
									m_treeView->currentIndex();

	InstanceNode* ancestor = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = ancestor->GetNode();

	if( parentNode->getTypeId() != TSeparatorKit::getClassTypeId() ) return;


	while( ancestor->GetParent() )
	{
		ancestor = ancestor->GetParent();
	}

	SoSceneKit* scene = m_document->GetSceneKit();
	/*TLightKit* lightKit = static_cast< TLightKit* > ( scene->getPart("lightList[0]", false ) );
	if( !lightKit )
	{
		QMessageBox::information( this, "Tonatiuh Action",
							  "Define a sun light before insert a tracker", 1);
		return;
	}

	TSeparatorKit* parentNodeKit = dynamic_cast< TSeparatorKit* > ( parentNode );
	SoTransform* parentTransform = static_cast< SoTransform* > ( parentNodeKit->getPart("transform", true ) );
	if( parentTransform->rotation.isConnected() )
	{
		QMessageBox::information( this, "Tonatiuh Action",
										  "Delete previous tracker before define a new one", 1);
				return;
	}*/

	TTracker* tracker = pTTrackerFactory->CreateTTracker( );
	tracker->SetSceneKit( scene );

	CmdInsertTracker* command = new CmdInsertTracker( tracker, parentIndex, scene, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
}

//Manipulators actions
void MainWindow::SoTransform_to_SoCenterballManip()
{
	//Transform to a SoCenterballManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoJackManip()
{
	//Transform to a SoJackManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoHandleBoxManip()
{
	//Transform to a SoHandleBoxManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTabBoxManip()
{
	//Transform to a SoTabBoxManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTrackballManip()
{
	//Transform to a SoTrackballManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTransformBoxManip()
{
	//Transform to a SoTransformBoxManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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

	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );
}

void MainWindow::SoTransform_to_SoTransformerManip()
{
	//Transform to a SoTransformerManip manipulator
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

	SoDragger* dragger = manipulator->getDragger();
	dragger->addStartCallback (startManipulator, static_cast< void*>( this ) );
	dragger->addFinishCallback(finishManipulator, static_cast< void*>( this ) );

	m_document->SetDocumentModified( true );

}

void MainWindow::SoManip_to_SoTransform()
{
	//Transform manipulator to a SoTransform
	QModelIndex currentIndex = m_treeView->currentIndex();

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
	selectionChanged( currentIndex, currentIndex );

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

void MainWindow::selectionChanged( const QModelIndex& current, const QModelIndex& /*previous*/ )
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
		parametersView->ChangeParameters( selectedCoinNodeKit );
	}
}

//for treeview signals
void MainWindow::mousePressEvent( QMouseEvent * e )
{
	QPoint pos = e->pos();
	int x = pos.x();
	int y = pos.y()-64;

	QSplitter *pSplitter = findChild<QSplitter *>( "graphicHorizontalSplitter" );
	QRect mainViewRect = pSplitter->geometry();

	if(mainViewRect.contains(x,y))
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
	if( node == m_treeView->rootIndex() ) return;

	InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex( node );
	SoNode* coinNode = nodeInstnace->GetNode();
	//if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

	QUndoCommand* dragAndDrop = new QUndoCommand();
	dragAndDrop->setText("Drag and Drop node");
	new CmdCut( node, m_coinNode_Buffer, m_sceneModel, dragAndDrop );

	new CmdPaste( tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDrop );
	m_commandStack->push( dragAndDrop );

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

	m_document->SetDocumentModified( true );
}

void MainWindow::showMenu( const QModelIndex& index)
{
	if( !index.isValid() ) return;
	m_selectionModel->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect );

	SoNode* coinNode = m_sceneModel->NodeFromIndex(index)->GetNode();
	SoType type = coinNode->getTypeId();

	QMenu popupmenu(this);

   	popupmenu.addAction(actionCut);
   	popupmenu.addAction(actionCopy);
  	popupmenu.addAction(actionPaste_Copy);
  	popupmenu.addAction(actionPaste_Link);
  	popupmenu.addAction(actionDelete);
  	QMenu transformMenu( "Convert to ", &popupmenu );
	if( type.isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		//QMenu transformMenu( &popupmenu );
		//transformMenu.setTitle( "Convert to ");
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

//for sunposdialog signals
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
	}
	delete time;
}

/*!
 * This property holds whether the SunPositionCalculator action is enabled.
 * If the action is disabled, it does not disappear from menu, but it is displayed
 * in a way which indicates that they are unavailable.
 */
void MainWindow::SetEnabled_SunPositionCalculator( int enabled )
{
	actionCalculateSunPosition->setEnabled( enabled );
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
 * Returns to the start origin state and starts with a new model defined in \a fileName.
 * If the file name is not defined, it starts with an empty scene.
 */
bool MainWindow::StartOver( const QString& fileName )
{

	actionDisplay_rays->setEnabled( false );
	actionDisplay_rays->setChecked( false );

	if( m_document->GetRoot()->getChild( 0 )->getName() == "Rays"  ) m_document->GetRoot()->removeChild( 0 );

	if( m_pRays )
	{
		m_pRays->removeAllChildren();
		if ( m_pRays->getRefCount() > 1 ) tgf::SevereError("ShowRaysIn3DView: m_pRays referenced in excess ");
		m_pRays->unref();
		m_pRays = 0;
	}

	m_commandStack->clear();
	SetEnabled_SunPositionCalculator( 0 );

	QStatusBar* statusbar = new QStatusBar;
	setStatusBar( statusbar );

    if( fileName.isEmpty() )
    {
    	m_document->New();
    	statusbar->showMessage( tr( "New file" ), 2000 );
    	//statusBar()->showMessage( tr( "New file" ), 2000 );
    }
    else
    {
    	if( !m_document->ReadFile( fileName ) )
		{
			statusBar()->showMessage( tr( "Loading canceled" ), 2000 );
			return false;
		}
        //statusBar()->showMessage( tr( "File loaded" ), 2000 );
    	statusbar->showMessage( tr( "File loaded" ), 2000 );
    }

    SetCurrentFile( fileName );
	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );

    return true;
}

bool MainWindow::Save()
{
	if ( m_currentFile.isEmpty() ) return SaveAs();
	else return SaveFile( m_currentFile );
}

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

bool MainWindow::SaveAs()
{
	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save Tonatiuh document" ), ".",
	                       tr( "Tonatiuh files (*.tnh)" ) );
	if( fileName.isEmpty() ) return false;

	return SaveFile( fileName );
}


bool MainWindow::SaveComponent()
{
    if( !m_selectionModel->hasSelection() ) return false;
    if( m_selectionModel->currentIndex() == m_treeView->rootIndex() ) return false;

    QModelIndex componentIndex = m_treeView->currentIndex();

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
                              tr( "Cannot open file %1:\n%2. " )
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

bool MainWindow::Copy( )
{
	if( !m_selectionModel->hasSelection() ) return false;
	if( m_selectionModel->currentIndex() == m_treeView->rootIndex() ) return false;
	if( m_selectionModel->currentIndex().parent() == m_treeView->rootIndex() ) return false;


	CmdCopy* command = new CmdCopy( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
	return true;
}

/*!
 * Creates a new \a type paste command. The clipboard node was inerted as selected node
 * child.
 *
 * Returns \a true if the node was successfully pasted, otherwise returns \a false.
 */
bool MainWindow::Paste( tgc::PasteType type )
{
	if( !m_selectionModel->hasSelection() ) return false;
	if( !m_coinNode_Buffer ) return false;

	InstanceNode* ancestor = m_sceneModel->NodeFromIndex( m_selectionModel->currentIndex() );
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

	/*QString nodeName( m_coinNode_Buffer->getName().getString() );
	int count = 1;
	QString newName = nodeName;
	while ( !m_sceneModel->SetNodeName( *m_coinNode_Buffer, newName ) )
	{
		count++;
		newName = QString( "%1_copy%2").arg( nodeName, QString::number( count) );
	}*/


	m_document->SetDocumentModified( true );
	return true;

}

/*!
 * Creates a new delete command, where the selected node was deleted.
 * child.
 *
 * Returns \a true if the node was successfully deleted, otherwise returns \a false.
 */
bool MainWindow::Delete( )
{
	if( !m_selectionModel->hasSelection() ) return false;
	if( !m_selectionModel->currentIndex().isValid()) return false;
	if( m_selectionModel->currentIndex() == m_treeView->rootIndex() ) return false;
	if( m_selectionModel->currentIndex().parent() == m_treeView->rootIndex() ) return false;

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex( m_selectionModel->currentIndex() );
	SoNode* coinNode = instanceNode->GetNode();

	if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )
	{
		CmdDeleteTracker* commandDelete = new CmdDeleteTracker( m_selectionModel->currentIndex(), m_document->GetSceneKit(), *m_sceneModel );
		m_commandStack->push( commandDelete );
	}
	else
	{
		CmdDelete* commandDelete = new CmdDelete( m_selectionModel->currentIndex(), *m_sceneModel );
		m_commandStack->push( commandDelete );
	}



	if( m_selectionModel->hasSelection() )	m_selectionModel->clearSelection();

	m_document->SetDocumentModified( true );
	return true;
}

bool MainWindow::Cut()
{
	if( !m_selectionModel->hasSelection() ) return false;
	if( m_selectionModel->currentIndex() == m_treeView->rootIndex() ) return false;
	if( m_selectionModel->currentIndex().parent() == m_treeView->rootIndex() ) return false;

	CmdCut* command = new CmdCut( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
	return true;
}

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

QString MainWindow::StrippedName( const QString& fullFileName )
{
	return QFileInfo( fullFileName ).fileName();
}

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

/**
 * Saves application settings.
 **/
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
/**
 * Creates a command for the modification of a node parameter.
 *
 *The command is stored at the stack.
 **/
void MainWindow::parameterModified( const QStringList& oldValueList, SoBaseKit* coinNode, QString coinPart )
{
   	CmdParameterModified* parameterModified = new CmdParameterModified( oldValueList, coinNode, coinPart );
	if ( m_commandStack ) m_commandStack->push( parameterModified );

	m_document->SetDocumentModified( true );
}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 *The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
/*void MainWindow::ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO,
		                              QMap< InstanceNode*, QPair< BBox, Transform > >* sceneMap )*/
void MainWindow::ComputeSceneTreeMap( InstanceNode* instanceNode, Transform parentWTO )
{

	if( !instanceNode ) return;
	SoBaseKit* coinNode = static_cast< SoBaseKit* > ( instanceNode->GetNode() );
	if( !coinNode ) return;

	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart( "transform", true ) );
		Transform objectToWorld = tgf::TransformFromSoTransform( nodeTransform );
		Transform worldToObject = objectToWorld.GetInverse();

		BBox nodeBB;
		Transform nodeWTO(worldToObject * parentWTO );
		instanceNode->SetIntersectionTransform( nodeWTO );

		for( int index = 0; index < instanceNode->children.count() ; ++index )
		{
			InstanceNode* childInstance = instanceNode->children[index];
			ComputeSceneTreeMap(childInstance, nodeWTO );

			nodeBB = Union( nodeBB, childInstance->GetIntersectionBBox() );
		}

		instanceNode->SetIntersectionBBox( nodeBB );

	}
	else
	{
		Transform shapeTransform = parentWTO;
		Transform shapeToWorld = shapeTransform.GetInverse();
		BBox shapeBB;

		if(  instanceNode->children.count() > 0 )
		{
			InstanceNode* shapeInstance = 0;
			if( instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
				shapeInstance =  instanceNode->children[0];
			else if(  instanceNode->children.count() > 1 )	shapeInstance =  instanceNode->children[1];

			SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
			coinNode->getBoundingBox( bbAction );

			SbBox3f box = bbAction->getXfBoundingBox().project();
			delete bbAction;

			SbVec3f pMin = box.getMin();
			SbVec3f pMax = box.getMax();
			shapeBB = shapeToWorld( BBox( Point3D( pMin[0],pMin[1],pMin[2]), Point3D( pMax[0],pMax[1],pMax[2]) ) );
		}
		instanceNode->SetIntersectionTransform( shapeTransform );

		instanceNode->SetIntersectionBBox( shapeBB );
	}

}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a instanceNode.
 *
 *The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
/*void MainWindow::ComputeSceneTreeMap( InstanceNode* instanceNode,
		                              QMap< InstanceNode*,QPair< BBox, Transform* > >* sceneMap )
{

	if( !instanceNode ) return;
	SoBaseKit* coinNode = static_cast< SoBaseKit* > ( instanceNode->GetNode() );
	if( !coinNode ) return;

	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		SoTransform* nodeTransform = static_cast< SoTransform* >(coinNode->getPart( "transform", true ) );
		Transform objectToWorld = tgf::TransformFromSoTransform( nodeTransform );
		Transform worldToObject = objectToWorld.GetInverse();

		Transform parentWTO( new Matrix4x4() );
		if( sceneMap->contains( instanceNode->GetParent() ) )
		{
			QPair< BBox, Transform* > parentData =  sceneMap->value( instanceNode->GetParent() );
			parentWTO = *parentData.second;
		}

		Transform* nodeWTO = new Transform ( worldToObject * parentWTO );
		sceneMap->insert( instanceNode , QPair< BBox, Transform* > ( BBox(), nodeWTO ) );



		BBox nodeBB;

		for( int index = 0; index < instanceNode->children.count() ; ++index )
		{
			InstanceNode* childInstance = instanceNode->children[index];
			ComputeSceneTreeMap(childInstance, sceneMap );

			QPair< BBox, Transform* > childData =  sceneMap->value( childInstance );
			nodeBB = Union( nodeBB, childData.first);

		}
		sceneMap->remove( instanceNode );
		sceneMap->insert( instanceNode , QPair< BBox, Transform* > ( nodeBB, nodeWTO ) );

	}
	else
	{

		QPair< BBox, Transform* > parentData =  sceneMap->value( instanceNode->GetParent() );

		Transform* shapeTransform = parentData.second;
		Transform shapeToWorld = shapeTransform->GetInverse();
		BBox shapeBB;

		if(  instanceNode->children.count() > 0 )
		{
			InstanceNode* shapeInstance = 0;
			if( instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
				shapeInstance =  instanceNode->children[0];
			else if(  instanceNode->children.count() > 1 )	shapeInstance =  instanceNode->children[1];

			SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( SbViewportRegion() ) ;
			coinNode->getBoundingBox( bbAction );

			SbBox3f box = bbAction->getXfBoundingBox().project();
			delete bbAction;

			SbVec3f pMin = box.getMin();
			SbVec3f pMax = box.getMax();
			shapeBB = shapeToWorld( BBox( Point3D( pMin[0],pMin[1],pMin[2]), Point3D( pMax[0],pMax[1],pMax[2]) ) );

			sceneMap->insert( shapeInstance , QPair< BBox, Transform* > ( shapeBB, shapeTransform ) );

		}

		sceneMap->insert( instanceNode , QPair< BBox, Transform* > ( shapeBB, shapeTransform) );

	}

}*/

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

void MainWindow::FinishManipulation( )
{
	QModelIndex currentIndex = m_treeView->currentIndex();
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( m_sceneModel->NodeFromIndex(currentIndex)->GetNode() );

	CmdParameterModified* parameterModified;
	QString type = coinNode->getTypeId().getName().getString() ;
	if ( type == "TLightKit" )	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "tshapekit.transform" );
	else	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "transform" );
	m_commandStack->push( parameterModified );

}

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
