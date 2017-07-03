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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QVariant>

#include "ParallelRandomDeviate.h"
#include "Ray.h"
#include "RayCasting.h"
#include "TGroupNode.h"
#include "TNodesList.h"
#include "TShape.h"
#include "TSunshape.h"

/*!
 * Creates ray tracer object.
 */
RayCasting::RayCasting()
:RayTracer( ),
 m_sceneRootNode( 0 )
{

}

/*!
 * Destroys ray tracer object.
 */
RayCasting::~RayCasting()
{
	if( m_sceneRootNode || m_sceneRootNode != 0 )
		RemoveRayTracerNodesTree( m_sceneRootNode );
}

/*!
 * Set the scene to simulate for the ray tracer.
 * Returns true if the scene is valid.
 */
bool RayCasting::SetScene( TSceneNode* scene, QStringList notFirstStageNodesURL )
{
	if( !scene )	return ( false );
	m_pSunNode = scene->GetPart( "light" )->as<TSunNode>();

	TGroupNode* childrenRoot = scene->GetPart( "childrenList" )->as<TGroupNode>();
	if( !childrenRoot )	return ( false );


	m_sceneRootNode = new RayCastingNode;
	Transform parentWT0(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1 );

	if( !CreateRayTracerNodesTree( childrenRoot, m_sceneRootNode, parentWT0, QLatin1String( "\\\\SunNode" ) ) )
	{
		RemoveRayTracerNodesTree( m_sceneRootNode );
		return (false);
	}


	double azimuthRad = m_pSunNode->GetAzimuth();
	double zenithRad =  m_pSunNode->GetZenith();

	double distMax =  Distance( m_sceneRootNode->boundingBox.pMin, m_sceneRootNode->boundingBox.pMax ) +10;
	Vector3D sunVector ( sin(zenithRad) * sin(azimuthRad), cos(zenithRad), -sin(zenithRad) * cos(azimuthRad) );

	Vector3D auxiliaryAxis( 1, 0, 0 );
	Vector3D uVector = Normalize( CrossProduct( auxiliaryAxis, sunVector ) );
	Vector3D vVector = Normalize( CrossProduct( sunVector, uVector ));
	m_sunTransformSTW = Transform( uVector.x, sunVector.x, vVector.x, 0,
													uVector.y, sunVector.y, vVector.y, 0,
													uVector.z, sunVector.z, vVector.z, 0,
													0, 0, 0, 1 ) *
	    		Transform( 1, 0, 0, 0,
	    				0, 1, 0, distMax,
						0, 0, 1, 0,
						0, 0, 0, 1 );

	Transform sunTransformWTS = m_sunTransformSTW.GetInverse();
	Point3D pMinBBoxSunCoord = sunTransformWTS( m_sceneRootNode->boundingBox).pMin;
	Point3D pMaxBBoxSunCoord = sunTransformWTS( m_sceneRootNode->boundingBox).pMax;

	double xMin = pMinBBoxSunCoord.x;
	double zMin = pMinBBoxSunCoord.z;
	double xMax = pMaxBBoxSunCoord.x;
	double zMax = pMaxBBoxSunCoord.z;

	if( -gc::Infinity  == m_sceneRootNode->boundingBox.Volume() )
	{
		xMin = 0.0;
		xMax = 0.0;
		zMin = 0.0;
		zMax = 0.0;
		distMax = 0.0;
	}

	TSunshape* sunshape = m_pSunNode->GetPart( QLatin1String( "sunshape" ) )->as<TSunshape>();
	if( !sunshape )	return ( false );

	double thetaMax = sunshape->GetThetaMax();
	double delta = 0.01;
	if(  thetaMax > 0.0 ) delta = distMax * tan( thetaMax );

	m_lightOriginShape.xMinSunPlane = ( xMin - delta );
	m_lightOriginShape.xMaxSunPlane = ( xMax + delta );
	m_lightOriginShape.zMinSunPlane = ( zMin - delta );
	m_lightOriginShape.zMaxSunPlane = ( zMax + delta );
	double width = m_lightOriginShape.xMaxSunPlane - m_lightOriginShape.xMinSunPlane;
	double height = m_lightOriginShape.zMaxSunPlane - m_lightOriginShape.zMinSunPlane;

	int widthDivisions = 200;
	int heigthDivisions = 200;

	while( ( width / widthDivisions ) < delta)	widthDivisions--;
	m_lightOriginShape.widthElements = widthDivisions;
	m_lightOriginShape.cellWidth = double( width / widthDivisions );

	while( ( height / heigthDivisions ) < delta )	heigthDivisions--;
	m_lightOriginShape.heightElements = heigthDivisions;
	m_lightOriginShape.cellHeight = height / heigthDivisions;

	int** areaMatrix = new int*[heigthDivisions];
	for( int i = 0; i < heigthDivisions; i++ )
	{
		areaMatrix[i] = new int[widthDivisions];
		for( int j = 0; j < widthDivisions; j++ )
			areaMatrix[i][j] = 0;
	}

	for( unsigned int s = 0; s < m_surfacesNodeList.size(); s++ )
	{
		if( notFirstStageNodesURL.count() < 1 || !notFirstStageNodesURL.contains(  m_surfacesNodeList[s]->nodeURL ) )
		{

			BBox shapeBB = m_surfacesNodeList[s]->boundingBox;

			std::vector< Point3D > surfacePolygonList;
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMin.x, shapeBB.pMin.y, shapeBB.pMin.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMax.x, shapeBB.pMin.y, shapeBB.pMin.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMax.x, shapeBB.pMin.y, shapeBB.pMax.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMin.x, shapeBB.pMin.y, shapeBB.pMax.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMin.x, shapeBB.pMax.y, shapeBB.pMin.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMax.x, shapeBB.pMax.y, shapeBB.pMin.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMax.x, shapeBB.pMax.y, shapeBB.pMax.z ) ) );
			surfacePolygonList.push_back( sunTransformWTS( Point3D( shapeBB.pMin.x, shapeBB.pMax.y, shapeBB.pMax.z ) ) );

			int xMinIndex = widthDivisions;
			int xMaxIndex = 0;
			int zMinIndex = heigthDivisions;
			int zMaxIndex = 0;
			for( unsigned int p = 0; p < surfacePolygonList.size(); p++ )
			{
				int qPX = ( surfacePolygonList[p].x - m_lightOriginShape.xMinSunPlane ) / m_lightOriginShape.cellWidth;
				int qPZ = ( surfacePolygonList[p].z - m_lightOriginShape.zMinSunPlane ) / m_lightOriginShape.cellHeight;


				if( qPX < xMinIndex  )	xMinIndex = qPX;
				if( qPX > xMaxIndex  )	xMaxIndex = qPX;

				if( qPZ < zMinIndex  )	zMinIndex = qPZ;
				if( qPZ > zMaxIndex  )	zMaxIndex = qPZ;
			}

			for( int i = zMinIndex; i <=zMaxIndex; i++ )
				for( int j = xMinIndex; j<=xMaxIndex; j++ )
					areaMatrix[i][j] = 1;

		}

	}

	for( int i = 0; i < heigthDivisions; i++ )
	{
		for( int j = 0; j < widthDivisions; j++ )
		{

			double pixelIntensity = areaMatrix[i][j];
			if( ( i - 1 ) >= 0 && ( j - 1 ) >= 0 ) pixelIntensity += areaMatrix[i-1][j-1];
			if( ( j - 1 ) >= 0 ) pixelIntensity += areaMatrix[i][j-1];
			if( ( i + 1 ) < heigthDivisions && ( j - 1) >= 0 )  pixelIntensity += areaMatrix[i+1][j-1];
			if( ( i - 1 ) >= 0 )	pixelIntensity +=  areaMatrix[i-1][j];
			if( ( i + 1 ) < heigthDivisions )  pixelIntensity +=  areaMatrix[i+1][j];
			if( ( i - 1 ) >= 0 && ( j + 1 ) < widthDivisions ) pixelIntensity +=  areaMatrix[i-1][j+1];
			if( ( j + 1 ) < widthDivisions ) pixelIntensity +=  areaMatrix[i][j+1];
			if( ( i + 1 ) < heigthDivisions && ( j + 1 ) < widthDivisions ) pixelIntensity +=  areaMatrix[i+1][j+1];

			if( pixelIntensity > 0.0 )
				m_lightOriginShape.validSunAreasVector.push_back( std::pair<int,int>(i,j) );
		}
	}

	for( int i = 0; i < heigthDivisions; i++ )
		delete[] areaMatrix[i];
	delete[] areaMatrix;

	return ( true );
}

/*!
 * Executes the ray tracer.
 */
void RayCasting::RunRaytracer( unsigned long numberOfRays)
{
	std::vector< Photon > photonsVector;

	ParallelRandomDeviate rand( m_pRandomNumberGenerator, &m_randomMutex );

	for(  unsigned long  i = 0; i < numberOfRays; ++i )
	{
		Ray ray;
		if( NewRay( &ray, rand ) )
		{
			photonsVector.push_back( Photon( ray.origin, 1, 0, QLatin1String( "\\\\SunNode" ) ) );
			int rayLength = 0;

			QString intersectedSurface;
			bool isFront = false;

			//Trace the ray
			bool isReflectedRay = true;
			while( isReflectedRay )
			{
				//intersectedSurface = 0;
				intersectedSurface = QLatin1String( "" );
				isFront = 0;
				Ray reflectedRay;
				isReflectedRay = m_sceneRootNode->Intersect( ray, rand, &isFront, &intersectedSurface, &reflectedRay );

				if( isReflectedRay )
				{
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1) );

					//Prepare node and ray for next iteration
					ray = reflectedRay;
				}

			}

			if( !(rayLength == 0 && ray.maxt == HUGE_VAL) )
			{
				if( ray.maxt == HUGE_VAL  )
				{
					ray.maxt = 0.1;
					photonsVector.push_back( Photon( (ray)( ray.maxt ), 0, ++rayLength, intersectedSurface) );
				}
				else
					photonsVector.push_back( Photon( (ray)( ray.maxt ), isFront, ++rayLength, intersectedSurface, 1 ) );
			}

		}

	}

	photonsVector.resize( photonsVector.size() );


	m_mutexPhotonMap.lock();
	m_pPhotonMap->StoreRays( photonsVector );
	m_mutexPhotonMap.unlock();

}

/*!
 * Creates nodes tree. The bounding box of each node instance and the transformation to pass from world to object local coordinates are calculated.
 */
bool RayCasting::CreateRayTracerNodesTree( TContainerNode* node, RayCastingNode* rTRNode, Transform parentWT0, QString parentURL )
{
	if( !node || !rTRNode )	return ( false );

	rTRNode->nodeURL = QString( "%1\\%2" ).arg( parentURL, node->GetName() );

	TGroupNode* nodeGroup = node->as<TGroupNode>();
	if( nodeGroup )
	{
		Transform transformOTW = nodeGroup->GetTrasformation();
		if( !transformOTW.GetMatrix() ) 	return ( false );
		rTRNode->wtoTransformation = (transformOTW.GetInverse() * parentWT0 );
		rTRNode->otwTransformation = rTRNode->wtoTransformation.GetInverse();

		TNodesList* childrenListNode = nodeGroup->GetPart( "childrenList" )->as<TNodesList>();
		if( !childrenListNode )	return ( false );
		int numberOfChildren = childrenListNode->Count();
		if( numberOfChildren == 1  )
		{
			TNode* childNode = childrenListNode->Item( 0 );
			TGroupNode* childNodeGroup = childNode->as<TGroupNode>();

			//if the parent node only has one child and this child is a group node, the transformations can combined.
			if( childNodeGroup )
			{
				rTRNode->nodeURL = QString( "%1\\%2" ).arg( rTRNode->nodeURL, childNode->GetName() );

				//Transform childTransformOTW;
				Transform childTransformOTW = childNodeGroup->GetTrasformation();
				if( !childTransformOTW.GetMatrix() ) 	return ( false );


				rTRNode->wtoTransformation = (childTransformOTW.GetInverse() * rTRNode->wtoTransformation );


			childrenListNode = childNodeGroup->GetPart( "childrenList" )->as<TNodesList>();
			numberOfChildren = childrenListNode->Count();


			}
		}

		BBox nodeBB;
		for( int l = 0; l < numberOfChildren; l++ )
		{

			TContainerNode* childContainerNode = childrenListNode->Item( l )->as<TContainerNode>();
			if( !childContainerNode )	return ( false );
			RayCastingNode* childRTNode = new RayCastingNode;
			rTRNode->childrenList.push_back( childRTNode );
			if( !CreateRayTracerNodesTree( childContainerNode, childRTNode, rTRNode->wtoTransformation, rTRNode->nodeURL ) )	return ( false );

			nodeBB = Union( nodeBB, childRTNode->boundingBox );

		}
		rTRNode->boundingBox = nodeBB;

	}

	else
	{
		m_surfacesNodeList.push_back(rTRNode);
		rTRNode->wtoTransformation = parentWT0;
		rTRNode->otwTransformation = parentWT0.GetInverse();

		TSurfaceNode* surfaceNode = node->as<TSurfaceNode>();
		if( !surfaceNode )	return ( false );
		rTRNode->surfaceNode = ( surfaceNode );

		TShape* shape = surfaceNode->GetPart( QLatin1String( "shape" ) )->as<TShape>();
		rTRNode->boundingBox = rTRNode->otwTransformation( shape->GetBondingBox() );
	}

	return ( true );
}

/*!
 * Removes tray tracer nodes.
 */
void RayCasting::RemoveRayTracerNodesTree( RayCastingNode* rTParentNode )
{
	if( rTParentNode )
	{
		int numberOfChildren = rTParentNode->childrenList.size();
		for( int c = 0; c < numberOfChildren; c++  )
		{
			RemoveRayTracerNodesTree( rTParentNode->childrenList[c] );
		}
		rTParentNode->childrenList.clear();
		delete rTParentNode;
		rTParentNode = 0;
	}

}
