/*
 * MaterialOneSideSpecularFactory.h
 *
 *  Created on: 17/11/2015
 *      Author: amutuberria
 */

#ifndef MATERIALONESIDESPECULARFACTORY_H_
#define MATERIALONESIDESPECULARFACTORY_H_


#include "TMaterialFactory.h"
#include "MaterialOneSideSpecular.h"


class MaterialOneSideSpecularFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
    Q_INTERFACES(TMaterialFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")
#endif

public:
   	QString TMaterialName() const;
   	QIcon TMaterialIcon() const;
   	MaterialOneSideSpecular* CreateTMaterial( ) const;
};


#endif /* MATERIALONESIDESPECULARFACTORY_H_ */
