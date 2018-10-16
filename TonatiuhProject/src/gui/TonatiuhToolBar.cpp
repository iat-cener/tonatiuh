#include <iostream>

#import "TonatiuhToolBar.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QMetaObject>
#include <QVariant>

#include <QDir>

#include <Qt3DCore\QEntity>

#include "PluginManager.h"
#include "TTrackerFactory.h"
#include "TTracker.h"
#include "TMaterialFactory.h"
#include "TMaterial.h"
#include "TShapeFactory.h"
#include "TShape.h"
#include <TNodesDatabase.h>


TonatiuhToolBar::TonatiuhToolBar():
	    QObject()
{

}

void TonatiuhToolBar::saveToolbarRef(QObject *toolbarObject)
{

	std::cout<<toolbarObject->objectName().toStdString()<<std::endl;

	 if (toolbarObject->objectName().toStdString().compare("fileToolBar")==0) {
		 m_fileToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("doUndoToolBar")==0) {
		 m_doUndoToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("editToolBar")==0) {
		 m_editToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("ViewToolBar")==0) {
		 m_viewToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("RunToolBar")==0) {
		 m_runToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("InsertElementToolbar")==0) {
		 m_insertElementToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("InsertTrackerToolBar")==0) {
		 m_insertTrackerToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("InsertShapeToolBar")==0) {
		 m_insertShapeToolbar = toolbarObject;
	 }
	 else if (toolbarObject->objectName().toStdString().compare("InsertMaterialToolBar")==0) {
		 m_insertMaterialToolbar = toolbarObject;
	 }

}

void TonatiuhToolBar::fileToolBarUpdater(int tonatiuhWindowWidth) {

	//! Insert the arguments: totalSize, currentTotalSize, remainingSize, full
	int currentTotalSize=0;
	int toolIndex = 8;

	QMetaObject::invokeMethod(m_fileToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_fileToolbar->property("width").toInt();
	std::cout<<"m_fileToolbar"<<m_fileToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_doUndoToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_doUndoToolbar->property("width").toInt();
	std::cout<<"m_doUndoToolbar"<<m_doUndoToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_editToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_editToolbar->property("width").toInt();
	std::cout<<" m_editToolbar"<< m_editToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_viewToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_viewToolbar->property("width").toInt();
	std::cout<<"m_viewToolbar->property"<<m_viewToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_runToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_runToolbar->property("width").toInt();
	std::cout<<"m_fileToolbar width"<<m_runToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_insertElementToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_insertElementToolbar->property("width").toInt();
	std::cout<<"m_insertElementToolbar"<<m_insertElementToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_insertTrackerToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_insertTrackerToolbar->property("width").toInt();
	std::cout<<"m_insertTrackerToolbar"<<m_insertTrackerToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_insertMaterialToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_insertMaterialToolbar->property("width").toInt();
	std::cout<<"m_insertMaterialToolbar.toInt()"<<m_insertMaterialToolbar->property("width").toString().toStdString()<<std::endl;
	toolIndex--;
	QMetaObject::invokeMethod(m_insertShapeToolbar, "compressToolBar", Q_ARG(int, tonatiuhWindowWidth),Q_ARG(int, currentTotalSize), Q_ARG(int, 65*toolIndex));
	currentTotalSize = currentTotalSize + m_insertMaterialToolbar->property("width").toInt();
	std::cout<<"m_insertMaterialToolbar.toInt()"<<m_insertMaterialToolbar->property("width").toString().toStdString()<<std::endl;
	std::cout<<"current total size"<<currentTotalSize<<std::endl;
}

