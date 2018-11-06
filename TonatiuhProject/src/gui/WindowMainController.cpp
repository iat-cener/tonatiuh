
#include "WindowMainController.h"
#include "SceneView.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>

#include <QDir>

#include <Qt3DCore\QEntity>

#include "PluginManager.h"
#include "TreeView.h"
#include "TreeViewListController.h"
#include "TTrackerFactory.h"
#include "TTracker.h"
#include "TMaterialFactory.h"
#include "TMaterial.h"
#include "TShapeFactory.h"
#include "TShape.h"

WindowMainController::WindowMainController(QObject *parent, ApplicationSettings * appSettings) :
	    QObject(parent)
{
	m_appSettings = appSettings;
	const std::string filename = "D:\\Olaia\\Tonatiuh\\RootNode.tnh";
	m_workingFile = new File(filename);
}

void WindowMainController::init(QObject *parent)
{
	loadPlugins();

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

	//! Connect parameters view callbacks
	QObject::connect(parent, SIGNAL(updateParameterViewCb(QString, QString)), this, SLOT(updateParameterViewCb(QString, QString)));
	QObject::connect(parent, SIGNAL(loadChildParametersViewCb(QString, QString, QString, QString, QString, QString, QString)),
			this, SLOT(loadChildParametersViewCb(QString, QString, QString, QString, QString, QString, QString)));
	m_tonatiuhContent = parent->findChild<QObject *>("tonatiuhContent");

	//!Tree view GUI
	m_treeViewList = m_tonatiuhContent->findChild<QObject *>("treeviewList");
	m_treeviewlistController = new TreeViewListController(m_treeViewList, m_treeviewXml);
	//m_parametersViewController = new ParametersViewController();

	//! Connect treeviewlist callbacks
	QObject::connect(m_treeViewList, SIGNAL(deleteNodeCb(QString)), this, SLOT(deleteNodeCb(QString)));
	QObject::connect(m_treeViewList, SIGNAL(copyNodeCb(QString)), this, SLOT(copyNodeCb(QString)));
	QObject::connect(m_treeViewList, SIGNAL(pasteNodeCb(QString)), this, SLOT(pasteNodeCb(QString)));
	QObject::connect(m_treeViewList, SIGNAL(pasteRefNodeCb(QString)), this, SLOT(pasteRefNodeCb(QString)));


	//!Parameter view GUI
	m_parametersView = m_tonatiuhContent->findChild<QObject *>("parametersViewLoader");
	m_parametersViewController = new ParametersViewController(m_parametersView);

	//!3D view GUI
	m_sceneRoot = m_tonatiuhContent->findChild<QObject *>("sceneRoot");


	//! Node structure
	m_treeviewXml = new TreeView(*m_workingFile);
	//m_treeviewXml = new TreeView("D:\\Olaia\\Tonatiuh\\RootNode.tnh");

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
	std::string parentNodeURL = "//Tonatiuh//sceneNode//rootNode";
	TreeViewListController::treeViewNode parentNode = m_treeviewlistController->getTreeViewSelectedNode();
	//std::string parentNodeURL = parentNode.nodeUrl;
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
	std::cout<<"add separator kit"<<std::endl;
	try
	{
		nodeURL = m_treeviewXml->addNode(nodetype, parentNodeURL);
		std::cout<< "nodeUrl" << nodeURL << std::endl;

		//! Set default parameters
		//! Group Node parameters (default):
		//! 	- Translation: 0 0 0
		//! 	- Rotation: 0 0 1  0
		//!		- ScaleFactor: 1 1 1
		//! 	- Center 0 0 0
		//TODO: llamar a la función m_treeviewXml->setParameter(nodeurl, parmeter, value)

		//TODO: coger la URL que devuelva la función addNode
		//nodeURL = parentNodeURL + "//" + "NODENAME";

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
	std::string parentNodeURL = parentNode.nodeUrl;
	//std::string parentNodeURL = "//Tonatiuh//sceneNode//rootNode";
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
		nodeURL = m_treeviewXml->addNode(nodetype, parentNodeURL);
		std::cout<< "nodeUrl shape kit" << nodeURL << std::endl;
		//m_treeviewXml->addNode(nodetype, parentNodeURL);
		//TODO: coger la URL que devuelva la función addNode
		//nodeURL = parentNodeURL + "//" + "NODENAME";
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
	std::string parentNodeURL = parentNode.nodeUrl;
	//std::string parentNodeURL = "//Tonatiuh//sceneNode//rootNode";
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
		nodeURL = m_treeviewXml->addNode(nodeType.toStdString(), parentNodeURL);
		std::cout<< "nodeUrl node" << nodeURL << std::endl;
		//m_treeviewXml->addNode(nodeType.toStdString(), parentNodeURL);
		//TODO: coger la URL que devuelva la función addNode y coger el nombre por defecto para
		//meterlo como parametro de entrada de las funciones addTreeViewMaterialNode, addTreeViewShapeNode, addTreeViewTrackerNode
		//nodeURL = parentNodeURL + "//" + "NODENAME";
		if (nodeType.toStdString() == "Material")
		{
			//! Set material parameters
			//! TODO
			setMaterialProperties(nodeName.toStdString());
			m_treeviewlistController->addTreeViewMaterialNode(QString::fromUtf8(m_currentIcon.c_str()), QString::fromUtf8(nodeURL.c_str()), nodeName);
			//TODO: Update Parameters View with m_currentParameterList

		}
		else if (nodeType.toStdString() == "Shape")
		{
			setShapeProperties(nodeName.toStdString());
			m_treeviewlistController->addTreeViewShapeNode(QString::fromUtf8(m_currentIcon.c_str()), QString::fromUtf8(nodeURL.c_str()), nodeName);
			//TODO: Update Parameters View  with m_currentParameterList

		}
		else if (nodeType.toStdString() == "Tracker")
		{
			setTrackerProperties(nodeName.toStdString());
			m_treeviewlistController->addTreeViewTrackerNode(QString::fromUtf8(m_currentIcon.c_str()), QString::fromUtf8(nodeURL.c_str()), nodeName);
			//TODO: Update Parameters View with m_currentParameterList

		}
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
		m_treeviewXml->deleteNode(nodeUrl.toStdString());
		m_treeviewlistController->deleteTreeViewNode(nodeUrl);
		/*m_treeviewXml->deleteNode("//Tonatiuh//RootNode//childrenRoot//nodename");
		m_treeviewlistController->deleteTreeViewNode("//Tonatiuh//RootNode//childrenRoot//nodename");*/
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}


void WindowMainController::cutNodeCb(QString nodeUrl)
{
	try
	{
		m_treeviewXml->copyNode(nodeUrl.toStdString());
		m_treeviewXml->deleteNode(nodeUrl.toStdString());
		m_treeviewlistController->deleteTreeViewNode(nodeUrl);
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}

}

void WindowMainController::copyNodeCb(QString nodeUrl)
{
	try
	{
		m_treeviewXml->copyNode(nodeUrl.toStdString());
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}

}
void WindowMainController::copyRefNodeCb(QString nodeUrl)
{
	try
	{
		m_treeviewXml->copyRefNode(nodeUrl.toStdString());
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
}

void WindowMainController::pasteNodeCb(QString nodeUrl)
{
	try
	{
		//TODO: delete node with the real url
		m_treeviewXml->pasteNode(nodeUrl.toStdString());
		//TODO: Update Parameters View
		//TODO: Update 3D View
	}
	catch(std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}

}

void WindowMainController::pasteRefNodeCb(QString nodeUrl)
{
	try
	{
		//TODO: delete node with the real url
		m_treeviewXml->pasteRefNode(nodeUrl.toStdString());
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


void WindowMainController::updateParameterViewCb(QString nodeName, QString pluginName)
{
	setNodeProperties(nodeName.toStdString(), pluginName.toStdString());
	m_parametersViewController->updateParametersView(m_currentParameterList, pluginName);
}

void WindowMainController::loadChildParametersViewCb(QString nodeUrl,
		QString childUrl1, QString childName1, QString childPluginName1,
		QString childUrl2, QString childName2, QString childPluginName2)
{
	std::vector< std::pair<std::string, std::string> > child1_paramList;
	std::vector< std::pair<std::string, std::string> > child2_paramList;

	//! Set child 1 parameters
	setNodeProperties(childName1.toStdString(), childPluginName1.toStdString());
	child1_paramList = m_currentParameterList;

	//! Set child 2 parameters
	setNodeProperties(childName2.toStdString(), childPluginName2.toStdString());
	child2_paramList = m_currentParameterList;

	m_parametersViewController->updateChildParametersView(child1_paramList, child2_paramList, childPluginName1, childPluginName2);



}

void WindowMainController::loadPlugins()
{
	QString str_pluginsDirectory= QApplication::applicationDirPath() + QDir::separator() + "plugins";
	QDir pluginsDirectory(str_pluginsDirectory);
	std::string error;
	m_pluginManager.LoadAvailablePlugins( pluginsDirectory.absolutePath().toStdString(), &error);
	m_trackerFactoryNameList = m_pluginManager.GetTTrackerFactoryNames();
	m_shapeFactoryNameList = m_pluginManager.GetTShapeFactoryNames();
	m_materialFactoryNameList = m_pluginManager.GetTMaterialFactoryNames();
}

void WindowMainController::setTrackerProperties(std::string name)
{
	//TODO: the parameter values must be updated with the xml values, not the default values
	// Load Tracker Properties
	for( unsigned int i = 0; i < m_trackerFactoryNameList.size(); i++ ) {
		if (name == m_trackerFactoryNameList.at(i))
		{
			std::shared_ptr< TTracker > pTTrackerFactory = m_pluginManager.CreateTTracker(m_trackerFactoryNameList.at(i));
			m_currentIcon = "qrc" + pTTrackerFactory->GetIcon();
			std::vector<std::string> parameterList = pTTrackerFactory->GetVisibleParametersName();
			for( unsigned int p = 0; p < parameterList.size(); p++ )
			{
				std::string parameterName = parameterList[p];
				std::string parameterValue = pTTrackerFactory->GetParameterToString( parameterName );
				std::pair<std::string, std::string> parametersPair;
				parametersPair = std::make_pair(std::string(parameterName),std::string(parameterValue));
				m_currentParameterList.push_back( parametersPair);
			}
			//m_currentParameterList = pTTrackerFactory->GetVisibleParametersName();
		}
	}
}

void WindowMainController::setShapeProperties(std::string name)
{
	//TODO: the parameter values must be updated with the xml values, not the default values
	// Load Shape Properties
	for( unsigned int i = 0; i < m_shapeFactoryNameList.size(); i++ ) {
		if (name == m_shapeFactoryNameList.at(i))
		{
			std::shared_ptr< TShape > pTShapeFactory = m_pluginManager.CreateTShape(m_shapeFactoryNameList.at(i));
			m_currentIcon = "qrc" + pTShapeFactory->GetIcon();
			std::vector<std::string> parameterList = pTShapeFactory->GetVisibleParametersName();
			for( unsigned int p = 0; p < parameterList.size(); p++ )
			{
				std::string parameterName = parameterList[p];
				std::string parameterValue = pTShapeFactory->GetParameterToString( parameterName );
				std::pair<std::string, std::string> parametersPair;
				parametersPair = std::make_pair(std::string(parameterName),std::string(parameterValue));
				m_currentParameterList.push_back( parametersPair);
			}
		}
	}
}

void WindowMainController::setMaterialProperties(std::string name)
{
	//TODO: the parameter values must be updated with the xml values, not the default values
	// Load Material Properties
	for( unsigned int i = 0; i < m_materialFactoryNameList.size(); i++ ) {
		if (name == m_materialFactoryNameList.at(i))
		{
			std::shared_ptr< TMaterial > pTMaterialFactory = m_pluginManager.CreateTMaterial(m_materialFactoryNameList.at(i));
			m_currentIcon = "qrc" + pTMaterialFactory->GetIcon();
			std::vector<std::string> parameterList = pTMaterialFactory->GetVisibleParametersName();
			for( unsigned int p = 0; p < parameterList.size(); p++ )
			{
				std::string parameterName = parameterList[p];
				std::string parameterValue = pTMaterialFactory->GetParameterToString( parameterName );
				std::pair<std::string, std::string> parametersPair;
				parametersPair = std::make_pair(std::string(parameterName),std::string(parameterValue));
				m_currentParameterList.push_back( parametersPair);
			}
		}
	}

}

void WindowMainController::setSeparatorKitProperties()
{
	//TODO: the parameter values must be updated with the xml values, not the default values
	// Load Separator kit Properties
	std::vector<std::string> parameterList = {"Translation", "Rotation", "ScaleFactor", "ScaleOrientation", "Center"};
	for( unsigned int p = 0; p < parameterList.size(); p++ )
	{
		std::string parameterName = parameterList[p];
		std::string parameterValue = "0"; //TODO: get parameter from node structure
		std::pair<std::string, std::string> parametersPair;
		parametersPair = std::make_pair(std::string(parameterName),std::string(parameterValue));
		m_currentParameterList.push_back( parametersPair);
	}


}

void WindowMainController::setNodeProperties(std::string nodeName, std::string pluginName)
{
	// Load Node Properties
	m_currentParameterList.clear();
	std::cout<<"nodeName"<<nodeName<<std::endl;
	if (nodeName == "Shape")
	{
		setShapeProperties(pluginName);
	}
	else if (nodeName == "Tracker")
	{
		setTrackerProperties(pluginName);
	}
	else if (nodeName == "Material")
	{
		setMaterialProperties(pluginName);
	}
	else if (nodeName == "ShapeKit")
	{
		//Do nothing
	}
	else if (nodeName =="SeparatorKit")
	{
		setSeparatorKitProperties();
	}
}

