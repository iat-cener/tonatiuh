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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPluginLoader>
#include <QProgressDialog>
#include <QSettings>
#include <QTime>
#include <QUndoStack>
#include <QUndoView>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
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
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>

#include "ActionInsertMaterial.h"
#include "ActionInsertPhotonMap.h"
#include "ActionInsertShape.h"
#include "ActionInsertTracker.h"
#include "AnalyzerWindow.h"
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
#include "GraphicView.h"
#include "InstanceNode.h"
#include "MainWindow.h"
#include "MersenneTwister.h"
#include "NodeNameDelegate.h"
#include "ProgressUpdater.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "RayTraceDialog.h"
#include "SceneModel.h"
#include "LightDialog.h"
#include "SunPositionCalculatorDialog.h"
#include "SunPositionDialog.h"
#include "TGateEngine.h"
#include "tgf.h"
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
#include "Trace.h"
#include "TTracker.h"
#include "TTrackerFactory.h"


void startManipulator(void *data, SoDragger* dragger)
{
	Trace trace( "MainWindow::startManipulator", false );
	MainWindow* mainwindow = static_cast< MainWindow* >( data );
	mainwindow->StartManipulation( dragger );

}

void finishManipulator(void *data, SoDragger* /*dragger*/ )
{
	Trace trace( "MainWindow::manipulatorChanged", false );
	MainWindow* mainwindow = static_cast< MainWindow* >( data );
	mainwindow->FinishManipulation( );

}

MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
:QMainWindow( parent, flags ), m_currentFile( 0 ), m_recentFiles( 0 ),
m_recentFileActions( 0 ), m_document( 0 ), m_commandStack( 0 ),
m_commandView( 0 ), m_materialsToolBar( 0 ), m_shapeToolBar( 0 ),m_trackersToolBar( 0 ),
m_TPhotonMapFactoryList( 0 ), m_TFlatShapeFactoryList( 0 ), m_TSunshapeFactoryList( 0 ),m_sceneModel( 0 ),
m_selectionModel( 0 ), m_photonMap( 0 ), m_selectedPhotonMap( -1 ), m_increasePhotonMap( false ),
m_pRays( 0 ), m_pGrid( 0 ), m_pRand( 0 ), m_coinNode_Buffer( 0 ),m_manipulators_Buffer( 0 ),
m_tracedRays( 0 ), m_raysPerIteration( 1000 ), m_fraction( 10 ),
m_drawPhotons( false ), m_graphicView( 0 ), m_focusView( 0 )
{


	Trace trace( "MainWindow::MainWindow", false );

    unsigned long seed = QTime::currentTime().msec();
    m_pRand = new MersenneTwister( seed );
	setupUi( this );
    SetupActions();
    SetupMenus();
	SetupToolBars();
    SetupDocument();
    SetupModels();
	SetupViews();
    LoadPlugins();
    ReadSettings();

}

MainWindow::~MainWindow()
{
	Trace trace( "MainWindow::~MainWindow", false );
	delete[] m_recentFileActions;
}

void MainWindow::SetupActions()
{
    Trace trace( "MainWindow::SetupActions", false );
    m_recentFileActions = new QAction*[m_maxRecentFiles];
    for ( int i = 0; i < m_maxRecentFiles; i++ )
    {
    	m_recentFileActions[i] = new QAction( this );
    	m_recentFileActions[i]->setVisible( false );
    	connect( m_recentFileActions[i], SIGNAL( triggered() ), this, SLOT( OpenRecentFile() ) );
    }
}

void MainWindow::SetupMenus()
{
    Trace trace( "MainWindow::SetupMenus", false );
    for ( int i = 0; i < m_maxRecentFiles; i++ )
          menuRecent->addAction( m_recentFileActions[i] );
}

void MainWindow::SetupToolBars()
{
    Trace trace( "MainWindow::SetupToolBars", false );

	//Create a new toolbar for plugins
	m_shapeToolBar = new QToolBar( menuInsert );

	if( m_shapeToolBar )
	{
   	    m_shapeToolBar->setObjectName( QString::fromUtf8("pluginToolBar" ) );
   	    m_shapeToolBar->setOrientation( Qt::Horizontal );
	    addToolBar( m_shapeToolBar );

	}
	else tgf::SevereError( "MainWindow::SetupToolBars: NULL m_pluginToolBar" );

	m_trackersToolBar = new QToolBar( menuInsert );
	if( m_trackersToolBar )
	{
  	    m_trackersToolBar->setOrientation( Qt::Horizontal );
	    addToolBar( m_trackersToolBar );
	}
	else tgf::SevereError( "MainWindow::SetupToolBars: NULL m_trackersToolBar" );

}


void MainWindow::SetupDocument()
{
    Trace trace( "MainWindow::SetupDocument", false );
    m_document = new Document();
    if ( m_document )
    {
        connect( m_document, SIGNAL( selectionFinish( SoSelection* ) ), this, SLOT(selectionFinish( SoSelection* ) ) );
    }
    else tgf::SevereError( "MainWindow::SetupDocument: Fail to create new document" );
}

void MainWindow::SetupModels()
{
    Trace trace( "MainWindow::SetupModels", false );
    m_sceneModel = new SceneModel();
    m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
    m_sceneModel->SetCoinRoot( *m_document->GetRoot() );
    m_selectionModel = new QItemSelectionModel( m_sceneModel );
}

void MainWindow::SetupViews()
{
	Trace trace( "MainWindow::SetupViews", false );
    SetupCommandView();
    SetupGraphicView();
   	SetupTreeView();
   	SetupParametersView();
   	SetupSunposView();
}

void MainWindow::SetupCommandView()
{
    Trace trace( "MainWindow::SetupCommandView", false );

    m_commandStack = new QUndoStack(this);
	m_commandView = new QUndoView( m_commandStack );
	m_commandView->setWindowTitle( tr( "Command List" ) );
	m_commandView->setAttribute( Qt::WA_QuitOnClose, false );
    connect( m_commandStack, SIGNAL( canRedoChanged( bool ) ), actionRedo, SLOT( setEnabled( bool ) ) );
    connect( m_commandStack, SIGNAL( canUndoChanged( bool ) ), actionUndo, SLOT( setEnabled( bool ) ) );
}

void MainWindow::SetupGraphicView()
{
    Trace trace( "MainWindow::SetupGraphicView", false );

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

	QSplitter *pSplitter = findChild<QSplitter *>( "horizontalSplitter" );

	QSplitter* graphicHorizontalSplitter = new QSplitter();
	graphicHorizontalSplitter->setObjectName(QString::fromUtf8("graphicHorizontalSplitter"));
    graphicHorizontalSplitter->setOrientation(Qt::Vertical);
    pSplitter->insertWidget( 0, graphicHorizontalSplitter );

	QList<int> sizes;
    sizes<<500<<200;
    pSplitter->setSizes ( sizes );


    QSplitter *graphicVerticalSplitter1 = new QSplitter();
    graphicVerticalSplitter1->setObjectName( QString::fromUtf8( "graphicVerticalSplitter1" ) );
    graphicVerticalSplitter1->setOrientation(Qt::Horizontal);
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

void MainWindow::SetupTreeView()
{
    Trace trace( "MainWindow::SetupTreeView" );
    QSplitter *pSplitter = findChild< QSplitter* >( "horizontalSplitter" );
    if( pSplitter )
    {
        SceneModelView* treeView = pSplitter->findChild< SceneModelView* >( "treeView" );

    	if ( treeView )
    	{

    		NodeNameDelegate* delegate = new NodeNameDelegate( m_sceneModel );
   			treeView->setItemDelegate( delegate );

			treeView->setModel( m_sceneModel );
			treeView->setSelectionModel( m_selectionModel );
			treeView->setDragEnabled(true);
        	treeView->setAcceptDrops(true);
			treeView->setDropIndicatorShown(true);
			treeView->setDragDropMode(QAbstractItemView::DragDrop);
			treeView->setSelectionMode(QAbstractItemView::SingleSelection);
        	treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        	treeView->setRootIsDecorated(true);

			connect( treeView, SIGNAL( dragAndDrop( const QModelIndex&, const QModelIndex& ) ),
			         this, SLOT ( itemDragAndDrop( const QModelIndex&, const QModelIndex& ) ) );
			connect( treeView, SIGNAL( dragAndDropCopy( const QModelIndex&, const QModelIndex& ) ),
			         this, SLOT ( itemDragAndDropCopy( const QModelIndex&, const QModelIndex& ) ) );
			connect( treeView, SIGNAL( showMenu( const QModelIndex& ) ),
			         this, SLOT ( showMenu( const QModelIndex& ) ) );

    	}
    	else tgf::SevereError( "MainWindow::Create3DView: treeView not found" );
    }
    else tgf::SevereError( "MainWindow::Create3DView: horizontalSplitter not found" );
}

void MainWindow::SetupParametersView()
{
    Trace trace( "MainWindow::SetupParametersView" );
    QSplitter *pSplitter = findChild< QSplitter* >( "horizontalSplitter" );
    if( pSplitter )
    {
    	ParametersView* parametersView = pSplitter->findChild< ParametersView* >( "parametersView" );
    	if ( parametersView )
    	{
    		 connect ( parametersView, SIGNAL(valueModificated( const QStringList&, SoBaseKit*, QString  ) ),
			                            this, SLOT( parameterModified( const QStringList&, SoBaseKit*, QString  ) ) );
			connect( m_selectionModel, SIGNAL( currentChanged ( const QModelIndex& , const QModelIndex& ) ), this, SLOT( selectionChanged( const QModelIndex& , const QModelIndex& ) ) );
    	}
    	else tgf::SevereError( "MainWindow::SetupParametersView: parametersView not found" );
    }
    else tgf::SevereError( "MainWindow::SetupParametersView: horizontalSplitter not found" );
}

void MainWindow::SetupSunposView()
{

}

void MainWindow::LoadPlugins( )
{
    Trace trace( "MainWindow::LoadPlugins", false );
	// Build the list of files to process
	QStringList fileList;
	BuildFileList( PluginDirectory(), fileList );

	//Iterate over pluginsList using "foreach" and load the plugin using QPluginLoader
    foreach( QString fileName, fileList )
    {
     	QPluginLoader loader( fileName );
        QObject* plugin = loader.instance();
        if ( plugin != 0)
        {

        	TShapeFactory* pTShapeFactory = qobject_cast<TShapeFactory* >( plugin );
        	if ( pTShapeFactory != 0 )
        	{
        		SetupActionInsertShape( pTShapeFactory );
        		if( pTShapeFactory->IsFlat() )	m_TFlatShapeFactoryList.push_back( pTShapeFactory );


      			pTShapeFactory->CreateTShape();
        	}
        	if( plugin->inherits("TSunShapeFactory") )
        	{
        	    TSunShapeFactory* pTSunShapeFactory = qobject_cast<TSunShapeFactory* >( plugin );
        	    if( !pTSunShapeFactory ) tgf::SevereError( "MainWindow::LoadPlugins: SunShape Plugin not recognized" );     	    	;
        	   	pTSunShapeFactory->CreateTSunShape( );
				m_TSunshapeFactoryList.push_back( pTSunShapeFactory );
        	}
        	if( plugin->inherits("TMaterialFactory") )
        	{
        		TMaterialFactory* pTMaterialFactory = qobject_cast<TMaterialFactory* >( plugin );
        		if( !pTMaterialFactory )  tgf::SevereError( "MainWindow::LoadPlugins: Material Plugin not recognized" );
        		SetupActionInsertMaterial( pTMaterialFactory );
      			pTMaterialFactory->CreateTMaterial();
        	}
        	if( plugin->inherits("TPhotonMapFactory") )
        	{
        		TPhotonMapFactory* pTPhotonMapFactory = qobject_cast<TPhotonMapFactory* >( plugin );
        		if( !pTPhotonMapFactory ) tgf::SevereError( "MainWindow::LoadPlugins: PhotonMap Plugin not recognized" );
        		pTPhotonMapFactory->CreateTPhotonMap();
				m_TPhotonMapFactoryList.push_back( pTPhotonMapFactory );
        	}
        	if( plugin->inherits("TTrackerFactory") )
        	{
        	    TTrackerFactory* pTTrackerFactory = qobject_cast< TTrackerFactory* >( plugin );
        	    if( !pTTrackerFactory ) tgf::SevereError( "MainWindow::LoadPlugins: Tracker Plugin not recognized" );
        	    SetupActionInsertTracker( pTTrackerFactory );
        	   	pTTrackerFactory->CreateTTracker( );
        	}
    	}
    }
}

void MainWindow::BuildFileList( QDir parentDirectory, QStringList& fileList )
{
	Trace trace( "MainWindow::BuildFileList", false );

    QString parentDirectoryPath( parentDirectory.absolutePath().append( "/" ) );

    QStringList fileNameList = parentDirectory.entryList( QDir::Files, QDir::Unsorted );
    for( int file = 0; file < fileNameList.size(); file++ )
    {
    	fileList << (parentDirectoryPath + fileNameList[file]);
    }

    QStringList subDirList = parentDirectory.entryList( QDir::Dirs, QDir::Unsorted );
    for( int dir = 0; dir < subDirList.size(); dir++ )
   {
   		if( ( subDirList[dir] != "." ) && ( subDirList[dir]!= ".." ) )
   		{
   			QDir subdirectory( parentDirectoryPath + subDirList[dir] );
   			BuildFileList( subdirectory, fileList );
   		}
   	}
}

QDir MainWindow::PluginDirectory()
{
	// This function returns the path to the top level (i.e., root) plugin directory.
	// It is assumed that this is a subdirectory named "plugins" of the directory in
	// which the running version of Tonatiuh is located.
	Trace trace( "MainWindow::PluginDirectory" );
    QDir directory( qApp->applicationDirPath() );
  	directory.cd("../plugins");
	return directory;
}

void MainWindow::SetupActionInsertMaterial( TMaterialFactory* pTMaterialFactory )
{
	ActionInsertMaterial* actionInsertMaterial = new ActionInsertMaterial( pTMaterialFactory->TMaterialName(), this, pTMaterialFactory );
    actionInsertMaterial->setIcon( pTMaterialFactory->TMaterialIcon() );
    QMenu* menuMaterial = menuInsert->findChild< QMenu* >( "menuMaterial" );
    if( !menuMaterial )
    {
    	menuMaterial = new QMenu( "Material", menuInsert );
    	menuMaterial->setObjectName( "menuMaterial" );
    	menuInsert->addMenu( menuMaterial );

    	//Create a new toolbar for materials
		m_materialsToolBar = new QToolBar( menuMaterial );

		if( m_materialsToolBar )
		{
   	   		m_materialsToolBar->setObjectName( QString::fromUtf8("materialsToolBar" ) );
   	    	m_materialsToolBar->setOrientation( Qt::Horizontal );
	    	addToolBar( m_materialsToolBar );
		}
		else tgf::SevereError( "MainWindow::SetupToolBars: NULL m_materialsToolBar" );
    }

	menuMaterial->addAction( actionInsertMaterial );
	m_materialsToolBar->addAction( actionInsertMaterial );
	m_materialsToolBar->addSeparator();
    connect( actionInsertMaterial, SIGNAL( triggered() ), actionInsertMaterial, SLOT( OnActionInsertMaterialTriggered() ) );
	connect( actionInsertMaterial, SIGNAL( CreateMaterial( TMaterialFactory* ) ), this, SLOT( CreateMaterial( TMaterialFactory* ) ) );
}

void MainWindow::SetupActionInsertShape( TShapeFactory* pTShapeFactory )
{
    Trace trace( "MainWindow::SetupActionInsertShape", false );
    ActionInsertShape* actionInsertShape = new ActionInsertShape( pTShapeFactory->TShapeName(), this, pTShapeFactory );
    actionInsertShape->setIcon( pTShapeFactory->TShapeIcon() );
    QMenu* menuShape = menuInsert->findChild< QMenu* >( "shapeMenu" );
   	if( !menuShape )
    {
    	menuShape = new QMenu( "Shape", menuInsert );
    	menuShape->setObjectName( "shapeMenu" );
    	menuInsert->addMenu( menuShape );
    }
	menuShape->addAction( actionInsertShape );
	m_shapeToolBar->addAction( actionInsertShape );
	m_shapeToolBar->addSeparator();
    connect( actionInsertShape, SIGNAL( triggered() ), actionInsertShape, SLOT( OnActionInsertShapeTriggered() ) );
	connect( actionInsertShape, SIGNAL( CreateShape( TShapeFactory* ) ), this, SLOT( CreateShape(TShapeFactory*) ) );
}

void MainWindow::SetupActionInsertTracker( TTrackerFactory* pTTrackerFactory )
{
	Trace trace( "MainWindow::SetupActionInsertTracker", false );

	ActionInsertTracker* actionInsertTracker = new ActionInsertTracker( pTTrackerFactory->TTrackerName(), this, pTTrackerFactory );
    actionInsertTracker->setIcon( pTTrackerFactory->TTrackerIcon() );
    QMenu* menuTracker = menuInsert->findChild< QMenu* >( "trackerMenu" );
   	if( !menuTracker )
    {
    	menuTracker = new QMenu( "Tracker", menuInsert );
    	menuTracker->setObjectName( "trackerMenu" );
    	menuInsert->addMenu( menuTracker );
    }
	menuTracker->addAction( actionInsertTracker );
	m_trackersToolBar->addAction( actionInsertTracker );
	m_trackersToolBar->addSeparator();
    connect( actionInsertTracker, SIGNAL( triggered() ), actionInsertTracker, SLOT( OnActionInsertTrackerTriggered() ) );
	connect( actionInsertTracker, SIGNAL( CreateTracker( TTrackerFactory* ) ), this, SLOT( CreateTracker(TTrackerFactory*) ) );
}



void MainWindow::message()
{
    Trace trace( "MainWindow::message" );
	QMessageBox::information( this, "Tonatiuh Action",
	                          "This action is yet to be implemented", 1);
}

void MainWindow::on_actionNew_triggered()
{
    Trace trace( "MainWindow::on_actionNew_triggered" );
    if ( OkToContinue() ) StartOver( "" );
}


void MainWindow::on_actionOpen_triggered()
{
    Trace trace( "MainWindow::on_actionOpen_triggered" );
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
    Trace trace( "MainWindow::on_actionSave_triggered" );
    Save();
}

void MainWindow::on_actionSaveAs_triggered()
{
    Trace trace( "MainWindow::on_actionSaveAs_triggered" );
    SaveAs();
}

void MainWindow::on_actionPrint_triggered()
{
    Trace trace( "MainWindow::on_actionPrint_triggered" );
	message();
}

void MainWindow::on_actionClose_triggered()
{
    Trace trace( "MainWindow::on_actionClose_triggered" );
	close();
}

void MainWindow::OpenRecentFile()
{
    Trace trace( "MainWindow::OpenRecentFile" );
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
    Trace trace( "MainWindow::on_actionUndo_triggered" );
    m_commandStack->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    Trace trace( "MainWindow::on_actionRedo_triggered" );
    m_commandStack->redo();
}

void MainWindow:: on_actionUndoView_triggered()
{
    Trace trace( "MainWindow:: on_actionUndoView_triggered" );
    m_commandView->show();
}

void MainWindow::on_actionCut_triggered()
{
    Trace trace( "MainWindow::on_actionCut_triggered", false );
	Cut();
}

void MainWindow::on_actionCopy_triggered()
{
    Trace trace( "MainWindow::on_actionCopy_triggered", false );
	Copy();
}

void MainWindow::on_actionPaste_Copy_triggered()
{
    Trace trace( "MainWindow::on_actionPasteCopy_triggered", false );

	Paste( tgc::Copied );
}

void MainWindow::on_actionPaste_Link_triggered()
{
	Trace trace( "MainWindow::on_actionPasteLink_triggered", false );

	Paste( tgc::Shared );
}

void MainWindow::on_actionDelete_triggered()
{
	Trace trace( "MainWindow::on_actionDelete_triggered", false );
	Delete ();

}

// Insert menu actions
void MainWindow::on_actionNode_triggered()
{
    Trace trace( "MainWindow::on_actionNode_triggered", false );

	QModelIndex parentIndex;
    if ((! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex()))
    	parentIndex = m_sceneModel->index (0,0,treeView->rootIndex());
	else
		parentIndex = treeView->currentIndex();

	SoNode* coinNode = m_sceneModel->NodeFromIndex( parentIndex )->GetNode();

	if ( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		TSeparatorKit* separatorKit = new TSeparatorKit();

		CmdInsertSeparatorKit* cmdInsertSeparatorKit = new CmdInsertSeparatorKit( separatorKit, QPersistentModelIndex(parentIndex), m_sceneModel );
		cmdInsertSeparatorKit->setText( "Insert SeparatorKit node" );
		m_commandStack->push( cmdInsertSeparatorKit );

		m_document->SetDocumentModified( true );

	}

}

void MainWindow::on_actionShapeKit_triggered()
{
	Trace trace( "MainWindow::on_actionShapeKit_triggered", false );

	QModelIndex parentIndex;
    if ((! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex()))
    	parentIndex = m_sceneModel->index (0,0,treeView->rootIndex());
	else
		parentIndex = treeView->currentIndex();

   	SoNode* selectedCoinNode = m_sceneModel->NodeFromIndex( parentIndex )->GetNode();
	if ( selectedCoinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		TShapeKit* shapeKit = new TShapeKit;

		CmdInsertShapeKit* insertShapeKit = new CmdInsertShapeKit( parentIndex, shapeKit, m_sceneModel );
	    m_commandStack->push( insertShapeKit );

	    m_document->SetDocumentModified( true );
	}
}

//Sun Light menu actions
void MainWindow::on_actionDefine_SunLight_triggered()
{
	Trace trace( "MainWindow::on_actionDefine_SunLight_triggered", false );

	SoSceneKit* coinScene = m_document->GetSceneKit();
	if( !coinScene ) return;

	TLightKit* currentLight = dynamic_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );

	LightDialog dialog( currentLight, m_TFlatShapeFactoryList, m_TSunshapeFactoryList );
	if( dialog.exec() )
	{

		TLightKit* lightKit = dialog.GetTLightKit();
		if( !lightKit ) return;

 		CmdLightKitModified* command = new CmdLightKitModified( lightKit, coinScene, *m_sceneModel );
		m_commandStack->push( command );

		//Select lightKit
	    TLightKit* newLightKit = dynamic_cast< TLightKit* >( coinScene->getPart("lightList[0]", false) );

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
	}
}

void MainWindow::on_actionDefineSunPosition_triggered()
{
	Trace trace( "MainWindow::on_actionDefineSunPosition_triggered", false );
	SoSceneKit* coinScene = m_document->GetSceneKit();
	TLightKit* lightKit = dynamic_cast< TLightKit* >( coinScene->getPart("lightList[0]", false) );
	if( !lightKit ) return;

	SunPositionDialog* dialog = new SunPositionDialog();
	if( dialog->exec() )
	{

		lightKit->SetSunPosition( dialog->GetAzimuth(), ( ( tgc::Pi / 2 ) - dialog->GetElevation() ), dialog->GetDistance()  );
		m_document->SetDocumentModified( true );
	}
}

void MainWindow::on_actionCalculateSunPosition_triggered()
{
	Trace trace( "MainWindow::on_actionCalculateSunPosition_triggered", false );

	SunPositionCalculatorDialog* sunposDialog = new SunPositionCalculatorDialog();

	SoSceneKit* coinScene = m_document->GetSceneKit();
	TLightKit* lightKit = dynamic_cast< TLightKit* >( coinScene->getPart("lightList[0]", false) );
	if( lightKit ) sunposDialog->SetDateTime( lightKit->GetTime() );

	connect( sunposDialog, SIGNAL( changeSunLight( QDateTime*, double, double ) ) , this, SLOT( SunPositionChanged( QDateTime*, double, double ) ) );
	sunposDialog->exec();

	delete sunposDialog;
}

//Ray trace menu actions
void MainWindow::on_actionRayTraceRun_triggered()
{
    Trace trace( "MainWindow::on_actionRayTraceRun_triggered", false );

	ProgressUpdater progress(m_raysPerIteration, QString("Tracing Rays"), 100, this);
	actionDisplay_rays->setEnabled( false );

	//Check if there is a scene
	SoSceneKit* coinScene = m_document->GetSceneKit();
	if ( !coinScene )  return;

    //Check if there is a light and is properly configured
	TLightKit* lightKit = dynamic_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );
	if ( !lightKit )	return;

	TSunShape* sunShape = static_cast< TSunShape * >( lightKit->getPart( "tsunshape", false ) );
	if( !sunShape ) return;

	TShape* shape = static_cast< TShape * >(lightKit->getPart( "icon", false ) );
	if( !shape ) return;

	SoTransform* lightTransform = dynamic_cast< SoTransform * >(lightKit->getPart("transform",true));
	if( !lightTransform ) return;

	SbMatrix coinMatrix;
	coinMatrix.setTransform( lightTransform->translation.getValue(),
								lightTransform->rotation.getValue(),
								lightTransform->scaleFactor.getValue(),
								lightTransform->scaleOrientation.getValue(),
								lightTransform->center.getValue() );

	Transform lightToWorld( coinMatrix[0][0], coinMatrix[1][0], coinMatrix[2][0], coinMatrix[3][0],
			coinMatrix[0][1], coinMatrix[1][1], coinMatrix[2][1], coinMatrix[3][1],
			coinMatrix[0][2], coinMatrix[1][2], coinMatrix[2][2], coinMatrix[3][2],
			coinMatrix[0][3], coinMatrix[1][3], coinMatrix[2][3], coinMatrix[3][3] );

    //Check if there is a rootSeparator InstanceNode
	InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( treeView->rootIndex() );
	if ( !sceneInstance )  return;
	InstanceNode* rootSeparatorInstance = sceneInstance->children[1];
	if( !rootSeparatorInstance ) return;

    //Compute objects bounding boxes iteratively
	SbViewportRegion region = m_graphicView[0]->GetViewportRegion();

	QModelIndex rootIndex = treeView->rootIndex();
	QPersistentModelIndex rootSepartorIndex = m_sceneModel->index ( 1, 0 );

	QMap< InstanceNode*,QPair< SbBox3f, Transform* > >* sceneMap = new QMap< InstanceNode*,QPair< SbBox3f, Transform* > >();
	ComputeSceneTreeMap( &rootSepartorIndex, region, sceneMap );

	//Check if there is a photon Map type selected;
	if( m_selectedPhotonMap == -1 ) return;

	//Create the photon map where photons are going to be stored
	if ( !m_increasePhotonMap )
	{
		delete m_photonMap;
		m_photonMap = m_TPhotonMapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
		m_tracedRays = 0;
	}
	else
	{
		if( !m_photonMap )
		{
			m_photonMap = m_TPhotonMapFactoryList[m_selectedPhotonMap]->CreateTPhotonMap();
			m_tracedRays = 0;
		}
	}
	m_tracedRays += m_raysPerIteration;

	//Ray tracing storing the generated photons in the Photon Map

	//Random Ray generator
	for ( long unsigned i = 0; i < m_raysPerIteration; i++ )
	{
		Ray ray;

		//Generate ray origin and direction in the Light coordinate system
		ray.origin = shape->Sample( m_pRand->RandomDouble( ), m_pRand->RandomDouble( ) );
		sunShape->generateRayDirection( ray.direction, *m_pRand );

		//Transform ray to World coordinate system and trace the scene
		ray = lightToWorld( ray );

		//Perform Ray Trace
		tgf::TraceRay( ray, sceneMap, rootSeparatorInstance, *m_photonMap, *m_pRand );

		//Update progressDiaglog when appropriate
		progress.Update();
 	}

 	if( m_pRays && ( m_document->GetRoot()->findChild( m_pRays )!= -1 ) )
 	{
  		m_document->GetRoot()->removeChild( m_pRays );
  		while ( m_pRays->getRefCount( ) > 1 ) m_pRays->unref();
 	}

 	if( m_fraction > 0.0 || m_drawPhotons )
 	{
	  	m_pRays = new SoSeparator;
	  	m_pRays->ref();
  		m_pRays->setName("Rays");

		if( m_drawPhotons )
		{
			SoSeparator* points = tgf::DrawPhotonMapPoints(*m_photonMap);
			m_pRays->addChild(points);
		}
		if( m_fraction > 0.0 )
		{
			SoSeparator* currentRays = tgf::DrawPhotonMapRays(*m_photonMap, m_tracedRays, m_fraction );
		   	m_pRays->addChild(currentRays);

		   	actionDisplay_rays->setEnabled( true );
		   	actionDisplay_rays->setChecked( true );
		}
  		m_document->GetRoot()->addChild( m_pRays );
	}
 	progress.Done();
}

void MainWindow::on_actionDisplay_rays_toggled()
{
	if ( actionDisplay_rays->isChecked() && (m_pRays ) )
	{
	  	m_document->GetRoot()->addChild(m_pRays);
	}
	else if( (!actionDisplay_rays->isChecked()) && (m_pRays ) )
	{
		if ( m_pRays ) m_document->GetRoot()->removeChild(m_pRays);
	}
}

void MainWindow::on_actionResults_triggered()
{
	Trace trace( "MainWindow::on_actionResults_triggered", false );

	if ( m_photonMap == NULL )
	{
		QMessageBox::information( this, "Tonatiuh Action",
	                          "No Photon Map stored", 1);
	    return;
	}

	QString type = m_sceneModel->NodeFromIndex( m_selectionModel->currentIndex() )->GetNode()->getTypeId().getName().getString();
	if ( type != "TShapeKit" )
	{
		QMessageBox::information( this, "Tonatiuh Action",
	                          "Analysis requested with no surface selected", 1);
	    return;
	}

	long unsigned nearestPhotons = 100;
	if ( nearestPhotons > m_photonMap->StoredPhotons() )
	{
		QMessageBox::information( this, "Tonatiuh Action",
	                          "Flux requested with more points than are stored in photon map", 1);
	    return;
	}

	TShapeKit* shapeKit = dynamic_cast<TShapeKit*>( m_sceneModel->NodeFromIndex( m_selectionModel->currentIndex() )->GetNode() );
	TShape* shape = dynamic_cast< TShape* >( shapeKit->getPart( "shape", false ) );
	if(!shape)
	{
		QMessageBox::information( this, "Tonatiuh Action",
	                          "Invalid Shape", 1);
	    return;
	}

	int resolution = 100;

	AnalyzerWindow* analyzerWindow = new AnalyzerWindow( this );
	analyzerWindow->Plot(m_photonMap, shape, resolution, resolution);
	analyzerWindow->show();
}


void MainWindow::on_actionExport_PhotonMap_triggered()
{
	Trace trace( "MainWindow::on_actionExport_PhotonMap_triggered", false );

	QString fileName = QFileDialog::getSaveFileName( this, tr("Export PhotonMap"),
			tr( "." ),
            tr( "Binary data files (*.dat)" ) );

	 if( !fileName.isEmpty() )
	 {
		 QFile exportFile( fileName );

		 if(!exportFile.open( QIODevice::WriteOnly ) )
		 {
			 QMessageBox::information( this, "Tonatiuh Error",
			                         "Tonatiuh can't open export file\n", 1);
				return;
		 }

		QDataStream out( &exportFile );
		for( long unsigned i=1; i<= m_photonMap->StoredPhotons(); i++ )
		{
			Photon* node = m_photonMap->GetPhoton(i);
			Point3D photon = node->m_pos;
			double id = node->m_id;
			double prev_id = ( node->m_prev )? node->m_prev->m_id : 0;
			double next_id = ( node->m_next )? node->m_next->m_id : -1;
			out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
		}
		exportFile.close();
	 }
}

/**
 * Action slot to open a Ray Trace Options dialog.
 */
void MainWindow::on_actionRayTraceOptions_triggered()
{
	RayTraceDialog* options = new RayTraceDialog( m_raysPerIteration, m_fraction, m_drawPhotons, m_TPhotonMapFactoryList, m_selectedPhotonMap, m_increasePhotonMap, this );
	int dialog_code = options->exec();
	if( !dialog_code ) return;

	m_raysPerIteration = options->GetNumRays();

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

void MainWindow::on_actionGrid_toggled()
{
	Trace trace( "MainWindow::on_actionGrid_toggled", false );
	if( actionGrid->isChecked() )
	{
		InstanceNode* sceneInstance = m_sceneModel->NodeFromIndex( treeView->rootIndex() );
		if ( !sceneInstance )  return;
		SoNode* rootNode = sceneInstance->GetNode();
		SoPath* nodePath = new SoPath( rootNode );
		nodePath->ref();

		SbViewportRegion region = m_graphicView[m_focusView]->GetViewportRegion();
		SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( region ) ;
		if(nodePath)	bbAction->apply(nodePath);

		SbXfBox3f box= bbAction->getXfBoundingBox();

		SbVec3f min, max;
		box.getBounds(min, max);


		int sizex = (int)(max[0]-(int)min[0]+.5);
		int sizez = (int)(max[2]-(int)min[2]+.5);
		int size = std::max(sizex,sizez);

		if(size <= 0)
			m_pGrid = createGrid(10);
		else
			m_pGrid = createGrid(size);

		m_document->GetRoot()->addChild(m_pGrid);
		delete bbAction;
		nodePath->unref();
	}
	else
		m_document->GetRoot()->removeChild(m_pGrid);
}

void MainWindow::on_actionBackground_toggled()
{
	Trace trace( "MainWindow::on_actionBackground_toggled", false );

	SoVRMLBackground* vrmlBackground = dynamic_cast< SoVRMLBackground* > ( m_document->GetRoot()->getChild( 0 ) );

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

void MainWindow::on_action_X_Y_Plane_triggered()
{
	Trace trace( "MainWindow::on_action_X_Y_Plane_triggered", false );
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( 0, 0, 1 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 1, 0 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_action_X_Z_Plane_triggered()
{
	Trace trace( "MainWindow::on_action_X_Z_Plane_triggered", false );
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( 0, 1, 0 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 0, 1 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

void MainWindow::on_action_Y_Z_Plane_triggered()
{
	Trace trace( "MainWindow::on_action_Y_Z_Plane_triggered", false );
	SoCamera* cam = m_graphicView[m_focusView]->GetCamera();
	SbViewportRegion vpr = m_graphicView[m_focusView]->GetViewportRegion();
	cam->position.setValue( SbVec3f( -1, 0, 0 ) );
	cam->pointAt( SbVec3f( 0, 0, 0 ), SbVec3f( 0, 1, 0 )  );
	cam->viewAll( m_document->GetRoot(), vpr );
}

//Create actions
void MainWindow::CreateMaterial( TMaterialFactory* pTMaterialFactory )
{
	Trace trace( "MainWindow::CreateMaterial", false );

	QModelIndex parentIndex = ( (! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex() ) ) ?
								m_sceneModel->index( 0, 0, treeView->rootIndex( )):
								treeView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = parentInstance->GetNode();
	if( !parentNode->getTypeId().isDerivedFrom( SoShapeKit::getClassTypeId() ) ) return;

	TShapeKit* shapeKit = dynamic_cast< TShapeKit* >( parentNode );
	TMaterial* material = dynamic_cast< TMaterial* >( shapeKit->getPart( "material", false ) );

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

void MainWindow::CreatePhotonMap( TPhotonMapFactory* pTPhotonMapFactory )
{
	Trace trace( "MainWindow::CreatePhotonMap", false);
    /*QModelIndex parentIndex = ((! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex())) ?
								m_sceneModel->index (0,0,treeView->rootIndex()) : treeView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = parentInstance->GetNode();
	if( !parentNode->getTypeId().isDerivedFrom( SoShapeKit::getClassTypeId() ) ) return;

	TShapeKit* shapeKit = dynamic_cast< TShapeKit* >( parentNode );
	TPhotonMap* photonMap = dynamic_cast< TPhotonMap* >( shapeKit->getPart( "photonMap", false ) );
    if (photonMap)
    {
    	QMessageBox::information( this, "Tonatiuh Action",
	                          "This TShapeKit already contains a photonMap", 1);
    }
    else
    {
    	photonMap = pTPhotonMapFactory->CreateTPhotonMap();
    	photonMap->setName( pTPhotonMapFactory->TPhotonMapName().toStdString().c_str() );
        CmdInsertPhotonMap* createPhotonMap = new CmdInsertPhotonMap( shapeKit, photonMap, m_sceneModel );
        QString commandText = QString( "Create PhotonMap: %1" ).arg( pTPhotonMapFactory->TPhotonMapName().toLatin1().constData());
        createPhotonMap->setText( commandText );
        m_commandStack->push( createPhotonMap );

        m_document->SetDocumentModified( true );
    }*/
}

void MainWindow::CreateShape( TShapeFactory* pTShapeFactory )
{
    Trace trace( "MainWindow::CreateShape", false );

    QModelIndex parentIndex = ((! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex())) ?
								m_sceneModel->index (0,0,treeView->rootIndex()) : treeView->currentIndex();

	InstanceNode* parentInstance = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = parentInstance->GetNode();
	if( !parentNode->getTypeId().isDerivedFrom( SoShapeKit::getClassTypeId() ) ) return;

	TShapeKit* shapeKit = dynamic_cast< TShapeKit* >( parentNode );
	TShape* shape = dynamic_cast< TShape* >( shapeKit->getPart( "shape", false ) );

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
	Trace trace( "MainWindow::CreateTracker", true );

	QModelIndex parentIndex = ((! treeView->currentIndex().isValid() ) || (treeView->currentIndex() == treeView->rootIndex())) ?
								m_sceneModel->index (0,0,treeView->rootIndex()):
								treeView->currentIndex();

	InstanceNode* ancestor = m_sceneModel->NodeFromIndex( parentIndex );
	SoNode* parentNode = ancestor->GetNode();

	if( parentNode->getTypeId() != TSeparatorKit::getClassTypeId() ) return;

	while( ancestor->GetParent() )
	{
		if(ancestor->GetParent()->GetNode()->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )	return;
		ancestor = ancestor->GetParent();
	}

	SoSceneKit* scene = m_document->GetSceneKit();
	TLightKit* lightKit = dynamic_cast< TLightKit* > ( scene->getPart("lightList[0]", true) );
   	SoTransform* lightTransform = dynamic_cast< SoTransform* > ( lightKit->getPart("transform", true) );

    TTracker* tracker = pTTrackerFactory->CreateTTracker( );
    QString typeName = pTTrackerFactory->TTrackerName();
    tracker->setName( typeName.toStdString().c_str() );

	TGateEngine* gate = new TGateEngine;
	gate->rotation.connectFrom( &lightTransform->rotation );
	tracker->sunRotation.connectFrom( &gate->outputRotation );

	CmdInsertTracker* command = new CmdInsertTracker( tracker, parentIndex, m_sceneModel );
	m_commandStack->push( command );

}

//Manipulators actions
void MainWindow::SoTransform_to_SoCenterballManip()
{
	Trace trace( "MainWindow::SoTransform_to_SoCenterballManip", false );

	//Transform to a SoCenterballManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoJackManip", false );

	//Transform to a SoJackManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoHandleBoxManip", false );

	//Transform to a SoHandleBoxManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoTabBoxManip", false );

	//Transform to a SoTabBoxManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoTrackballManip", false );

	//Transform to a SoTrackballManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoTransformBoxManip", false );

	//Transform to a SoTransformBoxManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoTransform_to_SoTransformerManip", false );

	//Transform to a SoTransformerManip manipulator
	QModelIndex currentIndex = treeView->currentIndex();

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
	Trace trace( "MainWindow::SoManip_to_SoTransform", false );

	//Transform manipulator to a SoTransform
	QModelIndex currentIndex = treeView->currentIndex();

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
    Trace trace( "MainWindow::selectionFinish", false );


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
    Trace trace( "MainWindow::selectionChanged", false );

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
	Trace trace( "MainWindow::mousePressEvent", false );
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
	Trace trace( "MainWindow::itemDragAndDrop", false );

	if( node == treeView->rootIndex() ) return;

	InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex( node );
	SoNode* coinNode = nodeInstnace->GetNode();
	if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

	QUndoCommand* dragAndDrop = new QUndoCommand();
	dragAndDrop->setText("Drag and Drop node");
	new CmdCut( node, m_coinNode_Buffer, m_sceneModel, dragAndDrop );

	new CmdPaste( tgc::Shared, newParent, coinNode, *m_sceneModel, dragAndDrop );
	m_commandStack->push( dragAndDrop );

	m_document->SetDocumentModified( true );

}

void MainWindow::itemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node)
{
	Trace trace( "MainWindow::itemDragAndDropCopy", false );

	InstanceNode* nodeInstnace = m_sceneModel->NodeFromIndex( node );
	SoNode* coinNode = nodeInstnace->GetNode();
	if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) ) return;

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

	if( type.isDerivedFrom( SoBaseKit::getClassTypeId() ) )
	{
		popupmenu.addAction( tr("Convert to SoCenterballManip"),  this, SLOT(SoTransform_to_SoCenterballManip()));
		popupmenu.addAction( tr("Convert to SoHandleBoxManip"), this, SLOT(SoTransform_to_SoHandleBoxManip()));
		popupmenu.addAction( tr("Convert to SoJackManip"), this, SLOT(SoTransform_to_SoJackManip()));
		popupmenu.addAction( tr("Convert to SoTabBoxManip"), this, SLOT(SoTransform_to_SoTabBoxManip()));
		popupmenu.addAction( tr("Convert to SoTrackballManip"),  this, SLOT(SoTransform_to_SoTrackballManip()));
		popupmenu.addAction( tr("Convert to SoTransformBoxManip"), this, SLOT(SoTransform_to_SoTransformBoxManip()));
		popupmenu.addAction( tr("Convert to SoTransformerManip"), this, SLOT(SoTransform_to_SoTransformerManip()));

	    SoBaseKit* coinKit = static_cast< SoBaseKit* > ( coinNode );
   		SoTransform* transform = dynamic_cast< SoTransform* >( coinKit->getPart("transform", true) );
		SoType type = transform->getTypeId();

      	//Manipuladores
		if ( type.isDerivedFrom(SoTransformManip::getClassTypeId()) )	popupmenu.addAction( tr("Convert to SoTransform"), this, SLOT(SoManip_to_SoTransform()) );

	}

	//Mostramos el menu contextual
	popupmenu.exec( QCursor::pos() );

}

//for sunposdialog signals
void MainWindow::SunPositionChanged( QDateTime* time, double longitude, double latitude )
{
	Trace trace( "MainWindow::closeEvent", false);

	SoSceneKit* coinScene = m_document->GetSceneKit();
	TLightKit* lightKit = dynamic_cast< TLightKit* >( coinScene->getPart( "lightList[0]", true ) );
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

void MainWindow::closeEvent( QCloseEvent* event )
{
    Trace trace( "MainWindow::closeEvent", false);
    if ( OkToContinue() )
    {
    	WriteSettings();
    	event->accept();
    }
    else event->ignore();
}

bool MainWindow::OkToContinue()
{
    Trace trace( "MainWindow::OkToContinue", false );
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

bool MainWindow::StartOver( const QString& fileName )
{
    Trace trace( "MainWindow::StartOver" );
    if( fileName.isEmpty() )
    {
    	m_document->New();
        statusBar()->showMessage( tr( "New file" ), 2000 );
    }
    else
    {
    	if( !m_document->ReadFile( fileName ) )
		{
			statusBar()->showMessage( tr( "Loading canceled" ), 2000 );
			return false;
		}
        statusBar()->showMessage( tr( "File loaded" ), 2000 );
    }

    SetCurrentFile( fileName );
	m_sceneModel->SetCoinScene( *m_document->GetSceneKit() );
	if ( m_pRays ) m_document->GetRoot()->removeChild(m_pRays);
	actionDisplay_rays->setEnabled( false );
	m_commandStack->clear();
    return true;
}

bool MainWindow::Save()
{
    Trace trace( "MainWindow::Save", false );
	if ( m_currentFile.isEmpty() ) return SaveAs();
	else return SaveFile( m_currentFile );
}

bool MainWindow::SaveFile( const QString& fileName )
{
    Trace trace( "MainWindow::SaveFile", false );
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
    Trace trace( "MainWindow::SaveAs", false );
	QString fileName = QFileDialog::getSaveFileName( this,
	                       tr( "Save Tonatiuh document" ), ".",
	                       tr( "Tonatiuh files (*.tnh)" ) );
	if( fileName.isEmpty() ) return false;

	return SaveFile( fileName );
}

bool MainWindow::Copy( )
{
	Trace trace( "MainWindow::Copy", false );

	if( !m_selectionModel->hasSelection() ) return false;
	if( m_selectionModel->currentIndex() == treeView->rootIndex() ) return false;
	if( m_selectionModel->currentIndex().parent() == treeView->rootIndex() ) return false;


	CmdCopy* command = new CmdCopy( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
	return true;
}

bool MainWindow::Paste( tgc::PasteType type )
{
	Trace trace( "MainWindow::PasteCopy", true );

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

	m_document->SetDocumentModified( true );
	return true;

}

bool MainWindow::Delete( )
{
	Trace trace( "MainWindow::Delete", false );

	if( !m_selectionModel->hasSelection() ) return false;

	if( !m_selectionModel->currentIndex().isValid()) return false;

	if( m_selectionModel->currentIndex() == treeView->rootIndex() ) return false;

	if( m_selectionModel->currentIndex().parent() == treeView->rootIndex() ) return false;

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex( m_selectionModel->currentIndex() );
	SoNode* coinNode = instanceNode->GetNode();

	if( coinNode->getTypeId().isDerivedFrom( TTracker::getClassTypeId() ) )
	{
		CmdDeleteTracker* commandDelete = new CmdDeleteTracker( m_selectionModel->currentIndex(), *m_sceneModel );
		m_commandStack->push( commandDelete );
	}
	else
	{
		CmdDelete* commandDelete = new CmdDelete( m_selectionModel->currentIndex(), *m_sceneModel );
		m_commandStack->push( commandDelete );
	}

	m_selectionModel->clearSelection ();

	m_document->SetDocumentModified( true );
	return true;
}


bool MainWindow::Cut()
{
	Trace trace( "MainWindow::Cut", false );

	if( !m_selectionModel->hasSelection() ) return false;
	if( m_selectionModel->currentIndex() == treeView->rootIndex() ) return false;
	if( m_selectionModel->currentIndex().parent() == treeView->rootIndex() ) return false;

	CmdCut* command = new CmdCut( m_selectionModel->currentIndex(), m_coinNode_Buffer, m_sceneModel );
	m_commandStack->push( command );

	m_document->SetDocumentModified( true );
	return true;
}

void MainWindow::SetCurrentFile( const QString& fileName )
{
    Trace trace( "MainWindow::SetCurrentFile", false );
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
    Trace trace( "MainWindow::StrippedName" );
	return QFileInfo( fullFileName ).fileName();
}

void MainWindow::UpdateRecentFileActions()
{
    Trace trace( "MainWindow::UpdateRecentFileActions" );
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



void MainWindow::WriteSettings()
{
    Trace trace( "MainWindow::WriteSettings", false );
	QSettings settings( "NREL UTB CENER", "Tonatiuh" );
	settings.setValue( "geometry", geometry() );
    settings.setValue( "recentFiles", m_recentFiles );
}

void MainWindow::ReadSettings()
{
    Trace trace( "MainWindow::ReadSettings", false );

    QSettings settings( "NREL UTB CENER", "Tonatiuh" );
    QRect rect = settings.value( "geometry", QRect(200, 200, 400, 400 ) ).toRect();
    move( rect.topLeft() );
    resize( rect.size() );

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
    Trace trace( "MainWindow::parameterModified", false );
   	CmdParameterModified* parameterModified = new CmdParameterModified( oldValueList, coinNode, coinPart );
	if ( m_commandStack )	m_commandStack->push( parameterModified );

}

/**
 * Compute a map with the InstanceNodes of sub-tree with top node \a nodeIndex.
 *
 *The map stores for each InstanceNode its BBox and its transform in global coordinates.
 **/
void MainWindow::ComputeSceneTreeMap( QPersistentModelIndex* nodeIndex, SbViewportRegion region, QMap< InstanceNode*,QPair< SbBox3f, Transform* > >* sceneMap )
{
	Trace trace( "MainWindow::ComputeSceneTreeMap", false );

	InstanceNode* instanceNode = m_sceneModel->NodeFromIndex( *nodeIndex );
	SoBaseKit* coinNode = dynamic_cast< SoBaseKit* > ( instanceNode->GetNode() );

	SoPath* pathFromRoot = m_sceneModel->PathFromIndex( *nodeIndex );

	SoGetMatrixAction* getmatrixAction = new SoGetMatrixAction( region );
	getmatrixAction->apply(pathFromRoot);

	SbMatrix pathTransformation = getmatrixAction->getMatrix();

	Transform objectToWorld( pathTransformation[0][0], pathTransformation[1][0], pathTransformation[2][0], pathTransformation[3][0],
					pathTransformation[0][1], pathTransformation[1][1], pathTransformation[2][1], pathTransformation[3][1],
					pathTransformation[0][2], pathTransformation[1][2], pathTransformation[2][2], pathTransformation[3][2],
					pathTransformation[0][3], pathTransformation[1][3], pathTransformation[2][3], pathTransformation[3][3] );

	Transform* worldToObject = new Transform( objectToWorld.GetInverse() );

	SoGetBoundingBoxAction* bbAction = new SoGetBoundingBoxAction( region ) ;
	coinNode->getBoundingBox( bbAction );

	sceneMap->insert( instanceNode , QPair< SbBox3f, Transform* > ( bbAction->getXfBoundingBox().project(), worldToObject ) );


	SbXfBox3f box= bbAction->getXfBoundingBox();
	if( coinNode->getTypeId().isDerivedFrom( TSeparatorKit::getClassTypeId() ) )
	{
		for( int index = 0; index < instanceNode->children.count() ; index++ )
		{
			QPersistentModelIndex childIndex = nodeIndex->child( index, nodeIndex->column() );
			ComputeSceneTreeMap( &childIndex, region, sceneMap );
		}
	}
	else
	{
		SoTransform* nodeTransform = static_cast< SoTransform* > ( coinNode->getPart( "transform", true ) );
		if ( nodeTransform )
		{
			SbMatrix nodeMatrix;
			nodeMatrix.setTransform( nodeTransform->translation.getValue(),
					nodeTransform->rotation.getValue(),
					nodeTransform->scaleFactor.getValue(),
					nodeTransform->scaleOrientation.getValue(),
					nodeTransform->center.getValue() );
			pathTransformation = pathTransformation.multLeft( nodeMatrix );

			Transform shapeObjectToWorld( pathTransformation[0][0], pathTransformation[1][0], pathTransformation[2][0], pathTransformation[3][0],
								pathTransformation[0][1], pathTransformation[1][1], pathTransformation[2][1], pathTransformation[3][1],
								pathTransformation[0][2], pathTransformation[1][2], pathTransformation[2][2], pathTransformation[3][2],
								pathTransformation[0][3], pathTransformation[1][3], pathTransformation[2][3], pathTransformation[3][3] );

			Transform* shapeWorldToObject = new Transform( shapeObjectToWorld.GetInverse() );

			if( instanceNode->children[0]->GetNode()->getTypeId().isDerivedFrom( TShape::getClassTypeId() ) )
				sceneMap->insert( instanceNode->children[0] , QPair< SbBox3f, Transform* > ( bbAction->getXfBoundingBox().project(), shapeWorldToObject ) );
			else
				sceneMap->insert( instanceNode->children[1] , QPair< SbBox3f, Transform* > ( bbAction->getXfBoundingBox().project(), shapeWorldToObject ) );
		}
	}

	delete bbAction;
	delete getmatrixAction;

}

void MainWindow::StartManipulation( SoDragger* dragger )
{
	Trace trace( "MainWindow::StartManipulation", false );

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


	for( int index = 0; index < totalFields; index++ )
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
	Trace trace( "MainWindow::FinishManipulation", false );

	QModelIndex currentIndex = treeView->currentIndex();
	SoBaseKit* coinNode = static_cast< SoBaseKit* >( m_sceneModel->NodeFromIndex(currentIndex)->GetNode() );

	CmdParameterModified* parameterModified;
	QString type = coinNode->getTypeId().getName().getString() ;
	if ( type == "TLightKit" )	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "tshapekit.transform" );
	else	parameterModified = new CmdParameterModified( *m_manipulators_Buffer, coinNode, "transform" );
	m_commandStack->push( parameterModified );

}

SoSeparator* MainWindow::createGrid( int tsize )
{
	const int size = tsize;
    const int vsum = (size + size + 1) * 4;
    const int isum = (vsum/2) * 3;
    const int msum = (size + size + 1) * 2;

    static float colors[2][3] = { {0.4f, 0.4f, 0.4f}, {0.6f, 0.6f, 0.6f} };

    float step, i, s;
    int vnum, inum, mnum, m;

    SbVec3f* vertices = new SbVec3f[vsum];
    int* cindex = new int[isum];
    int* mindex = new int[msum];

    vnum = inum = mnum = m = 0;
    step = 1.0;
    s = (float)size;
    for( i = -s; i <= s; i += step)
    {
            if( i == 0.0) m = 1;

            vertices[vnum].setValue( i, 0.0, s );
            cindex[inum++] = vnum++;
            vertices[vnum].setValue( i, 0.0, -s );
            cindex[inum++] = vnum++;
            cindex[inum++] = -1;
            mindex[mnum++] = m;

            vertices[vnum].setValue( s, 0.0, i );
            cindex[inum++] = vnum++;
            vertices[vnum].setValue( -s, 0.0, i );
            cindex[inum++] = vnum++;
            cindex[inum++] = -1;
            mindex[mnum++] = m;

            if( i == 0.0) m = 0;
    }

    SoSeparator * grid = new SoSeparator;
    grid->ref();

    SoMaterial * mat = new SoMaterial;
    mat->diffuseColor.setValues(0, 2, colors);
    grid->addChild(mat);

    SoCoordinate3 * line_coords = new SoCoordinate3;
    line_coords->point.setValues(0, vsum, vertices);
    grid->addChild(line_coords);

    SoIndexedLineSet * lineset = new SoIndexedLineSet;
    lineset->coordIndex.setValues(0, isum, cindex);
    lineset->materialIndex.setValues(0, msum, mindex);
    grid->addChild(lineset);

    grid->unrefNoDelete();

    delete vertices;
    delete[] cindex;
    delete[] mindex;
    return grid;
}
