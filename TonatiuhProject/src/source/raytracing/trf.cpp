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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/
#include <cmath>

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


/**
 * Saves to \a filename file the \a photonMap all information.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportAll( QString fileName, double wPhoton, TPhotonMap* photonMap )
{
	QFile exportFile( fileName );
	if(!exportFile.open( QIODevice::WriteOnly ) )	return 0;

	QDataStream out( &exportFile );
	out<< wPhoton;

	QList< Photon* > photonsList = photonMap->GetAllPhotons();
	for (int i = 0; i < photonsList.size(); ++i)
	{
		Photon* node = photonsList[i];
		Point3D photon = node->pos;
		double id = node->id;
		double prev_id = ( node->prev ) ? node->prev->id : 0;
		double next_id = ( node->next ) ? node->next->id : 0;
		out<<id <<photon.x << photon.y << photon.z <<prev_id << next_id;
	}
	exportFile.close();

	return 1;
}

/**
 * Saves to \a filename file the \a selectedSurface photons in scene global coordinates.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportSurfaceGlobalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap )
{
	if( ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
		( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TLightKit::getClassTypeId() ) ) )
		return 0;

	QFile exportFile( fileName );
	if(!exportFile.open( QIODevice::WriteOnly ) )	return 0;

	QDataStream out( &exportFile );
	out<< wPhoton;

	QList< Photon* > nodePhotonsList = photonMap->GetSurfacePhotons( selectedSurface );
	if( nodePhotonsList.size() == 0 )
	{
		exportFile.close();
		return 1;
	}

	for( int i = 0; i< nodePhotonsList.size(); ++i )
	{
		Photon* node = nodePhotonsList[i];
		Point3D photon = node->pos;
		double id = node->id;
		double prev_id = ( node->prev ) ? node->prev->id : 0;
		double next_id = ( node->next ) ? node->next->id : 0;
		out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
	}

	exportFile.close();

	return 1;
}

/**
 * Saves to \a filename file the \a selectedSurface photons in scene surface node local coordinates.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportSurfaceLocalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap )
{
	if( ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
		( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TLightKit::getClassTypeId() ) ) )
			return 0;

	QFile exportFile( fileName );
	if(!exportFile.open( QIODevice::WriteOnly ) )	return 0;

	QDataStream out( &exportFile );
	out<< wPhoton;

	QList< Photon* > nodePhotonsList = photonMap->GetSurfacePhotons( selectedSurface );
	if( nodePhotonsList.size() == 0 )
	{
		exportFile.close();
		return 1;
	}
	Transform worldToObject = selectedSurface->GetIntersectionTransform();
	for( int i = 0; i< nodePhotonsList.size(); ++i )
	{
		Photon* node = nodePhotonsList[i];
		Point3D photon =  worldToObject( node->pos );
		double id = node->id;
		double prev_id = ( node->prev ) ? node->prev->id : 0;
		double next_id = ( node->next ) ? node->next->id : 0;
		out<<id <<photon.x << photon.y <<photon.z<<prev_id <<next_id ;
	}

	exportFile.close();
	return 1;
}

SoSeparator* trf::DrawPhotonMapPoints( const TPhotonMap& map )
{
        SoSeparator* drawpoints=new SoSeparator;
        SoCoordinate3* points = new SoCoordinate3;

        QList< Photon* > photonsList = map.GetAllPhotons();

        for (int i = 0; i < photonsList.size(); ++i)
        {
                Point3D photon = photonsList[i]->pos;
                points->point.set1Value( i, photon.x, photon.y, photon.z );
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

SoSeparator* trf::DrawPhotonMapRays( const TPhotonMap& map, unsigned long numberOfRays, double fraction )
{
        SoSeparator* drawrays = new SoSeparator;
        SoCoordinate3* points = new SoCoordinate3;

        int drawRays =  (int) (numberOfRays * ( fraction / 100 ) );
        if( drawRays == 0 ) drawRays = 1;

        int* lines = new int[drawRays];

        unsigned long rayLength = 0;

        unsigned long numberOfPhoton = 0;

        QList< Photon* > photonsList = map.GetAllPhotons();

        int indexPhotonList = 0;
        for (int drawnRay = 0; drawnRay < drawRays; ++drawnRay)
        {
        	 while( ( indexPhotonList < photonsList.size() ) && ( photonsList[indexPhotonList]->prev != 0 ) )	indexPhotonList++;

        	 if ( photonsList[indexPhotonList]->prev == 0 )
        	 {
        		 Photon* node = photonsList[indexPhotonList];
        		 rayLength = 0;

        		 while ( node != 0 )
        		 {
        			 Point3D photon = node->pos;
        			 points->point.set1Value( numberOfPhoton, photon.x, photon.y, photon.z );

        			 if( node->next != 0 )   node = map.GetPhoton( node->next->id );
        			 else    node = 0;

        			 rayLength++;
        			 numberOfPhoton++;
				 }

        		 lines[drawnRay]= rayLength;
        		 indexPhotonList++;
        	 }

        }


        SoMaterial* myMaterial = new SoMaterial;
        myMaterial->diffuseColor.setValue(1.0, 1.0, 0.8);
        drawrays->addChild( myMaterial );
        drawrays->addChild( points );

        SoLineSet* lineset = new SoLineSet;
        lineset->numVertices.setValues( 0, drawRays, lines );
        drawrays->addChild( lineset );


        delete lines;
        return drawrays;
}
