#include <iostream>

#include "WindowMainController.h"
#include "SceneView.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDir>

#include <Qt3DCore\QEntity>

#include "PluginManager.h"
#include "TreeView.h"
#include "TreeViewListController.h"

WindowMainController::WindowMainController(QObject *parent, ApplicationSettings * appSettings) :
	    QObject(parent)
{
	m_appSettings = appSettings;
}

void WindowMainController::init(QObject *parent)
{

	//! Connect file callbacks
	QObject::connect(parent, SIGNAL(newFileCb()), this, SLOT(newFileCb()));
	QObject::connect(parent, SIGNAL(openFileCb(QString)), this, SLOT(openFileCb(QString)));
	QObject::connect(parent, SIGNAL(saveFileCb()), this, SLOT(saveFileCb()));
	QObject::connect(parent, SIGNAL(saveFileAsCb(QString)), this, SLOT(saveFileAsCb(QString)));
	QObject::connect(parent, SIGNAL(printFileCb()), this, SLOT(printFileCb()));
	QObject::connect(parent, SIGNAL(closeFileCb()), this, SLOT(closeFileCb()));

	//! Connect insert element callbacks
	QObject::connect(parent, SIGNAL(addSeparatorKitCb()), this, SLOT(addSeparatorKitCb()));
	QObject::connect(parent, SIGNAL(addShapeKitCb()), this, SLOT(addShapeKitCb()));
	QObject::connect(parent, SIGNAL(addNodeCb(QString, QString)), this, SLOT(addNodeCb(QString, QString)));

	m_tonatiuhContent = parent->findChild<QObject *>("tonatiuhContent");

	//!Tree view GUI
	m_treeViewList = m_tonatiuhContent->findChild<QObject *>("treeviewList");
	m_treeviewlistController = new TreeViewListController(m_treeViewList, m_treeviewXml);
	//! Connect treeviewlist callbacks
	QObject::connect(m_treeViewList, SIGNAL(deleteNodeCb(QString)), this, SLOT(deleteNodeCb(QString)));

	//!Parameter view GUI
	m_parametersView = m_tonatiuhContent->findChild<QObject *>("parametersView");

	//!3D view GUI
	m_sceneRoot = m_tonatiuhContent->findChild<QObject *>("sceneRoot");

	//! Node structure
	m_treeviewXml = new TreeView("D:\\Olaia\\Tonatiuh\\RootNode.tnh");

	m_appSettings->loadRecentFiles();

}

void WindowMainController::newFile()
{
	const std::string filename = "D:\\Olaia\\Tonatiuh\\RootNode.tnh";
	m_workingFile = new File(filename);
}

void WindowMainController::openFile(std::string filename)
{
	m_workingFilename = filename;
	m_workingFile->openFile(filename);
	std::cout<<"open filessssssss"<<std::endl;

	m_appSettings->addRecentFile(QString::fromStdString(filename));

}

void WindowMainController::saveFile(std::string filename)
{
	m_workingFile->saveFile(filename);
}

void WindowMainController::addSeparatorKitCb()
{
	std::string nodetype = "GroupNode";
	std::string parentNodeURL = "//Tonatiuh//RootNode//childrenRoot";
	std::string nodeURL;
	TreeViewListController::treeViewNode parentNode = m_treeviewlistController->getTreeViewSelectedNode();
	//TODO: poner bien la ruta de parentNodeURL
	/*if (parentNode.nodeIdx == 0)
	{
		parentNodeURL = "//Tonatiuh" + m_treeviewlistController->getTreeViewSelectedNode().nodeUrl + "childrenRoot";
	}
	else
	{
		parentNodeURL = m_treeviewlistController->getTreeViewSelectedNode().nodeUrl;
	}*/
	std::cout<<"add separator kit"<<std::endl;
	try
	{
		m_treeviewXml->addNode(nodetype, parentNodeURL);
		//TODO: coger la URL que devuelva la función addNode
		nodeURL = parentNodeURL + "//" + "NODENAME";
		m_treeviewlistController->addTreeViewSeparatorKit(QString::fromUtf8(nodeURL.c_str()));

	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}


	//addParameters
	//std::cout<<selectedNodeURL<<std::endl;
	std::cout<<"update toolbar"<<std::endl;


}

void WindowMainController::addShapeKitCb()
{
	std::string nodetype = "SurfaceNode";

	TreeViewListController::treeViewNode parentNode = m_treeviewlistController->getTreeViewSelectedNode();
	std::string parentNodeURL = "//Tonatiuh//RootNode//childrenRoot";
	std::string nodeURL;
	//TODO: poner bien la ruta de parentNodeURL
	/*if (parentNode.nodeIdx == 0)
	{
		parentNodeURL = "//Tonatiuh" + m_treeviewlistController->getTreeViewSelectedNode().nodeUrl + "childrenRoot";
	}
	else
	{
		parentNodeURL = m_treeviewlistController->getTreeViewSelectedNode().nodeUrl;
	}*/
	try
	{
		m_treeviewXml->addNode(nodetype, parentNodeURL);
		//TODO: coger la URL que devuelva la función addNode
		nodeURL = parentNodeURL + "//" + "NODENAME";
		m_treeviewlistController->addTreeViewShapeKit(QString::fromUtf8(nodeURL.c_str()));

	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}

void WindowMainController::addNodeCb(QString nodeType, QString nodeName)
{

	TreeViewListController::treeViewNode parentNode = m_treeviewlistController->getTreeViewSelectedNode();
	std::string parentNodeURL = "//Tonatiuh//RootNode//childrenRoot";
	std::string nodeURL;
	//TODO: poner bien la ruta de parentNodeURL
	/*if (parentNode.nodeIdx == 0)
	{
		parentNodeURL = "//Tonatiuh" + m_treeviewlistController->getTreeViewSelectedNode().nodeUrl + "childrenRoot";
	}
	else
	{
		parentNodeURL = m_treeviewlistController->getTreeViewSelectedNode().nodeUrl;
	}*/
	try
	{
		m_treeviewXml->addNode(nodeType.toStdString(), parentNodeURL);
		//TODO: coger la URL que devuelva la función addNode y coger el nombre por defecto para
		//meterlo como parametro de entrada de las funciones addTreeViewMaterialNode, addTreeViewShapeNode, addTreeViewTrackerNode
		nodeURL = parentNodeURL + "//" + "NODENAME";
		if (nodeType.toStdString() == "Material")
			m_treeviewlistController->addTreeViewMaterialNode(nodeName, QString::fromUtf8(nodeURL.c_str()));
		else if (nodeType.toStdString() == "Shape")
			m_treeviewlistController->addTreeViewShapeNode(nodeName, QString::fromUtf8(nodeURL.c_str()));
		else if (nodeType.toStdString() == "Tracker")
			m_treeviewlistController->addTreeViewTrackerNode(nodeName, QString::fromUtf8(nodeURL.c_str()));
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}

}

void WindowMainController::deleteNodeCb(QString nodeUrl)
{
	try
	{
		//TODO: delete node with the real url
		/*m_treeviewXml->deleteNode(nodeUrl.toStdString());
		m_treeviewlistController->deleteTreeViewNode(nodeUrl);*/
		m_treeviewXml->deleteNode("//Tonatiuh//RootNode//childrenRoot//nodename");
		m_treeviewlistController->deleteTreeViewNode("//Tonatiuh//RootNode//childrenRoot//nodename");
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}

void WindowMainController::newFileCb()
{
	std::cout<<"newfile"<<std::endl;
	//! Call to newFiel function
}

void WindowMainController::openFileCb(QString fileUrl)
{
	m_workingFilename = (QUrl( fileUrl ).toLocalFile()).toStdString();
	std::cout<<"open"<<m_workingFilename<<std::endl;
	try
	{
		m_workingFile->openFile(m_workingFilename);
		m_appSettings->addRecentFile(fileUrl);
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}

}
void WindowMainController::saveFileCb()
{
	try
	{
		m_workingFile->saveFile(m_workingFilename);
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}
void WindowMainController::saveFileAsCb(QString fileUrl)
{
	m_workingFilename = (QUrl( fileUrl ).toLocalFile()).toStdString();
	std::cout<<"saveas"<<m_workingFilename<<std::endl;
	try
	{
		m_workingFile->saveFile(m_workingFilename);
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}
void WindowMainController::printFileCb()
{
	std::cout<<"printFileCb"<<std::endl;
}
void WindowMainController::closeFileCb()
{
	std::cout<<"closeFileCb"<<std::endl;
}
