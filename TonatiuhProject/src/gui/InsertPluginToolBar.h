/*
 * InsertTrackerToolBar.h
 *
 *  Created on: 22/08/2018
 *      Author: oitoiz
 */

#ifndef INSERTPLUGINTOOLBAR_H_
#define INSERTPLUGINTOOLBAR_H_

#include <QObject>
#include <QString>

#include "PluginManager.h"


class InsertPluginToolBar : public QObject
{
    Q_OBJECT

public:
    explicit InsertPluginToolBar();
    Q_INVOKABLE void updateTrackerToolBar();
    Q_INVOKABLE void updateMaterialToolBar();
    Q_INVOKABLE void updateShapeToolBar();

signals:
    void addMaterialToolBarItem(QString pluginName, int xposition, QString icon);
    void addTrackerToolBarItem(QString pluginName, int xposition, QString icon);
    void addShapeToolBarItem(QString pluginName, int xposition, QString icon);


private:
    PluginManager m_pluginManager;
    std::vector<std::string > m_trackerFactoryNameList;
    std::vector<std::string > m_shapeFactoryNameList;
    std::vector<std::string > m_materialFactoryNameList;

};

#endif /* INSERTPLUGINTOOLBAR_H_ */
