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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Inventor/SbVec3f.h>

#include "tgc.h"

#include "ui_mainwindow.h"

class Document;
class GraphicRoot;
class GraphicView;
class InstanceNode;
class PluginManager;
class QDir;
class QUndoStack;
class QUndoView;
class RandomDeviate;
class SoDragger;
class SoSelection;
class SoSeparator;
class SoTransform;;
class TLightShape;
class TMaterialFactory;
class TPhotonMap;
class TShapeFactory;
class TSunShape;
class TTrackerFactory;
class TTransmissivity;
class UpdatesManager;
class SoCamera;


//!  Main window class.
/*!
  Tonatiuh's main window class. It includes the menu bars and all the actions, signals
  and slots to provide user interaction with the program.
*/

class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
	MainWindow( QString tonatiuhFile = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
    ~MainWindow();

    void FinishManipulation( );
    void StartManipulation( SoDragger* dragger );

public slots:
	void Copy();
	void Copy( QString nodeURL );
	void CreateGroupNode();
	void CreateAnalyzerNode();
	void CreateMaterial( QString materialType );
    void CreateShape( QString shapeType );
	void CreateSurfaceNode();
	void CreateTracker( QString trackerType );
    void Cut();
	void Cut( QString nodeURL );
	void Delete();
	void Delete( QString nodeURL );
	void SetAimingPointAbsolute();
	void SetAimingPointRelative();
	void InsertFileComponent( QString componentFileName = QString( "" ) );
	void InsertUserDefinedComponent();
	void New();
	void Open( QString fileName );
	void Paste( QString nodeURL, QString pasteType = QString( "Shared" ) );
	void PasteCopy();
	void PasteLink();
	void Run();
	void ResetAnalyzerValues();
	bool Save();
    bool SaveAs();
    bool SaveComponent();
    void SelectNode( QString nodeUrl );
    void SetNodeName( QString nodeName );
    int SetPhotonMapType( QString typeName );
    void SetRaysPerIteration( unsigned long rays );
    void SetValue( QString nodeUrl, QString parameter, QString value );


    //Manipulators actions
    void SoTransform_to_SoCenterballManip();
	void SoTransform_to_SoJackManip();
	void SoTransform_to_SoHandleBoxManip();
	void SoTransform_to_SoTabBoxManip();
	void SoTransform_to_SoTrackballManip();
	void SoTransform_to_SoTransformBoxManip();
	void SoTransform_to_SoTransformerManip();
	void SoManip_to_SoTransform();

    //Slots for graphicview signals
    void selectionFinish( SoSelection* selection );
    void mousePressEvent ( QMouseEvent * e );

    //Slots for treeview signals
	void itemDragAndDrop(const QModelIndex& newParent, const QModelIndex& node);
	void itemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node);

protected:
    void closeEvent( QCloseEvent* event );

protected slots:
	void CalculateSunPosition();
	void CalculateSunPosition(int year, int month, int day, int hours,double latitude, double longitude);
	void ChangeGridSettings();
	void ChangeNodeName( const QModelIndex& index, const QString& newName);
    void ChangeSelection( const QModelIndex & current );
    void ChangeSunPosition( double azimuth, double zenith );
	void CreateMaterial( TMaterialFactory* pTMaterialFactory );
    void CreateShape( TShapeFactory* pTShapeFactory );
	void CreateTracker( TTrackerFactory* pTTrackerFactory );
	void DefineSunLight();
	//void DefineSunLight(QScript typeOfSun, double irradiance, double angle, double azimuth, double zenith);
	void DefineTransmissivity();
	void DisconnectAllTrackers( bool disconnect );
	void DisplayRays( bool display );
	void ExportPhotonMap();
	void ExportPhotonMap(QString ruta, QString file, bool GlobalCoord,bool exportAllMap);
	void Open();
	void OpenRecentFile();
	void Redo();
	void SetParameterValue( SoNode* node, QString paramenterName, QString newValue );
    void SetSunPositionCalculatorEnabled( int enabled );
	void ShowBackground();
	void ShowCommandView();
	void ShowGrid();
    void ShowMenu( const QModelIndex& index );
    void ShowRayTracerOptionsDialog();
    void ShowRayTracerOptionsDialog(int numRays, QString photonMapType, int randomType,int widthDivisions, int heightDivisions, double raysToDraw, bool drawPhotons, bool increasePhotonMap);
    void ShowWarning( QString message );
	void Undo();

	SbVec3f getTargetOfCamera(SoCamera* cam);

	//View menu actions
	void on_actionAxis_toggled();
	void on_actionEdit_Mode_toggled();
	void on_actionSunPlane_triggered();
	void on_action_X_Y_Plane_triggered();
	void on_action_X_Z_Plane_triggered();
	void on_action_Y_Z_Plane_triggered();

	//Automation menu actions
	void on_actionOpenScriptEditor_triggered();

	//Help menu actions
	void on_actionAbout_triggered();
	void on_actionCheckForUpdates_triggered();
	//void on_actionHelp_triggered();


private:
    void ChangeModelScene();
	SoSeparator* CreateGrid( int xDimension, int zDimension, double xSpacing, double zSpacing );
    QToolBar* CreateMaterialsTooBar( QMenu* pMaterialsMenu );
    QToolBar* CreateTrackerTooBar( QMenu* pMaterialsMenu );
    bool Delete( QModelIndex index );
   	QSplitter* GetHorizontalSplitterPointer();
    bool OkToContinue();
    bool Paste( QModelIndex nodeIndex, tgc::PasteType type );
	void SetAimingPointRelativity(bool relative);
	bool SetAimingPointRelativity( QModelIndex nodeIndex, bool relative);
    QDir PluginDirectory();

    void ReadSettings();
	bool ReadyForRaytracing( InstanceNode*& rootSeparatorInstance,
			                 InstanceNode*& lightInstance,
			                 SoTransform*& lightTransform,
			                 TSunShape*& sunShape,
			                 TLightShape*& shape,
			                 TTransmissivity*& transmissivity );
    bool SaveFile( const QString& fileName );
    void SetCurrentFile( const QString& fileName );
    void SetupActions();
   	void SetupActionsInsertMaterial();
    void SetupActionsInsertShape();
    void SetupActionsInsertTracker();
    void SetupCommandView();
    void SetupDocument();
    void SetupGraphcisRoot();
    void SetupGraphicView();
    void SetupMenus();
    void SetupModels();
   	void SetupParametersView();
    void SetupPluginsManager();
   	void SetupTreeView();
   	void SetupTriggers();
    void SetupViews();
   	void SetupUpdateManager();
	void ShowRaysIn3DView();
    bool StartOver( const QString& fileName );
    QString StrippedName( const QString& fullFileName );
    void UpdateRecentFileActions();
    void WriteSettings();


    enum { m_maxRecentFiles = 7 };
    QString m_currentFile;
    QStringList m_recentFiles;
    QAction** m_recentFileActions;//[m_maxRecentFiles];
    Document* m_document;
    QUndoStack* m_commandStack;
    QUndoView* m_commandView;
    QToolBar* m_materialsToolBar;
    QToolBar* m_photonMapToolBar;
    QToolBar* m_shapeToolBar;
    QToolBar* m_trackersToolBar;

    PluginManager* m_pluginManager;
    UpdatesManager* m_updateManager;

    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;

    RandomDeviate* m_rand;
    int m_selectedRandomDeviate;

    TPhotonMap* m_photonMap;
    int m_selectedPhotonMap;
    bool m_increasePhotonMap;

    QString m_lastExportFileName;
    QString m_lastExportSurfaceUrl;
    bool m_lastExportInGlobal;

    GraphicRoot* m_graphicsRoot;
    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;
    unsigned long m_tracedRays;
    unsigned long m_raysPerIteration;
    double m_fraction;
    bool m_drawPhotons;
    int m_gridXElements;
    int m_gridZElements;
    double m_gridXSpacing;
    double m_gridZSpacing;
    std::vector<GraphicView*> m_graphicView;
    int m_focusView;
    int m_heightDivisions;
    int m_widthDivisions;

};

#endif
