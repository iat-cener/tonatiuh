/*
 * MaterialAngleDependentFactory.cpp
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */



#include <QIcon>

#include "MaterialAngleDependentFactory.h"

QString MaterialAngleDependentFactory::TMaterialName() const
{
	return QString("Angle_Dependent_Material");
}

QIcon MaterialAngleDependentFactory::TMaterialIcon() const
{
	return QIcon(":/icons/MaterialAngleDependent.png");
}

MaterialAngleDependent* MaterialAngleDependentFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
		MaterialAngleDependent::initClass();
	    firstTime = false;
	}

	return new MaterialAngleDependent;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(MaterialAngleDependent, MaterialAngleDependentFactory)
#endif
