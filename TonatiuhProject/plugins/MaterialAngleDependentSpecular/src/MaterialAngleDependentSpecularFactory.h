/*
 * MaterialAngleDependentSpecularFactory.h
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */

#ifndef MATERIALANGLEDEPENDINGSPECULARFACTORY_H_
#define MATERIALANGLEDEPENDINGSPECULARFACTORY_H_



#include "TMaterialFactory.h"
#include "MaterialAngleDependentSpecular.h"


class MaterialAngleDependentSpecularFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
    Q_INTERFACES(TMaterialFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")
#endif

public:
   	QString TMaterialName() const;
   	QIcon TMaterialIcon() const;
   	MaterialAngleDependentSpecular* CreateTMaterial( ) const;
};

#endif /* MATERIALANGLEDEPENDINGSPECULARFACTORY_H_ */
