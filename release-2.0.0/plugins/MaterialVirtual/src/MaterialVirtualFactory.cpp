/*
 * MaterialFactoryVirtual.cpp
 *
 *  Created on: 22/05/2012
 *      Author: amutuberria
 */


#include <QIcon>

#include "MaterialVirtualFactory.h"

QString MaterialVirtualFactory::TMaterialName() const
{
	return QString("Virtual_Material");
}

QIcon MaterialVirtualFactory::TMaterialIcon() const
{
	return QIcon(":/icons/MaterialVirtual.png");
}

MaterialVirtual* MaterialVirtualFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
	    MaterialVirtual::initClass();
	    firstTime = false;
	}

	return new MaterialVirtual;
}

Q_EXPORT_PLUGIN2(MaterialVirtual, MaterialVirtualFactory)
