/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>
#include <QDir>
#include <QStringList>
#include <vector>
#include "tgc.h"
#include "GraphicView.h"
#include <Inventor/fields/SoFieldContainer.h>

class AnalyzerWindow;
class Document;
class InstanceNode;
class ParametersViewItem;
class PhotonMap;
class QAbstractItemModel;
class QItemSelection;
class QItemSelectionModel;
class QToolBar;
class QUndoStack;
class QUndoView;
class RandomDeviate;
class SbStringList;
class SbXfBox3f;
class SceneModel;
class SoBaseKit;
class SoDragger;
class SoNode;
class SoPath;
class SoPathList;
class SoSelection;
class SoSeparator;
class SoTransformManip;
class TGateEngine;
class TMaterialFactory;
class TLightKit;
class Transform;
class TShape;
class TShapeKit;
class TShapeFactory;
class TSunShapeFactory;
class TTrackerFactory;
class SoVRMLBackground;

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
	void on_actionSunPosition_triggered();
	
	//Ray trace menu actions
	void on_actionRayTraceRun_triggered();
	void on_actionDisplay_rays_toggled();
	void on_actionResults_triggered();
	void on_actionRayTraceOptions_triggered();
	
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
    void SunLightChanged( QDateTime* time, double longitude, double latitude );
        
protected:
    void closeEvent( QCloseEvent* event );
    //static void manipulatorChanged(void *data, SoDragger *);


private:
    void SetupActions();
    void SetupActionInsertMaterial( TMaterialFactory* pTMaterialFactory );
    void SetupActionInsertShape( TShapeFactory* pTShapeFactory );
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
private:    
    enum { m_maxRecentFiles = 7 };
    QString m_currentFile;
    QStringList m_recentFiles;
    QAction* m_recentFileActions[m_maxRecentFiles];
    Document* m_document;
    QUndoStack* m_commandStack;
    QUndoView* m_commandView;
    QToolBar* m_materialsToolBar;
    QToolBar* m_shapeToolBar;
    QToolBar* m_trackersToolBar;
    std::vector< TShapeFactory* > m_TFlatShapeFactoryList;
	std::vector< TSunShapeFactory* > m_TSunshapeFactoryList;
    SceneModel* m_sceneModel;
    QItemSelectionModel* m_selectionModel; 
    PhotonMap* m_photonMap;
    SoSeparator* m_pRays;
    SoSeparator* m_pGrid;
    RandomDeviate* m_pRand;
    SoNode* m_coinNode_Buffer;
    QStringList* m_manipulators_Buffer;
    AnalyzerWindow* m_pAnalyzerWindow;
    unsigned long m_tracedRays;
    unsigned long m_raysPerIteration;
    bool m_increasePhotonMap;
    double m_fraction;
    bool m_drawPhotons;
    QString m_resultsFile;
    SoVRMLBackground* m_vrmlBackground;
    std::vector<GraphicView*> m_graphicView;
    int m_focusView;
    
};

#endif

