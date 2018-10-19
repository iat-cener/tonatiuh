/*
 * WindowMain.h
 *
 *  Created on: 22/08/2018
 *      Author: oitoiz
 */

#ifndef WINDOWMAINCONTROLLER_H_
#define WINDOWMAINCONTROLLER_H_

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include "SceneView.h"
#include "TreeView.h"
#include "PluginManager.h"
#include "TreeViewListController.h"
#include "ParametersViewController.h"
#include "File.h"
#include "ApplicationSettings.h"


class WindowMainController : public QObject
{
    Q_OBJECT

public:
    enum PluginType {Tracker, Shape, Material};
    Q_ENUM(PluginType)
    explicit WindowMainController(QObject *parent = nullptr, ApplicationSettings * appSettings = nullptr);

    void init(QObject *parent);
    void newFile();
    void openFile(std::string filename);
    void saveFile(std::string filename);
    void setNodeProperties(std::string nodeName,  std::string pluginName);


public slots:
	//! File callbacks
	void newFileCb();
	void openFileCb(QString fileUrl);
	void saveFileCb();
	void saveFileAsCb(QString fileUrl);
	void printFileCb();
	void closeFileCb();

    //! Node Callbacks
    void deleteNodeCb(QString nodeUrl);
    void addSeparatorKitCb();
    void addShapeKitCb();
    void addNodeCb(QString nodeType, QString nodeName);

    //! Parameters Callbacks
    void updateParameterViewCb(QString nodeName, QString pluginName);
    void loadChildParametersViewCb(QString nodeUrl, QString childUrl1, QString childName1, QString childPluginName1,
    		QString childUrl2, QString childName2, QString childPluginName);



signals:

	//void deleteNode_cb(QString nodeUrl);
   //void addToolBarItem(QString pluginName, PluginType pt);
   //void getSelectedNodeURL();

private:
	void loadPlugins();
    void setTrackerProperties(std::string name);
    void setShapeProperties(std::string name);
    void setMaterialProperties(std::string name);
    void setSeparatorKitProperties();
    PluginManager m_pluginManager;
    std::vector<std::string > m_trackerFactoryNameList;
    std::vector<std::string > m_shapeFactoryNameList;
    std::vector<std::string > m_materialFactoryNameList;
    std::string m_currentIcon;
    std::vector< std::pair<std::string, std::string> >  m_currentParameterList;

    TreeView  * m_treeviewXml;
    File * m_file;
    QObject * m_tonatiuhContent;
    QObject * m_treeViewList;
    QObject * m_parametersView;
    QObject * m_sceneRoot;
    TreeViewListController *m_treeviewlistController;
    ParametersViewController *m_parametersViewController;
    File * m_workingFile;
    std::string m_workingFilename;
    ApplicationSettings * m_appSettings;
};

#endif /* WINDOWMAINCONTROLLER_H_ */
