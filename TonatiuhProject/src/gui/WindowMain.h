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


class WindowMain : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit WindowMain(QQmlApplicationEngine * engine=nullptr);

    Q_INVOKABLE void buttonClicked();
    QQmlApplicationEngine *m_engine;

public slots:
	void buttonClicked2();

signals:
    void userNameChanged();

private:
    //QQmlApplicationEngine *m_engine;


};



#endif /* WINDOWMAIN_H_ */
