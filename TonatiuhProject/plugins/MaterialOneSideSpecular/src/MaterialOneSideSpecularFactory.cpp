/*
 * MaterialOneSideSpecularFactory.cpp
 *
 *  Created on: 17/11/2015
 *      Author: amutuberria
 */




#include <QIcon>

#include "MaterialOneSideSpecularFactory.h"

QString MaterialOneSideSpecularFactory::TMaterialName() const
{
	return QString("Specular_One_Side_Material");
}

QIcon MaterialOneSideSpecularFactory::TMaterialIcon() const
{
	return QIcon(":/icons/MaterialOneSideSpecular.png");
}

MaterialOneSideSpecular* MaterialOneSideSpecularFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
		MaterialOneSideSpecular::initClass();
	    firstTime = false;
	}

	return new MaterialOneSideSpecular;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(MaterialOneSideSpecular, MaterialOneSideSpecularFactory)
#endif

