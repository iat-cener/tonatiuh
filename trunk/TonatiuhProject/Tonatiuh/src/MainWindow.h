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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "tgc.h"
#include <ui_mainwindow.h>

class Document;
class GraphicView;
class InstanceNode;
class PhotonMap;
class QDir;
class QUndoStack;
class QUndoView;
class RandomDeviate;
class SbViewportRegion;
class SceneModel;
class SoDragger;
class SoSelection;
class SoSeparator;
class TMaterialFactory;
class Transform;
class TShapeFactory;
class TShapeKit;
class TSunShapeFactory;
class TTrackerFactory;
class TTreeSeparatorFactory;

class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
	MainWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
    ~MainWindow();

    QDir PluginDirectory();
    void FinishManipulation( );
    void StartManipulation( SoDragger* dragger );

public slots:
    // File menu actions
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionPrint_triggered();
	void on_actionClose_triggered();
	void OpenRecentFile();

    // Edit menu actions
    void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionUndoView_triggered();
	void on_actionCut_triggered();
	void on_actionCopy_triggered();
	void on_actionPaste_Link_triggered();
	void on_actionPaste_Copy_triggered();
	void on_actionDelete_triggered();

	// Insert menu actions
	void on_actionNode_triggered();
	void on_actionShapeKit_triggered();

	//Sun Light menu actions
	void on_actionDefine_SunLight_triggered();
	void on_actionDefineSunPosition_triggered();
	void on_actionCalculateSunPosition_triggered();

	//Ray trace menu actions
	void on_actionRayTraceRun_triggered();
	void on_actionDisplay_rays_toggled();
	void on_actionResults_triggered();
	void on_actionExport_PhotonMap_triggered();
	void on_actionRayTraceOptions_triggered();
	void on_actionBalance_triggered();

	//View menu actions
	void on_actionAxis_toggled();
	void on_actionGrid_toggled();
	void on_actionBackground_toggled();
	void on_actionEdit_Mode_toggled();
	void on_action_X_Y_Plane_triggered();
	void on_action_X_Z_Plane_triggered();
	void on_action_Y_Z_Plane_triggered();

	//Create actions
	void CreateMaterial( TMaterialFactory* pTMaterialFactory );
    void CreateShape( TShapeFactory* pTShapeFactory );
	void CreateTracker( TTrackerFactory* pTTrackerFactory );

    //Manipulators actions
    void SoTransform_to_SoCenterballManip();
	void SoTransform_to_SoJackManip();
	void SoTransform_to_SoHandleBoxManip();
	void SoTransform_to_SoTabBoxManip();
	void SoTransform_to_SoTrackballManip();
	void SoTransform_to_SoTransformBoxManip();
	void SoTransform_to_SoTransformerManip();
	void SoManip_to_SoTransform();

    //for graphicview signals
    void selectionFinish( SoSelection* selection );
    void selectionChanged( const QModelIndex & current, const QModelIndex & previous );
    void mousePressEvent ( QMouseEvent * e );

    //for treeview signals
	void itemDragAndDrop(const QModelIndex& newParent, const QModelIndex& node);
	void itemDragAndDropCopy(const QModelIndex& newParent, const QModelIndex& node);
    void showMenu( const QModelIndex& index );

    //for parameterview signals
    void parameterModified( const QStringList& oldValueList, SoBaseKit* coinNode, QString coinPart );

    //for sunposdialog signals
    void SunPositionChanged( QDateTime* time, double longitude, double latitude );

protected:
    void closeEvent( QCloseEvent* event );


private:
    void SetupActions();
    void SetupActionInsertMaterial( TMaterialFactory* pTMaterialFactory );
    void SetupActionInsertShape( TShapeFactory* pTShapeFactory );
    void SetupActionInsertTreeSeparator( TTreeSeparatorFactory* pTTreeSeparatorFactory );
    void SetupActionInsertTracker( TTrackerFactory* pTTrackerFactory );
    void SetupMenus();
    void SetupToolBars();
    void SetupDocument();
    void SetupModels();
    void SetupViews();
    void SetupCommandView();
    void SetupGraphicView();
   	void SetupTreeView();
   	void SetupParametersView();
   	void SetupSunposView();
	void LoadPlugins( );
    void ReadSettings();
    void BuildFileList( QDir parentDirectory, QStringList& fileList );

    void NewFile();
    bool OkToContinue();
    bool LoadFile( const QString& fileName );
    bool Save();
    bool SaveFile( const QString& fileName );
    bool SaveAs();
    bool Copy();
    bool Paste( tgc::PasteType type );
    bool Cut();
    bool Delete();
    void SetCurrentFile( const QString& fileName );
    QString StrippedName( const QString& fullFileName );
    void UpdateRecentFileActions();
    void WriteSettings();
    void message();
    void GetShapeTransformations( SoBaseKit* coinNode, SbViewportRegion region, std::map< TShapeKit*, QList< Transform > >& objectToWorld, std::map< TShapeKit*, QList< Transform > >& worldToObject );
	void SetBoundigBoxes( InstanceNode* instanceNode, SbViewportRegion region );
	SoSeparator* createGrid( int size );

    enum { m_maxRecentFiles = 7 };
    QString m_currentFile;
    QStringList m_recentFiles;
    QAction** m_recentFileActions;//[m_maxRecentFiles];
    Document* m_document;
    QUndoStack* m_commandStack;
    QUndoView* m_commandView;
    QToolBar* m_materialsToolBar;
    QToolBar* m_shapeToolBar;
    QToolBar* m_trackersToolBar;
    QVector< TShapeFactory* > m_TFlatShapeFactoryList;
	QVector< TSunShapeFactory* > m_TSunshapeFactoryList;
    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel;
    PhotonMap* m_photonMap;
    SoSeparator* m_pRays;
    SoSeparator* m_pGrid;
    RandomDeviate* m_pRand;
    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;
    unsigned long m_tracedRays;
    unsigned long m_raysPerIteration;
    bool m_increasePhotonMap;
    double m_fraction;
    bool m_drawPhotons;
    std::vector<GraphicView*> m_graphicView;
    int m_focusView;

};

#endif

