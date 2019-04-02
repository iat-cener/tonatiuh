/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "MFVec2.h"
#include "UserMField.h"

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

		//UserMField::initClass();
		MFVec2::initClass();
		MaterialAngleDependentSpecular::initClass();
	    firstTime = false;
	}

	return new MaterialAngleDependentSpecular;
}

#if QT_VERSION < 0x050000 // pre Qt 5
Q_EXPORT_PLUGIN2(MaterialAngleDependentSpecular, MaterialAngleDependentSpecularFactory)
#endif
