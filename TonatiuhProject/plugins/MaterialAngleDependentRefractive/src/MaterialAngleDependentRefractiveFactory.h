/*
 * MaterialAngleDependentRefractiveFactory.h
 *
 *  Created on: 10/12/2015
 *      Author: iles
 */

#ifndef MATERIALANGLEDEPENDINGREFRACTIVEFACTORY_H_
#define MATERIALANGLEDEPENDINGREFRACTIVEFACTORY_H_



#include "TMaterialFactory.h"
#include "MaterialAngleDependentRefractive.h"


class MaterialAngleDependentRefractiveFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
    Q_INTERFACES(TMaterialFactory)
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")
#endif

public:
   	QString TMaterialName() const;
   	QIcon TMaterialIcon() const;
   	MaterialAngleDependentRefractive* CreateTMaterial( ) const;
};

#endif /* MATERIALANGLEDEPENDINGREFRACTIVEFACTORY_H_ */
