#ifndef MATERIALVIRTUALFACTORY_H_
#define MATERIALVIRTUALFACTORY_H_

#include "TMaterialFactory.h"
#include "MaterialVirtual.h"


class MaterialVirtualFactory: public QObject, public TMaterialFactory
{
    Q_OBJECT
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TMaterialFactory")
#endif
    Q_INTERFACES(TMaterialFactory)

public:
   	QString TMaterialName() const;
   	QIcon TMaterialIcon() const;
   	MaterialVirtual* CreateTMaterial( ) const;
};

#endif /*MATERIALVIRTUALFACTORY_H_*/
