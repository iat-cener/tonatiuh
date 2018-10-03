#include <iostream>

#import "WindowMain.h"
#import "SceneView.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDir>

#include <Qt3DCore\QEntity>

#include "PluginManager.h"

/*WindowMain::WindowMain(QObject *root) :
    QObject(root)
{*/
WindowMain::WindowMain(QQmlApplicationEngine * engine) :
	    QObject()
{
	m_engine = engine;
	/*QDir pluginsDirectory("C:\\sde64\\workspace\\TonatiuhProject3.0QML\\TonatiuhProject\\bin\\debug\\plugins");
	//PluginManager pluginManager;
	std::string error;
	m_pluginManager.LoadAvailablePlugins( pluginsDirectory.absolutePath().toStdString(), &error);
	m_trackerFactoryNameList = m_pluginManager.GetTTrackerFactoryNames();
	m_shapeFactoryNameList = m_pluginManager.GetTShapeFactoryNames();
	m_materialFactoryNameList = m_pluginManager.GetTMaterialFactoryNames();*/

}

void WindowMain::updateToolBar()
{
	// Load Tracker Plugins Tool Bar
	/*for( int i = 0; i < m_trackerFactoryNameList.size(); i++ ) {
		emit addToolBarItem(QString::fromUtf8((m_trackerFactoryNameList.at(i)).c_str()), Tracker);
	}
	// Load Shape plugins Tool Bar
	for( int i = 0; i < m_shapeFactoryNameList.size(); i++ ) {
		emit addToolBarItem(QString::fromUtf8((m_shapeFactoryNameList.at(i)).c_str()), Shape);
	}
	// Load Material plugins Tool Bar
	for( int i = 0; i < m_materialFactoryNameList.size(); i++ ) {
		emit addToolBarItem(QString::fromUtf8((m_materialFactoryNameList.at(i)).c_str()), Material);
	}*/
}

