/*
 * MaterialAngleDependentSpecularFactory.cpp
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */



#include <QIcon>

#include "MaterialAngleDependentSpecularFactory.h"

QString MaterialAngleDependentSpecularFactory::TMaterialName() const
{
	return QString("Angle-Dependent_Specular_Material");
}

QIcon MaterialAngleDependentSpecularFactory::TMaterialIcon() const
{
	return QIcon(":/icons/MaterialAngleDependentSpecular.png");
}

MaterialAngleDependentSpecular* MaterialAngleDependentSpecularFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
		MaterialAngleDependentSpecular::initClass();
	    firstTime = false;
	}

	return new MaterialAngleDependentSpecular;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(MaterialAngleDependentSpecular, MaterialAngleDependentSpecularFactory)
#endif
