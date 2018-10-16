#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <iostream>

#include "WindowMainController.h"
#include "InsertPluginToolBar.h"
#include "TonatiuhToolBar.h"

#include <QDir>
#include <QIcon>

#include <TNodesDatabase.h>
#include "TMaterialFactory.h"

#include "PluginManager.h"
#include "RayCasting.h"
//#include "TNodesDocument.h"
#include "TPhotonMap.h"
#include "TShapeFactory.h"
#include "TSunshapeFactory.h"
#include "TTrackerFactory.h"
#include "ApplicationSettings.h"




int main(int argc, char *argv[])
{

	QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/qml/icons/Tonatiuh.ico"));

    TNodesDatabase::Init();

    qmlRegisterType<InsertPluginToolBar>("inserttrackertoolbar", 1, 0, "InsertTrackerToolBar");
    qmlRegisterType<InsertPluginToolBar>("insertmaterialtoolbar", 1, 0, "InsertMaterialToolBar");
    qmlRegisterType<InsertPluginToolBar>("insertshapetoolbar", 1, 0, "InsertShapeToolBar");
   // qmlRegisterType<InsertPluginToolBar>("insertshapemenu", 1, 0, "InsertShapeMenu");

    qmlRegisterType<TonatiuhToolBar>("tonatiuhtoolbarupdater", 1, 0, "TonatiuhToolBarUpdater");
    //qmlRegisterType<WindowMainController>("windowmaincontroller", 1, 0, "WindowMainController");
    //qRegisterMetaType<ApplicationSettings*>();


	QQmlApplicationEngine engine;

	//! Set application settings
	QCoreApplication::setOrganizationName("CENER");
	QCoreApplication::setApplicationName("Tonatiuh");

	ApplicationSettings * appSettings = new ApplicationSettings();
	engine.rootContext()->setContextProperty("appSettings", appSettings);



	engine.load(QUrl("qrc:/qml/qml/main.qml"));


	QList<QObject *> parent = engine.rootObjects();
	WindowMainController * windowMainController = new WindowMainController(parent.at(0), appSettings);
	windowMainController->init(parent.at(0));


	if( argc > 1 )
	{
		//! Call to openFile
		windowMainController->newFile();
		//windowMainController->openFile(argv[1]);
		//windowMainController->saveFile("D:\\Olaia\\Tonatiuh\\RootNode.tnh");

	}
	else
	{
		//! Call to newFile

		windowMainController->newFile();

	}


    return app.exec();

}
