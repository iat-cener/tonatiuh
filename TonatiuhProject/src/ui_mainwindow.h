/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Jan 16 11:21:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <ParametersView.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <SceneModelView.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionPrint;
    QAction *actionClose;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPasteCopy;
    QAction *actionRun;
    QAction *actionAxis;
    QAction *actionUndoView;
    QAction *actionNode;
    QAction *actionDisplayRays;
    QAction *actionPasteLink;
    QAction *actionDelete;
    QAction *actionRayTraceOptions;
    QAction *actionSurfaceNode;
    QAction *action_X_Y_Plane;
    QAction *action_X_Z_Plane;
    QAction *action_Y_Z_Plane;
    QAction *actionDefineSunLight;
    QAction *actionEdit_Mode;
    QAction *actionGrid;
    QAction *actionBackground;
    QAction *actionCalculateSunPosition;
    QAction *actionExportPhotonMap;
    QAction *actionUserComponent;
    QAction *actionSaveComponent;
    QAction *actionSunPlane;
    QAction *actionGridSettings;
    QAction *actionAbout;
    QAction *actionOpenScriptEditor;
    QAction *actionCheckForUpdates;
    QAction *actionAnalyzerNode;
    QAction *actionTrackers_Connected;
    QAction *actionDisconnect_All_Trackers;
    QAction *actionReset_Analyzer_Values;
    QAction *actionSetAimingPointRelative;
    QAction *actionSetAimingPointAbsolute;
    QAction *actionDefineTransmissivity;
    QWidget *centralWidget;
    QVBoxLayout *vboxLayout;
    QSplitter *horizontalSplitter;
    QSplitter *modelSplitter;
    SceneModelView *sceneModelView;
    ParametersView *parametersView;
    QWidget *tab;
    QMenuBar *menubar;
    QMenu *menuEdit;
    QMenu *menuTrackers;
    QMenu *menuFile;
    QMenu *menuRecent;
    QMenu *menuView;
    QMenu *menuInsert;
    QMenu *menuShape;
    QMenu *menuMaterial;
    QMenu *menuTracker;
    QMenu *menuEnvironment;
    QMenu *menuSunLight;
    QMenu *menuTransmissivity;
    QMenu *menu_Raytrace;
    QMenu *menuHelp;
    QMenu *menuAutomation;
    QStatusBar *statusbar;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *raytraceToolBar;
    QToolBar *viewToolBar;
    QToolBar *insertTollBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(857, 364);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/LogoTonatiuh.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon1);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/saveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveAs->setIcon(icon4);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionPrint->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon5);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon6);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon7);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionRedo->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon8);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon9);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon10);
        actionPasteCopy = new QAction(MainWindow);
        actionPasteCopy->setObjectName(QString::fromUtf8("actionPasteCopy"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPasteCopy->setIcon(icon11);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon12);
        actionAxis = new QAction(MainWindow);
        actionAxis->setObjectName(QString::fromUtf8("actionAxis"));
        actionAxis->setCheckable(true);
        actionAxis->setChecked(true);
        actionAxis->setEnabled(true);
        actionUndoView = new QAction(MainWindow);
        actionUndoView->setObjectName(QString::fromUtf8("actionUndoView"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/undoView.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndoView->setIcon(icon13);
        actionNode = new QAction(MainWindow);
        actionNode->setObjectName(QString::fromUtf8("actionNode"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/separatorKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNode->setIcon(icon14);
        actionDisplayRays = new QAction(MainWindow);
        actionDisplayRays->setObjectName(QString::fromUtf8("actionDisplayRays"));
        actionDisplayRays->setCheckable(true);
        actionDisplayRays->setChecked(false);
        actionDisplayRays->setEnabled(false);
        actionPasteLink = new QAction(MainWindow);
        actionPasteLink->setObjectName(QString::fromUtf8("actionPasteLink"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/paste_link.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPasteLink->setIcon(icon15);
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon16);
        actionRayTraceOptions = new QAction(MainWindow);
        actionRayTraceOptions->setObjectName(QString::fromUtf8("actionRayTraceOptions"));
        actionSurfaceNode = new QAction(MainWindow);
        actionSurfaceNode->setObjectName(QString::fromUtf8("actionSurfaceNode"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/shapeKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSurfaceNode->setIcon(icon17);
        action_X_Y_Plane = new QAction(MainWindow);
        action_X_Y_Plane->setObjectName(QString::fromUtf8("action_X_Y_Plane"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/viewer_viewXY.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_X_Y_Plane->setIcon(icon18);
        action_X_Z_Plane = new QAction(MainWindow);
        action_X_Z_Plane->setObjectName(QString::fromUtf8("action_X_Z_Plane"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/viewer_viewXZ.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_X_Z_Plane->setIcon(icon19);
        action_Y_Z_Plane = new QAction(MainWindow);
        action_Y_Z_Plane->setObjectName(QString::fromUtf8("action_Y_Z_Plane"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icons/viewer_viewYZ.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Y_Z_Plane->setIcon(icon20);
        actionDefineSunLight = new QAction(MainWindow);
        actionDefineSunLight->setObjectName(QString::fromUtf8("actionDefineSunLight"));
        actionDefineSunLight->setEnabled(true);
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/icons/lightKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDefineSunLight->setIcon(icon21);
        actionEdit_Mode = new QAction(MainWindow);
        actionEdit_Mode->setObjectName(QString::fromUtf8("actionEdit_Mode"));
        actionEdit_Mode->setCheckable(true);
        actionGrid = new QAction(MainWindow);
        actionGrid->setObjectName(QString::fromUtf8("actionGrid"));
        actionGrid->setCheckable(true);
        actionGrid->setChecked(false);
        actionGrid->setEnabled(true);
        actionBackground = new QAction(MainWindow);
        actionBackground->setObjectName(QString::fromUtf8("actionBackground"));
        actionBackground->setCheckable(true);
        actionBackground->setChecked(true);
        actionCalculateSunPosition = new QAction(MainWindow);
        actionCalculateSunPosition->setObjectName(QString::fromUtf8("actionCalculateSunPosition"));
        actionCalculateSunPosition->setCheckable(false);
        actionCalculateSunPosition->setEnabled(false);
        actionExportPhotonMap = new QAction(MainWindow);
        actionExportPhotonMap->setObjectName(QString::fromUtf8("actionExportPhotonMap"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/icons/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportPhotonMap->setIcon(icon22);
        actionUserComponent = new QAction(MainWindow);
        actionUserComponent->setObjectName(QString::fromUtf8("actionUserComponent"));
        actionSaveComponent = new QAction(MainWindow);
        actionSaveComponent->setObjectName(QString::fromUtf8("actionSaveComponent"));
        actionSunPlane = new QAction(MainWindow);
        actionSunPlane->setObjectName(QString::fromUtf8("actionSunPlane"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/icons/viewer_viewSun.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSunPlane->setIcon(icon23);
        actionGridSettings = new QAction(MainWindow);
        actionGridSettings->setObjectName(QString::fromUtf8("actionGridSettings"));
        actionGridSettings->setEnabled(false);
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/icons/grid.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGridSettings->setIcon(icon24);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOpenScriptEditor = new QAction(MainWindow);
        actionOpenScriptEditor->setObjectName(QString::fromUtf8("actionOpenScriptEditor"));
        actionCheckForUpdates = new QAction(MainWindow);
        actionCheckForUpdates->setObjectName(QString::fromUtf8("actionCheckForUpdates"));
        actionAnalyzerNode = new QAction(MainWindow);
        actionAnalyzerNode->setObjectName(QString::fromUtf8("actionAnalyzerNode"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/icons/AnalyzerKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAnalyzerNode->setIcon(icon25);
        actionTrackers_Connected = new QAction(MainWindow);
        actionTrackers_Connected->setObjectName(QString::fromUtf8("actionTrackers_Connected"));
        actionDisconnect_All_Trackers = new QAction(MainWindow);
        actionDisconnect_All_Trackers->setObjectName(QString::fromUtf8("actionDisconnect_All_Trackers"));
        actionDisconnect_All_Trackers->setCheckable(true);
        actionReset_Analyzer_Values = new QAction(MainWindow);
        actionReset_Analyzer_Values->setObjectName(QString::fromUtf8("actionReset_Analyzer_Values"));
        actionSetAimingPointRelative = new QAction(MainWindow);
        actionSetAimingPointRelative->setObjectName(QString::fromUtf8("actionSetAimingPointRelative"));
        actionSetAimingPointAbsolute = new QAction(MainWindow);
        actionSetAimingPointAbsolute->setObjectName(QString::fromUtf8("actionSetAimingPointAbsolute"));
        actionDefineTransmissivity = new QAction(MainWindow);
        actionDefineTransmissivity->setObjectName(QString::fromUtf8("actionDefineTransmissivity"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(centralWidget);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        horizontalSplitter = new QSplitter(centralWidget);
        horizontalSplitter->setObjectName(QString::fromUtf8("horizontalSplitter"));
        horizontalSplitter->setOrientation(Qt::Horizontal);
        modelSplitter = new QSplitter(horizontalSplitter);
        modelSplitter->setObjectName(QString::fromUtf8("modelSplitter"));
        modelSplitter->setOrientation(Qt::Vertical);
        sceneModelView = new SceneModelView(modelSplitter);
        sceneModelView->setObjectName(QString::fromUtf8("sceneModelView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sceneModelView->sizePolicy().hasHeightForWidth());
        sceneModelView->setSizePolicy(sizePolicy1);
        sceneModelView->setAcceptDrops(true);
        sceneModelView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        sceneModelView->setDragEnabled(true);
        sceneModelView->setDragDropMode(QAbstractItemView::DragDrop);
        sceneModelView->setSelectionBehavior(QAbstractItemView::SelectRows);
        sceneModelView->setAnimated(true);
        sceneModelView->setAllColumnsShowFocus(false);
        modelSplitter->addWidget(sceneModelView);
        parametersView = new ParametersView(modelSplitter);
        parametersView->setObjectName(QString::fromUtf8("parametersView"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        parametersView->addTab(tab, QString());
        modelSplitter->addWidget(parametersView);
        horizontalSplitter->addWidget(modelSplitter);

        vboxLayout->addWidget(horizontalSplitter);

        MainWindow->setCentralWidget(centralWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 857, 20));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuTrackers = new QMenu(menuEdit);
        menuTrackers->setObjectName(QString::fromUtf8("menuTrackers"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuRecent = new QMenu(menuFile);
        menuRecent->setObjectName(QString::fromUtf8("menuRecent"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuInsert = new QMenu(menubar);
        menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
        menuShape = new QMenu(menuInsert);
        menuShape->setObjectName(QString::fromUtf8("menuShape"));
        menuShape->setEnabled(false);
        menuMaterial = new QMenu(menuInsert);
        menuMaterial->setObjectName(QString::fromUtf8("menuMaterial"));
        menuMaterial->setEnabled(false);
        menuTracker = new QMenu(menuInsert);
        menuTracker->setObjectName(QString::fromUtf8("menuTracker"));
        menuTracker->setEnabled(false);
        menuEnvironment = new QMenu(menubar);
        menuEnvironment->setObjectName(QString::fromUtf8("menuEnvironment"));
        menuSunLight = new QMenu(menuEnvironment);
        menuSunLight->setObjectName(QString::fromUtf8("menuSunLight"));
        menuTransmissivity = new QMenu(menuEnvironment);
        menuTransmissivity->setObjectName(QString::fromUtf8("menuTransmissivity"));
        menu_Raytrace = new QMenu(menubar);
        menu_Raytrace->setObjectName(QString::fromUtf8("menu_Raytrace"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuAutomation = new QMenu(menubar);
        menuAutomation->setObjectName(QString::fromUtf8("menuAutomation"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        fileToolBar = new QToolBar(MainWindow);
        fileToolBar->setObjectName(QString::fromUtf8("fileToolBar"));
        fileToolBar->setOrientation(Qt::Horizontal);
        MainWindow->addToolBar(Qt::TopToolBarArea, fileToolBar);
        editToolBar = new QToolBar(MainWindow);
        editToolBar->setObjectName(QString::fromUtf8("editToolBar"));
        editToolBar->setOrientation(Qt::Horizontal);
        MainWindow->addToolBar(Qt::TopToolBarArea, editToolBar);
        raytraceToolBar = new QToolBar(MainWindow);
        raytraceToolBar->setObjectName(QString::fromUtf8("raytraceToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, raytraceToolBar);
        viewToolBar = new QToolBar(MainWindow);
        viewToolBar->setObjectName(QString::fromUtf8("viewToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, viewToolBar);
        insertTollBar = new QToolBar(MainWindow);
        insertTollBar->setObjectName(QString::fromUtf8("insertTollBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, insertTollBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuInsert->menuAction());
        menubar->addAction(menuEnvironment->menuAction());
        menubar->addAction(menu_Raytrace->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuAutomation->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPasteCopy);
        menuEdit->addAction(actionPasteLink);
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(menuTrackers->menuAction());
        menuTrackers->addAction(actionSetAimingPointRelative);
        menuTrackers->addAction(actionSetAimingPointAbsolute);
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(menuRecent->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addAction(actionSaveComponent);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuView->addAction(actionAxis);
        menuView->addAction(actionBackground);
        menuView->addAction(actionEdit_Mode);
        menuView->addSeparator();
        menuView->addAction(action_X_Y_Plane);
        menuView->addAction(action_X_Z_Plane);
        menuView->addAction(action_Y_Z_Plane);
        menuView->addAction(actionSunPlane);
        menuView->addSeparator();
        menuView->addAction(actionGrid);
        menuView->addAction(actionGridSettings);
        menuInsert->addAction(actionNode);
        menuInsert->addAction(actionSurfaceNode);
        menuInsert->addSeparator();
        menuInsert->addAction(menuShape->menuAction());
        menuInsert->addAction(menuMaterial->menuAction());
        menuInsert->addAction(menuTracker->menuAction());
        menuInsert->addSeparator();
        menuInsert->addAction(actionUserComponent);
        menuEnvironment->addAction(menuSunLight->menuAction());
        menuEnvironment->addAction(menuTransmissivity->menuAction());
        menuSunLight->addAction(actionDefineSunLight);
        menuSunLight->addAction(actionCalculateSunPosition);
        menuSunLight->addSeparator();
        menuSunLight->addAction(actionDisconnect_All_Trackers);
        menuTransmissivity->addSeparator();
        menuTransmissivity->addAction(actionDefineTransmissivity);
        menu_Raytrace->addAction(actionRun);
        menu_Raytrace->addAction(actionDisplayRays);
        menu_Raytrace->addAction(actionExportPhotonMap);
        menu_Raytrace->addSeparator();
        menu_Raytrace->addAction(actionRayTraceOptions);
        menu_Raytrace->addAction(actionReset_Analyzer_Values);
        menuHelp->addAction(actionCheckForUpdates);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuAutomation->addAction(actionOpenScriptEditor);
        fileToolBar->addAction(actionNew);
        fileToolBar->addAction(actionOpen);
        fileToolBar->addAction(actionSave);
        fileToolBar->addAction(actionSaveAs);
        fileToolBar->addSeparator();
        fileToolBar->addAction(actionPrint);
        fileToolBar->addSeparator();
        fileToolBar->addAction(actionClose);
        editToolBar->addAction(actionUndo);
        editToolBar->addAction(actionRedo);
        editToolBar->addAction(actionUndoView);
        editToolBar->addSeparator();
        editToolBar->addAction(actionCut);
        editToolBar->addAction(actionCopy);
        editToolBar->addAction(actionPasteCopy);
        editToolBar->addAction(actionPasteLink);
        editToolBar->addAction(actionDelete);
        raytraceToolBar->addAction(actionRun);
        raytraceToolBar->addAction(actionExportPhotonMap);
        viewToolBar->addAction(action_X_Y_Plane);
        viewToolBar->addAction(action_Y_Z_Plane);
        viewToolBar->addAction(action_X_Z_Plane);
        viewToolBar->addAction(actionSunPlane);
        viewToolBar->addSeparator();
        viewToolBar->addAction(actionGridSettings);
        insertTollBar->addAction(actionNode);
        insertTollBar->addAction(actionSurfaceNode);
        insertTollBar->addAction(actionAnalyzerNode);

        retranslateUi(MainWindow);
        QObject::connect(actionGrid, SIGNAL(toggled(bool)), actionGridSettings, SLOT(setEnabled(bool)));

        parametersView->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Tonatiuh", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open ...", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save &as ...", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("MainWindow", "&Print", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "&Close", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MainWindow", "&Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("MainWindow", "&Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "&Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "&Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionPasteCopy->setText(QApplication::translate("MainWindow", "&Paste", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPasteCopy->setToolTip(QApplication::translate("MainWindow", "Paste a clipboard node copy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPasteCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("MainWindow", "&Run", 0, QApplication::UnicodeUTF8));
        actionAxis->setText(QApplication::translate("MainWindow", "Axis", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAxis->setToolTip(QApplication::translate("MainWindow", "Draw Shape Local Axis", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUndoView->setText(QApplication::translate("MainWindow", "Undo View", 0, QApplication::UnicodeUTF8));
        actionNode->setText(QApplication::translate("MainWindow", "Group Node", 0, QApplication::UnicodeUTF8));
        actionDisplayRays->setText(QApplication::translate("MainWindow", "Display rays", 0, QApplication::UnicodeUTF8));
        actionPasteLink->setText(QApplication::translate("MainWindow", "Paste Link", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPasteLink->setToolTip(QApplication::translate("MainWindow", "Paste a link to the clipboard node", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        actionDelete->setShortcut(QApplication::translate("MainWindow", "Backspace", 0, QApplication::UnicodeUTF8));
        actionRayTraceOptions->setText(QApplication::translate("MainWindow", "Ray Trace Options ...", 0, QApplication::UnicodeUTF8));
        actionSurfaceNode->setText(QApplication::translate("MainWindow", "Surface Node", 0, QApplication::UnicodeUTF8));
        action_X_Y_Plane->setText(QApplication::translate("MainWindow", "X-Y PLane", 0, QApplication::UnicodeUTF8));
        action_X_Z_Plane->setText(QApplication::translate("MainWindow", "X-Z Plane", 0, QApplication::UnicodeUTF8));
        action_Y_Z_Plane->setText(QApplication::translate("MainWindow", "Z-YPlane", 0, QApplication::UnicodeUTF8));
        actionDefineSunLight->setText(QApplication::translate("MainWindow", "Define SunLight...", 0, QApplication::UnicodeUTF8));
        actionEdit_Mode->setText(QApplication::translate("MainWindow", "Edit Mode", 0, QApplication::UnicodeUTF8));
        actionGrid->setText(QApplication::translate("MainWindow", "Grid", 0, QApplication::UnicodeUTF8));
        actionBackground->setText(QApplication::translate("MainWindow", "Background", 0, QApplication::UnicodeUTF8));
        actionCalculateSunPosition->setText(QApplication::translate("MainWindow", "Sun Position Calculator ...", 0, QApplication::UnicodeUTF8));
        actionExportPhotonMap->setText(QApplication::translate("MainWindow", "Export PhotonMap", 0, QApplication::UnicodeUTF8));
        actionUserComponent->setText(QApplication::translate("MainWindow", "User Component", 0, QApplication::UnicodeUTF8));
        actionSaveComponent->setText(QApplication::translate("MainWindow", "Save Component", 0, QApplication::UnicodeUTF8));
        actionSunPlane->setText(QApplication::translate("MainWindow", "SunPlane", 0, QApplication::UnicodeUTF8));
        actionGridSettings->setText(QApplication::translate("MainWindow", "Grid Settings ...", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
        actionOpenScriptEditor->setText(QApplication::translate("MainWindow", "Script Editor", 0, QApplication::UnicodeUTF8));
        actionCheckForUpdates->setText(QApplication::translate("MainWindow", "Check for updates", 0, QApplication::UnicodeUTF8));
        actionAnalyzerNode->setText(QApplication::translate("MainWindow", "Analyzer Node", 0, QApplication::UnicodeUTF8));
        actionTrackers_Connected->setText(QApplication::translate("MainWindow", "Trackers Connected", 0, QApplication::UnicodeUTF8));
        actionDisconnect_All_Trackers->setText(QApplication::translate("MainWindow", "Disconnect All Trackers", 0, QApplication::UnicodeUTF8));
        actionReset_Analyzer_Values->setText(QApplication::translate("MainWindow", "Reset Analyzer Values", 0, QApplication::UnicodeUTF8));
        actionSetAimingPointRelative->setText(QApplication::translate("MainWindow", "Set Aiming Point Relative", 0, QApplication::UnicodeUTF8));
        actionSetAimingPointAbsolute->setText(QApplication::translate("MainWindow", "Set Aiming Point Absolute", 0, QApplication::UnicodeUTF8));
        actionDefineTransmissivity->setText(QApplication::translate("MainWindow", "Define Transmissivity...", 0, QApplication::UnicodeUTF8));
        parametersView->setTabText(parametersView->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuTrackers->setTitle(QApplication::translate("MainWindow", "Trackers", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuRecent->setTitle(QApplication::translate("MainWindow", "&Recent", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuInsert->setTitle(QApplication::translate("MainWindow", "&Insert", 0, QApplication::UnicodeUTF8));
        menuShape->setTitle(QApplication::translate("MainWindow", "Shape", 0, QApplication::UnicodeUTF8));
        menuMaterial->setTitle(QApplication::translate("MainWindow", "Material", 0, QApplication::UnicodeUTF8));
        menuTracker->setTitle(QApplication::translate("MainWindow", "Tracker", 0, QApplication::UnicodeUTF8));
        menuEnvironment->setTitle(QApplication::translate("MainWindow", "Environment", 0, QApplication::UnicodeUTF8));
        menuSunLight->setTitle(QApplication::translate("MainWindow", "Sun Light", 0, QApplication::UnicodeUTF8));
        menuTransmissivity->setTitle(QApplication::translate("MainWindow", "Transmissivity", 0, QApplication::UnicodeUTF8));
        menu_Raytrace->setTitle(QApplication::translate("MainWindow", "&Ray Trace", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuAutomation->setTitle(QApplication::translate("MainWindow", "Automation", 0, QApplication::UnicodeUTF8));
        fileToolBar->setWindowTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        editToolBar->setWindowTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        raytraceToolBar->setWindowTitle(QApplication::translate("MainWindow", "Ray Tracer", 0, QApplication::UnicodeUTF8));
        viewToolBar->setWindowTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        insertTollBar->setWindowTitle(QApplication::translate("MainWindow", "Node Kits", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
