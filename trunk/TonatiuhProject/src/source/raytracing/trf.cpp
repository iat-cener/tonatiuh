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


/**
 * Saves to \a filename file the \a photonMap all information.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportAll( QString fileName, double wPhoton, TPhotonMap* photonMap, bool saveCoordinate, bool saveSide )
{
        QFile exportFile( fileName );
        if(!exportFile.open( QIODevice::WriteOnly ) )   return 0;

        QDataStream out( &exportFile );
        out<< wPhoton;
        double id = 0;
        std::vector< std::vector< Photon > > photonsList = photonMap->GetAllPhotons();
        for( unsigned int i = 0; i < photonsList.size(); ++i)
        {
        	if(saveCoordinate && saveSide){

        		for(int j=0;j<photonsList.at(i).size();j++){
        			Photon* node = &photonsList[i].at(j);
        			Point3D photon = node->pos;
        			double side = double( node->side );

        			//double prev_id = ( node->prev ) ? node->prev->id : 0;
        			//double next_id = ( node->next ) ? node->next->id : 0;
        			out<<++id <<photon.x << photon.y << photon.z <<side;

        			}
        	}else if (saveCoordinate && !saveSide) {
        		for(int j=0;j<photonsList.at(i).size();j++){
        		     Photon* node = &photonsList[i].at(j);
        		     Point3D photon = node->pos;

        		     out<<++id <<photon.x << photon.y << photon.z;

        		        	 }
        	}else if(!saveCoordinate && saveSide){
        		for(int j=0;j<photonsList.at(i).size();j++){
        			Photon* node = &photonsList[i].at(j);
        		    double side = double( node->side );
        		     out<<++id<<side;

        		        }
        	}else{
        		for(int j=0;j<photonsList.at(i).size();j++){

        		     out<<++id;

        		        		        	 }
        	}
        }
        exportFile.close();

        return 1;
}

/**
 * Saves to \a filename file the \a selectedSurface photons in scene global coordinates.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportSurfaceGlobalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap,bool saveCoordinate, bool saveSide )
{
        if( ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
                ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TLightKit::getClassTypeId() ) ) )
                return 0;

        QFile exportFile( fileName );
        if(!exportFile.open( QIODevice::WriteOnly ) )   return 0;

        QDataStream out( &exportFile );
        out<< wPhoton;

        std::vector< std::vector< Photon > > nodePhotonsList = photonMap->GetSurfacePhotons( selectedSurface );
        if( nodePhotonsList.size() == 0 )
        {
                exportFile.close();
                return 1;
        }
        double id=0;
        for( unsigned int i = 0; i< nodePhotonsList.size(); ++i )
        {
        	if(saveCoordinate && saveSide){
        	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
        	        			Photon* node = &nodePhotonsList[i].at(j);
        	        			Point3D photon = node->pos;
        	        			double side = double( node->side );

        	        			out<<++id <<photon.x << photon.y << photon.z <<side;

        	        			}
        	        	}else if (saveCoordinate && !saveSide) {
        	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
        	        		     Photon* node = &nodePhotonsList[i].at(j);
        	        		     Point3D photon = node->pos;

        	        		     out<<++id <<photon.x << photon.y << photon.z;

        	        		        	 }
        	        	}else if(!saveCoordinate && saveSide){
        	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
        	        			Photon* node = &nodePhotonsList[i].at(j);
        	        		    double side = double( node->side );

        	        		     out<<++id<<side;

        	        		        }
        	        	}else{
        	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){

        	        		     out<<++id;

        	        		        		        	 }
        	        	}

        }

        exportFile.close();

        return 1;
}

/**
 * Saves to \a filename file the \a selectedSurface photons in scene surface node local coordinates.
 *
 * The file first data is the \a wPhoton. The power per photon in W/m2.
 */
int trf::ExportSurfaceLocalCoordinates( QString fileName, InstanceNode* selectedSurface, double wPhoton, TPhotonMap* photonMap, bool saveCoordinate, bool saveSide )
{
	if( !selectedSurface )	return 0;


    if( ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TShapeKit::getClassTypeId() ) ) &&
                ( !selectedSurface->GetNode()->getTypeId().isDerivedFrom( TLightKit::getClassTypeId() ) ) )
                        return 0;

    Transform worldToObject = selectedSurface->GetIntersectionTransform();

	QFile exportFile( fileName );
	if(!exportFile.open( QIODevice::WriteOnly ) )   return 0;

	QDataStream out( &exportFile );
    out<< wPhoton;
    std::vector< std::vector< Photon > > nodePhotonsList = photonMap->GetSurfacePhotons( selectedSurface );

    if( nodePhotonsList.size() == 0 )
    {
               exportFile.close();
               return 1;
    }
    double id=0;
    for( unsigned int i = 0; i< nodePhotonsList.size(); ++i )
        {
    	if(saveCoordinate && saveSide){
    	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
    	        			Photon* node = &nodePhotonsList[i].at(j);
    	        			Point3D photon = node->pos;
    	        			double side = double( node->side );

    	        			//double prev_id = ( node->prev ) ? node->prev->id : 0;
    	        			//double next_id = ( node->next ) ? node->next->id : 0;
    	        			out<<++id <<photon.x << photon.y << photon.z <<side;

    	        			}
    	        	}else if (saveCoordinate && !saveSide) {
    	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
    	        		     Photon* node = &nodePhotonsList[i].at(j);
    	        		     Point3D photon = node->pos;

    	        		     out<<++id <<photon.x << photon.y << photon.z;

    	        		        	 }
    	        	}else if(!saveCoordinate && saveSide){
    	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){
    	        			Photon* node = &nodePhotonsList[i].at(j);
    	        		    double side = double( node->side );
    	        		     out<<++id<<side;

    	        		        }
    	        	}else{
    	        		for(int j=0;j<nodePhotonsList.at(i).size();j++){

    	        		     out<<++id;

    	             }
    	        	}

    	}

        exportFile.close();
        return 1;
}

SoSeparator* trf::DrawPhotonMapPoints( const TPhotonMap& map )
{
	SoSeparator* drawpoints=new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;
	std::vector< std::vector< Photon > > photonsList = map.GetAllPhotons();
    unsigned int numRays=0;

	for( unsigned int i = 0; i < photonsList.size(); i++)
	{
		//std::cout <<"Fila.."<< i << std::endl;
		for(int j=0;j<photonsList[i].size();j++){
			//std::cout <<"Fila.."<< i << "hay.."<< photonsList[i].size()<<std::endl;
			Point3D photon = photonsList[i].at(j).pos;
			//std::cout <<photon.x << ":::" << photonsList.size() << std::endl;
			points->point.set1Value( numRays, photon.x, photon.y, photon.z );
			numRays++;
		}
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

SoSeparator* trf::DrawPhotonMapRays( const TPhotonMap& map, unsigned long numberOfRays )
{
	SoSeparator* drawrays = new SoSeparator;
	SoCoordinate3* points = new SoCoordinate3;

	//int drawRays =  (int) (numberOfRays * ( fraction / 100 ) );
	//int intervalRays = (int) ( numberOfRays / drawRays );
	//if( drawRays == 0 ) drawRays = 1;

	QVector< int >	rayLengths;

	//unsigned long rayLength = 0;
	unsigned long numberOfPhoton = 0;

	std::vector< std::vector< Photon > > allRaysLists = map.GetAllPhotons();


	int nRay = 0;
	for( unsigned int i = 0; i< allRaysLists.size(); i++ )
	{
		std::vector< Photon > raysList = allRaysLists[i];
		unsigned int indexPhotonList = 0;

		while( indexPhotonList < raysList.size() )
		{

			unsigned long rayLength = 0;
			do
			{
				Photon photon = raysList[indexPhotonList];
				Point3D photonPosistion = photon.pos;
				points->point.set1Value( numberOfPhoton, photonPosistion.x, photonPosistion.y, photonPosistion.z );
				indexPhotonList++;
				rayLength++;
				numberOfPhoton++;
			}while( indexPhotonList < raysList.size() && raysList[indexPhotonList].id > 0 );

/*
			//unsigned long rayLength = 0;
			while( indexPhotonList < raysList[i].size()-1 )
			{
				rayLength = 0;
				while ( ( indexPhotonList < raysLists[i].size()-1) && photonsList[i][indexPhotonList+1].id>0 )
				{

					 Photon* node = &photonsList[i].at(indexPhotonList);
					 Point3D photon = node->pos;

					 points->point.set1Value( numberOfPhoton, photon.x, photon.y, photon.z );
					 //std::cout <<"NPhoton.."<< numberOfPhoton << "photonx.."<< photon.x << std::endl;
					 //if( node->next != 0 )   node =node->next;
					 //else    node = 0;

					 indexPhotonList++;
					 rayLength++;
					 numberOfPhoton++;
				 }
				 Photon* node = &photonsList[i].at(indexPhotonList);
				 Point3D photon = node->pos;
				 //std::cout<<node->id<<std::endl;
				 points->point.set1Value( numberOfPhoton, photon.x, photon.y, photon.z );
				 indexPhotonList++;
				 rayLength++;
				 numberOfPhoton++;
				 //std::cout<< photon.x << ":::" <<rayLength<<":::"<< numberOfPhoton <<std::endl;
				 rayLengths.push_back( rayLength);

		 //}
		 //else
			// indexPhotonList=0;
         //std::cout<<numberOfPhoton<<std::endl;
*/
				rayLengths.push_back( rayLength );
				nRay++;
		}

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
