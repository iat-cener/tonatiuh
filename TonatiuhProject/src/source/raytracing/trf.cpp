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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <cmath>
#include <vector>

#include <QFile>
#include <QTextStream>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include "InstanceNode.h"
#include "Matrix4x4.h"
#include "Photon.h"
#include "TPhotonMap.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "TLightKit.h"
#include "trf.h"
#include "TShapeKit.h"


SoSeparator* trf::DrawPhotonMapPoints( const TPhotonMap& map )
{

	SoSeparator* drawpoints = new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;
	std::vector< Photon* > photonsList = map.GetAllPhotons();
    unsigned int numRays=0;

	for( unsigned int i = 0; i < photonsList.size(); i++)
	{
		Point3D photon = photonsList[i]->pos;
		points->point.set1Value( numRays, photon.x, photon.y, photon.z );
		numRays++;
	}

	SoMaterial* myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);
	drawpoints->addChild(myMaterial);
	drawpoints->addChild(points);

	SoDrawStyle* drawstyle = new SoDrawStyle;
	drawstyle->pointSize = 3;
	drawpoints->addChild(drawstyle);

	SoPointSet* pointset = new SoPointSet;
	drawpoints->addChild(pointset);

	return drawpoints;

}

SoSeparator* trf::DrawPhotonMapRays( const TPhotonMap& map, unsigned long /*numberOfRays*/ )
{

	SoSeparator* drawrays = new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	QVector< int >	rayLengths;
	std::vector< Photon* > allRaysLists = map.GetAllPhotons();


	int nRay = 0;
	unsigned int photonIndex = 0;
	while( photonIndex < allRaysLists.size() )
	{
		unsigned long rayLength = 0;
		do
		{
			Photon* photon = allRaysLists[photonIndex];
			Point3D photonPosistion = photon->pos;
			points->point.set1Value( photonIndex, photonPosistion.x, photonPosistion.y, photonPosistion.z );
			photonIndex++;
			rayLength++;
		}while( photonIndex < allRaysLists.size() && allRaysLists[photonIndex]->id > 0 );


		rayLengths.push_back( rayLength );
		nRay++;

	}

	SoMaterial* myMaterial = new SoMaterial;
	myMaterial->diffuseColor.setValue(1.0f, 1.0f, 0.8f);
	drawrays->addChild( myMaterial );
	drawrays->addChild( points );

	int* lines = new int[rayLengths.size()];
	for( int l = 0; l < rayLengths.size(); l++ )
		lines[l] =  rayLengths[l];

	SoLineSet* lineset = new SoLineSet;
	lineset->numVertices.setValues( 0, rayLengths.size(), lines );
	drawrays->addChild( lineset );


	delete[] lines;
	return drawrays;

}
