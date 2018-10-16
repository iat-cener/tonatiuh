#include <iostream>

#import "InsertPluginToolBar.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>

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


InsertPluginToolBar::InsertPluginToolBar():
	    QObject()
{
	//! Get available plugins
	QString str_pluginsDirectory= QApplication::applicationDirPath() + QDir::separator() + "plugins";
	QDir pluginsDirectory(str_pluginsDirectory);
	std::string error;
	m_pluginManager.LoadAvailablePlugins( pluginsDirectory.absolutePath().toStdString(), &error);
	m_trackerFactoryNameList = m_pluginManager.GetTTrackerFactoryNames();
	m_shapeFactoryNameList = m_pluginManager.GetTShapeFactoryNames();
	m_materialFactoryNameList = m_pluginManager.GetTMaterialFactoryNames();
}

void InsertPluginToolBar::updateTrackerToolBar()
{
	// Load Tracker Plugins Tool Bar
	for( unsigned int i = 0; i < m_trackerFactoryNameList.size(); i++ ) {
		std::shared_ptr< TTracker > pTTrackerFactory = m_pluginManager.CreateTTracker(m_trackerFactoryNameList.at(i));
		std::string icon = pTTrackerFactory->GetIcon();
		std::string icon_source ="qrc" + icon;
		emit addTrackerToolBarItem(QString::fromUtf8((m_trackerFactoryNameList.at(i)).c_str()), i*44, (QString::fromUtf8(icon_source.c_str())));
	}
}

void InsertPluginToolBar::updateMaterialToolBar()
{
	// Load Tracker Plugins Tool Bar
	for( unsigned int i = 0; i < m_materialFactoryNameList.size(); i++ ) {
		std::shared_ptr< TMaterial > pTMaterialFactory = m_pluginManager.CreateTMaterial(m_materialFactoryNameList.at(i));
		std::string icon = pTMaterialFactory->GetIcon();
		std::string icon_source ="qrc" + icon;
		emit addMaterialToolBarItem(QString::fromUtf8((m_materialFactoryNameList.at(i)).c_str()), i*44, (QString::fromUtf8(icon_source.c_str())));
	}
}

void InsertPluginToolBar::updateShapeToolBar()
{
	// Load Tracker Plugins Tool Bar
	for( unsigned int i = 0; i < m_shapeFactoryNameList.size(); i++ ) {
		std::shared_ptr< TShape > pTShapeFactory = m_pluginManager.CreateTShape(m_shapeFactoryNameList.at(i));
		std::string icon = pTShapeFactory->GetIcon();
		std::string icon_source ="qrc" + icon;
		emit addShapeToolBarItem(QString::fromUtf8((m_shapeFactoryNameList.at(i)).c_str()), i*44, (QString::fromUtf8(icon_source.c_str())));
	}
}


