/*
 * MaterialAngleDependentRefractiveFactory.cpp
 *
 *  Created on: 10/12/2015
 *      Author: iles
 */

#include "MFVec3.h"
#include "UserMField.h"

#include <QIcon>

#include "MaterialAngleDependentRefractiveFactory.h"

QString MaterialAngleDependentRefractiveFactory::TMaterialName() const
{
	return QString("Angle-Dependent_Refractive_Material");
}

QIcon MaterialAngleDependentRefractiveFactory::TMaterialIcon() const
{
	return QIcon(":/icons/MaterialAngleDependentRefractive.png");
}

MaterialAngleDependentRefractive* MaterialAngleDependentRefractiveFactory::CreateTMaterial( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{

		//UserMField::initClass();
		MFVec3::initClass();
		MaterialAngleDependentRefractive::initClass();
	    firstTime = false;
	}

	return new MaterialAngleDependentRefractive;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(MaterialAngleDependentRefractive, MaterialAngleDependentRefractiveFactory)
#endif
