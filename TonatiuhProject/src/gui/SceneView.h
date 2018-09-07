/*
 * SceneView.h
 *
 *  Created on: 22/08/2018
 *      Author: oitoiz
 */

#ifndef SCENEVIEW_H_
#define SCENEVIEW_H_

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>


class SceneView : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit SceneView(QQmlApplicationEngine *engine = nullptr);
    void buttonClicked();

    QObject *m_myParent;
	QQmlApplicationEngine *m_engine;
	QObject *m_root;

signals:
    void userNameChanged();

private:


};



#endif /* SCENEVIEW_H_ */
