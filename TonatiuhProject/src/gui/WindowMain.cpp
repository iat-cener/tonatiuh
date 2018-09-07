#import "WindowMain.h"
#import "SceneView.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>

 #include <Qt3DCore\QEntity>

/*WindowMain::WindowMain(QObject *root) :
    QObject(root)
{*/
WindowMain::WindowMain(QQmlApplicationEngine * engine) :
	    QObject()
{
	m_engine = engine;
}

void WindowMain::buttonClicked()
{
	std::cout<<"se ha clicado el boton"<<std::endl;
	SceneView * sceneView = new SceneView(m_engine);
	sceneView->buttonClicked();
}

void WindowMain::buttonClicked2()
{
	std::cout<<"se ha clicado el boton2"<<std::endl;
	SceneView * sceneView = new SceneView(m_engine);
	sceneView->buttonClicked();
}

