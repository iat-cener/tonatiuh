/*
 * MaterialAngleDependentFactory.h
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */

#ifndef MATERIALANGLEDEPENDENTFACTORY_H_
#define MATERIALANGLEDEPENDENTFACTORY_H_



#include "TMaterialFactory.h"
#include "MaterialAngleDependent.h"


class MaterialAngleDependentFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
    Q_INTERFACES(TMaterialFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")
#endif

public:
   	QString TMaterialName() const;
   	QIcon TMaterialIcon() const;
   	MaterialAngleDependent* CreateTMaterial( ) const;
};

#endif /* MATERIALANGLEDEPENDENTFACTORY_H_ */
