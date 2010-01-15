/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Fri Jan 15 12:17:53 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
    QAction *actionPaste_Copy;
    QAction *actionRayTraceRun;
    QAction *actionAxis;
    QAction *actionUndoView;
    QAction *actionNode;
    QAction *actionDisplay_rays;
    QAction *actionPaste_Link;
    QAction *actionDelete;
    QAction *actionRayTraceOptions;
    QAction *actionShapeKit;
    QAction *action_X_Y_Plane;
    QAction *action_X_Z_Plane;
    QAction *action_Y_Z_Plane;
    QAction *actionDefine_SunLight;
    QAction *actionEdit_Mode;
    QAction *actionGrid;
    QAction *actionBackground;
    QAction *actionCalculateSunPosition;
    QAction *actionExport_PhotonMap;
    QAction *actionUserComponent;
    QAction *actionSaveComponent;
    QAction *actionDeleteTracker;
    QWidget *centralWidget;
    QVBoxLayout *vboxLayout;
    QSplitter *horizontalSplitter;
    QSplitter *modelSplitter;
    SceneModelView *treeView;
    ParametersView *parametersView;
    QMenuBar *menubar;
    QMenu *menuEdit;
    QMenu *menuFile;
    QMenu *menuRecent;
    QMenu *menuView;
    QMenu *menuInsert;
    QMenu *menuSun_Light;
    QMenu *menu_Raytrace;
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
        MainWindow->resize(735, 289);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
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
        actionPaste_Copy = new QAction(MainWindow);
        actionPaste_Copy->setObjectName(QString::fromUtf8("actionPaste_Copy"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste_Copy->setIcon(icon11);
        actionRayTraceRun = new QAction(MainWindow);
        actionRayTraceRun->setObjectName(QString::fromUtf8("actionRayTraceRun"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/raytrace.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRayTraceRun->setIcon(icon12);
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
        actionDisplay_rays = new QAction(MainWindow);
        actionDisplay_rays->setObjectName(QString::fromUtf8("actionDisplay_rays"));
        actionDisplay_rays->setCheckable(true);
        actionDisplay_rays->setChecked(false);
        actionDisplay_rays->setEnabled(false);
        actionPaste_Link = new QAction(MainWindow);
        actionPaste_Link->setObjectName(QString::fromUtf8("actionPaste_Link"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/paste_link.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste_Link->setIcon(icon15);
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon16);
        actionRayTraceOptions = new QAction(MainWindow);
        actionRayTraceOptions->setObjectName(QString::fromUtf8("actionRayTraceOptions"));
        actionShapeKit = new QAction(MainWindow);
        actionShapeKit->setObjectName(QString::fromUtf8("actionShapeKit"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/shapeKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShapeKit->setIcon(icon17);
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
        actionDefine_SunLight = new QAction(MainWindow);
        actionDefine_SunLight->setObjectName(QString::fromUtf8("actionDefine_SunLight"));
        actionDefine_SunLight->setEnabled(true);
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/icons/lightKit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDefine_SunLight->setIcon(icon21);
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
        actionExport_PhotonMap = new QAction(MainWindow);
        actionExport_PhotonMap->setObjectName(QString::fromUtf8("actionExport_PhotonMap"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/icons/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_PhotonMap->setIcon(icon22);
        actionUserComponent = new QAction(MainWindow);
        actionUserComponent->setObjectName(QString::fromUtf8("actionUserComponent"));
        actionSaveComponent = new QAction(MainWindow);
        actionSaveComponent->setObjectName(QString::fromUtf8("actionSaveComponent"));
        actionDeleteTracker = new QAction(MainWindow);
        actionDeleteTracker->setObjectName(QString::fromUtf8("actionDeleteTracker"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/icons/deleteTracker.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDeleteTracker->setIcon(icon23);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        vboxLayout = new QVBoxLayout(centralWidget);
        vboxLayout->setSpacing(6);
        vboxLayout->setMargin(9);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        horizontalSplitter = new QSplitter(centralWidget);
        horizontalSplitter->setObjectName(QString::fromUtf8("horizontalSplitter"));
        horizontalSplitter->setOrientation(Qt::Horizontal);
        modelSplitter = new QSplitter(horizontalSplitter);
        modelSplitter->setObjectName(QString::fromUtf8("modelSplitter"));
        modelSplitter->setOrientation(Qt::Vertical);
        treeView = new SceneModelView(modelSplitter);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy2);
        treeView->setAcceptDrops(true);
        treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeView->setDragEnabled(true);
        treeView->setDragDropMode(QAbstractItemView::InternalMove);
        treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeView->setAllColumnsShowFocus(false);
        modelSplitter->addWidget(treeView);
        parametersView = new ParametersView(modelSplitter);
        parametersView->setObjectName(QString::fromUtf8("parametersView"));
        sizePolicy2.setHeightForWidth(parametersView->sizePolicy().hasHeightForWidth());
        parametersView->setSizePolicy(sizePolicy2);
        modelSplitter->addWidget(parametersView);
        horizontalSplitter->addWidget(modelSplitter);

        vboxLayout->addWidget(horizontalSplitter);

        MainWindow->setCentralWidget(centralWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 735, 21));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuRecent = new QMenu(menuFile);
        menuRecent->setObjectName(QString::fromUtf8("menuRecent"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuInsert = new QMenu(menubar);
        menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
        menuSun_Light = new QMenu(menubar);
        menuSun_Light->setObjectName(QString::fromUtf8("menuSun_Light"));
        menu_Raytrace = new QMenu(menubar);
        menu_Raytrace->setObjectName(QString::fromUtf8("menu_Raytrace"));
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
        menubar->addAction(menuSun_Light->menuAction());
        menubar->addAction(menu_Raytrace->menuAction());
        menubar->addAction(menuView->menuAction());
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste_Copy);
        menuEdit->addAction(actionPaste_Link);
        menuEdit->addAction(actionDelete);
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
        menuView->addAction(actionGrid);
        menuView->addAction(actionBackground);
        menuView->addAction(actionEdit_Mode);
        menuInsert->addAction(actionNode);
        menuInsert->addAction(actionShapeKit);
        menuInsert->addAction(actionUserComponent);
        menuInsert->addSeparator();
        menuSun_Light->addAction(actionDefine_SunLight);
        menuSun_Light->addAction(actionCalculateSunPosition);
        menu_Raytrace->addAction(actionRayTraceRun);
        menu_Raytrace->addAction(actionDisplay_rays);
        menu_Raytrace->addAction(actionExport_PhotonMap);
        menu_Raytrace->addSeparator();
        menu_Raytrace->addAction(actionRayTraceOptions);
        fileToolBar->addAction(actionNew);
        fileToolBar->addAction(actionOpen);
        fileToolBar->addSeparator();
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
        editToolBar->addAction(actionPaste_Copy);
        editToolBar->addAction(actionPaste_Link);
        editToolBar->addAction(actionDelete);
        raytraceToolBar->addAction(actionRayTraceRun);
        raytraceToolBar->addAction(actionExport_PhotonMap);
        viewToolBar->addAction(action_X_Y_Plane);
        viewToolBar->addAction(action_Y_Z_Plane);
        viewToolBar->addAction(action_X_Z_Plane);
        insertTollBar->addAction(actionNode);
        insertTollBar->addAction(actionShapeKit);

        retranslateUi(MainWindow);

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
        actionRedo->setText(QApplication::translate("MainWindow", "&Redo", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "&Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "&Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionPaste_Copy->setText(QApplication::translate("MainWindow", "&Paste", 0, QApplication::UnicodeUTF8));
        actionPaste_Copy->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionRayTraceRun->setText(QApplication::translate("MainWindow", "&Run", 0, QApplication::UnicodeUTF8));
        actionAxis->setText(QApplication::translate("MainWindow", "Axis", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAxis->setToolTip(QApplication::translate("MainWindow", "Draw Shape Local Axis", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUndoView->setText(QApplication::translate("MainWindow", "Undo View", 0, QApplication::UnicodeUTF8));
        actionNode->setText(QApplication::translate("MainWindow", "Node", 0, QApplication::UnicodeUTF8));
        actionDisplay_rays->setText(QApplication::translate("MainWindow", "Display rays", 0, QApplication::UnicodeUTF8));
        actionPaste_Link->setText(QApplication::translate("MainWindow", "Paste Link", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        actionDelete->setShortcut(QApplication::translate("MainWindow", "Backspace", 0, QApplication::UnicodeUTF8));
        actionRayTraceOptions->setText(QApplication::translate("MainWindow", "Ray Trace Options ...", 0, QApplication::UnicodeUTF8));
        actionShapeKit->setText(QApplication::translate("MainWindow", "ShapeKit", 0, QApplication::UnicodeUTF8));
        action_X_Y_Plane->setText(QApplication::translate("MainWindow", "X-Y PLane", 0, QApplication::UnicodeUTF8));
        action_X_Z_Plane->setText(QApplication::translate("MainWindow", "X-Z Plane", 0, QApplication::UnicodeUTF8));
        action_Y_Z_Plane->setText(QApplication::translate("MainWindow", "Z-YPlane", 0, QApplication::UnicodeUTF8));
        actionDefine_SunLight->setText(QApplication::translate("MainWindow", "Define SunLight ...", 0, QApplication::UnicodeUTF8));
        actionEdit_Mode->setText(QApplication::translate("MainWindow", "Edit Mode", 0, QApplication::UnicodeUTF8));
        actionGrid->setText(QApplication::translate("MainWindow", "Grid", 0, QApplication::UnicodeUTF8));
        actionBackground->setText(QApplication::translate("MainWindow", "Background", 0, QApplication::UnicodeUTF8));
        actionCalculateSunPosition->setText(QApplication::translate("MainWindow", "Sun Position Calculator ...", 0, QApplication::UnicodeUTF8));
        actionExport_PhotonMap->setText(QApplication::translate("MainWindow", "Export PhotonMap", 0, QApplication::UnicodeUTF8));
        actionUserComponent->setText(QApplication::translate("MainWindow", "User Component", 0, QApplication::UnicodeUTF8));
        actionSaveComponent->setText(QApplication::translate("MainWindow", "Save Component", 0, QApplication::UnicodeUTF8));
        actionDeleteTracker->setText(QApplication::translate("MainWindow", "Delete Tracker", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuRecent->setTitle(QApplication::translate("MainWindow", "&Recent", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuInsert->setTitle(QApplication::translate("MainWindow", "&Insert", 0, QApplication::UnicodeUTF8));
        menuSun_Light->setTitle(QApplication::translate("MainWindow", "Sun Light", 0, QApplication::UnicodeUTF8));
        menu_Raytrace->setTitle(QApplication::translate("MainWindow", "&Ray Trace", 0, QApplication::UnicodeUTF8));
        raytraceToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        viewToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        insertTollBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
