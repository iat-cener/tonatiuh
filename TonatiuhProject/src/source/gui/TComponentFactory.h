/*
 * TComponentFactory.h
 *
 *  Created on: 06/03/2012
 *      Author: amutuberria
 */

#ifndef TCOMPONENTFACTORY_H_
#define TCOMPONENTFACTORY_H_

#include <QtPlugin>

class PluginManager;
class QString;
class QIcon;
class TSeparatorKit;

class TComponentFactory
{
public:
    virtual ~TComponentFactory() {}
    virtual QString TComponentName() const  = 0;
    virtual QIcon TComponentIcon() const = 0;
    virtual TSeparatorKit* CreateTComponent( PluginManager* pPluginManager ) const = 0;
    virtual TSeparatorKit* CreateTComponent( PluginManager* pPluginManager, int numberofParameters, QVector< QVariant > parametersList ) const = 0;
};

Q_DECLARE_INTERFACE( TComponentFactory, "tonatiuh.TComponentFactory")


#endif /* TCOMPONENTFACTORY_H_ */
