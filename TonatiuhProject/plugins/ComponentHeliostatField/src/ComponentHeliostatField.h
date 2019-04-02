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


#ifndef COMPONENTHELIOSTATFIELD_H_
#define COMPONENTHELIOSTATFIELD_H_

#include <vector>

class PluginManager;
class Point3D;
class QString;
class SoNode;
class TSeparatorKit;
class TShapeFactory;
class TMaterial;
class TTrackerFactory;

class ComponentHeliostatField
{

public:
	ComponentHeliostatField( PluginManager* pPluginManager );
	~ComponentHeliostatField();

	TSeparatorKit* CreateField();
	TSeparatorKit* CreateField(QVector< QVariant >  argumentList);

private:
	void CreateHeliostatZones( std::vector< Point3D >  heliostatCenterList,
			TSeparatorKit* parentNode,
			TTrackerFactory* heliostatTrackerFactory,
			TShapeFactory* heliostatShaperFactory,
			int heliostat,
			TSeparatorKit* heliostatComponent,
			double heliostatWidth,
			double heliostatHeight,
			double heliostatRadius,
			TMaterial* materialNode,
			std::vector< Point3D > aimingPointList,
			int eje );

	TSeparatorKit* OpenHeliostatComponent( QString fileName );

	PluginManager* m_pPluginManager;
};

#endif /* COMPONENTHELIOSTATFIELD_H_ */
