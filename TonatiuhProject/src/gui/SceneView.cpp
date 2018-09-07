#import "SceneView.h"
#include <iostream>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlContext>

 #include <Qt3DCore\QEntity>

/*SceneView::SceneView(QObject *root) :
    QObject(root)
{*/
SceneView::SceneView(QQmlApplicationEngine * engine) :
	    QObject(engine)
{
	//QQmlComponent component(&m_engine, (QUrl("qrc:/qml/qml/main.qml")));
	//QObject *root = component.create();
	//QObject *root = qmlView->rootObject();
	//QQmlApplicationEngine * engine = qobject_cast<QQmlApplicationEngine *>(qmlEngine(this));

	/*QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
	m_engine = qobject_cast<QQmlApplicationEngine*>(engine);*/
	m_engine = engine;
	m_root = m_engine->rootObjects().first();

	m_myParent = m_root->findChild<QObject *>("sceneRoot");
	/*QQmlComponent component(m_engine, (QUrl("qrc:/qml/qml/Torus.qml")));

	QObject *myObject = component.create();
	myObject->setParent((m_myParent));
	myObject->setProperty("objectName", "NewTorus");
	m_engine->setObjectOwnership(myObject, QQmlEngine::CppOwnership);

	QObject *newTorus = m_root->findChild<QObject *>("torusEntity");
	QObject *newTorus1 = m_root->findChild<QObject *>("NewTorus");*/


	std::cout<<"alkadjlk"<<std::endl;
	/*QObject *object = root->findChild<QObject *>("scene");
	object->setProperty("color", "steelBlue");*/
}

void SceneView::buttonClicked()
{

	/*QQmlEngine *engine = QQmlEngine::contextForObject(root)->engine();
	m_engine = qobject_cast<QQmlApplicationEngine*>(engine);
	std::cout<<"hola"<<std::endl;*/
	/*QQmlApplicationEngine engine;
	engine.load(QUrl("qrc:/qml/qml/main.qml"));
	m_engine = &engine;
	QObject *root = m_engine->rootObjects().first();*/

	/*QObject *contentObject = root->findChild<QObject *>("content");
	QObject *scene3d = root->findChild<QObject *>("scene3d");*/
	QQmlComponent component(m_engine, (QUrl("qrc:/qml/qml/Torus.qml")));
	/*QObject *myParent = root->findChild<QObject *>("sceneRoot");
	QObject *viewScene = root->findChild<QObject *>("viewScene");

	Qt3DCore::QEntity *childEntit = scene3d->findChild<Qt3DCore::QEntity  *>("sceneRoot");*/
	QObject *myObject = component.create();
	myObject->setProperty("visible", true);
	//myObject->setProperty("visible", true);

	myObject->setParent((m_myParent));


	/*QQmlComponent component(m_engine, (QUrl("qrc:/qml/qml/Torus.qml")));
	QObject *myObject = component.create();
	myObject->setParent((m_myParent));*/

	m_engine->setObjectOwnership(myObject, QQmlEngine::CppOwnership);

	QObject *newTorus = m_root->findChild<QObject *>("torusEntity");
	std::cout<<"aaaaa"<<std::endl;

	/*QQmlComponent component(qmlView->engine(), QUrl("qrc:/qml/qml/Sphere.qml"));
		QObject *myObject = component.create();
		QObject *root = qmlView->rootObject();
		QObject *parentObject = root->findChild<QObject *>("viewScene");
		QObject *parentObject2 = root->findChild<QObject *>("scene");
		myObject->setParent(parentObject);
		qmlView->engine()->setObjectOwnership(myObject, QQmlEngine::CppOwnership);*/

}

