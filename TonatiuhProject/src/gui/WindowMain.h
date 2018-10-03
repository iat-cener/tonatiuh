/*
 * WindowMain.h
 *
 *  Created on: 22/08/2018
 *      Author: oitoiz
 */

#ifndef WINDOWMAIN_H_
#define WINDOWMAIN_H_

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include "SceneView.h"

#include "PluginManager.h"


class WindowMain : public QObject
{
    Q_OBJECT

public:
    enum PluginType {Tracker, Shape, Material};
    Q_ENUM(PluginType)
    explicit WindowMain(QQmlApplicationEngine * engine=nullptr);

    Q_INVOKABLE void updateToolBar();
    QQmlApplicationEngine *m_engine;

signals:
   void addToolBarItem(QString pluginName, PluginType pt);


private:
   PluginManager m_pluginManager;
   std::vector<std::string > m_trackerFactoryNameList;
   std::vector<std::string > m_shapeFactoryNameList;
   std::vector<std::string > m_materialFactoryNameList;
    //QQmlApplicationEngine *m_engine;


};

#endif /* WINDOWMAIN_H_ */
