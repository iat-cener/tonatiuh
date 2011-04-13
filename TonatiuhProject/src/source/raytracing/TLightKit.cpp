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

#include <QImage>
#include <QBrush>
#include <QColor>
#include <QImage>
#include <QPaintEngine>

#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#include "BBox.h"
#include "Matrix4x4.h"
#include "Point3D.h"
#include "sunpos.h"
#include "TDefaultSunShape.h"
#include "tgc.h"
#include "tgf.h"
#include "TLightKit.h"
#include "TLightShape.h"
#include "Transform.h"
#include "TShapeKit.h"
#include "TSquare.h"

SO_KIT_SOURCE(TLightKit);

/**
 * Initializates TLightKit componets
 */
void TLightKit::initClass()
{
	SO_KIT_INIT_CLASS(TLightKit, SoLightKit, "LightKit");
}

/**
 * Creates a new TLightKit.
 */
TLightKit::TLightKit()
{
	SO_KIT_CONSTRUCTOR(TLightKit);

	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconMaterial, SoNode, SoMaterial, TRUE, iconSeparator, icon, TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(iconTexture, SoNode, SoTexture2, TRUE, iconSeparator, iconMaterial, TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, TSunShape, TDefaultSunShape, TRUE, transformGroup, "", TRUE);

	SO_NODE_ADD_FIELD( azimuth, (0.0) );
	SO_NODE_ADD_FIELD( zenith, (0.0) );


	SO_KIT_INIT_INSTANCE();

	SoDirectionalLight* light = static_cast<SoDirectionalLight*>( getPart( "light", true ) );
	light->direction.setValue( SbVec3f( 0, -1, 0 ) );

	SoTransform* transform = new SoTransform;
	setPart( "transform", transform );

	SoMaterial* lightMaterial = static_cast<SoMaterial*>( getPart( "iconMaterial", true ) );
	//lightMaterial->diffuseColor.setValue( 0.933, 0.91, 0.666);
	lightMaterial->transparency = 0.5;
	setPart( "iconMaterial", lightMaterial );

	int widthPixeles = 100;
	int heightPixeles = 100;


	unsigned char bitmap[ widthPixeles * heightPixeles ];
	//julia(  widthPixeles, heightPixeles, bitmap, 128 );


	for( int y = 0; y < heightPixeles; y++ )
	{
		for( int x = 0; x < widthPixeles; x++ )
		{
			bitmap[ y* widthPixeles +x] = 255;
		}

	}


	SoTexture2 * texture = new SoTexture2;
    texture->image.setValue( SbVec2s( heightPixeles, widthPixeles ), 1, bitmap );
    texture->model = SoTexture2::BLEND;
    texture->blendColor.setValue( 0.933, 0.91, 0.666 );
	setPart( "iconTexture", texture );

	TLightShape* iconShape = new TLightShape;;
	setPart( "icon", iconShape );
}

/**
 * Destructor.
 */
TLightKit::~TLightKit()
{
	//void ChangePosition( QDateTime time, double longitude, double latitude );
	//void SetDateTime( QDateTime time );

}

/*!
 * Changes the light position to \a azimuth, \a zenith, and \a distance from the scene centre.
 * Azimuth and Zenith are in radians.
 * \sa redo().
 */
void TLightKit::ChangePosition( double newAzimuth, double newZenith/*, double newDistance*/ )
{
	azimuth = newAzimuth;
	zenith = newZenith;
	/*distance = newDistance;*/
	//UpdateSunPosition();

}

void TLightKit::Update( BBox box )
{
	double xMin = box.pMin.x;
	double xMax = box.pMax.x;
	double zMin = box.pMin.z;
	double zMax = box.pMax.z;
	double yMin = box.pMin.y;
	double yMax = box.pMax.y + 10;
	double distMax = yMax - yMin;

	if( -tgc::Infinity  == box.Volume() )
	{
		xMin = 0.0;
		xMax = 0.0;
		zMin = 0.0;
		zMax = 0.0;
		yMax = 10;
		distMax = 0.0;
	}

	TSunShape* sunshape = static_cast< TSunShape* >( this->getPart( "tsunshape", false ) );
	if( !sunshape )	return;
	double thetaMax = sunshape->GetThetaMax();
	double delta = 0.01;
	if(  thetaMax > 0.0 ) delta = 5 * distMax * tan( thetaMax );

	TLightShape* shape = static_cast< TLightShape* >( this->getPart( "icon", true ) );
	if( !shape )	return;
	shape->xMin.setValue( xMin - delta );
	shape->xMax.setValue( xMax + delta );
	shape->zMin.setValue( zMin - delta );
	shape->zMax.setValue( zMax + delta );
	shape->delta.setValue( delta );


	SoTransform* lightTransform = static_cast< SoTransform* >( this->getPart( "transform", true ) );
	SbVec3f translation( 0.0, yMax, 0.0 );
	lightTransform->translation.setValue(translation );


	/*double oldAzimuth = azimuth.getValue();
	azimuth.setValue( 0 );
	azimuth.setValue( oldAzimuth );
	double oldZenith = zenith.getValue();
	zenith.setValue( 0 );
	zenith.setValue( oldZenith );*/

}

void TLightKit::ComputeLightSourceArea( QVector< QPair< TShapeKit*, Transform > > surfacesList )
{
	TLightShape* shape = static_cast< TLightShape* >( this->getPart( "icon", true ) );
	if( !shape )	return;

	double width =  shape->xMax.getValue() - shape->xMin.getValue();
	double height = shape->zMax.getValue() - shape->zMin.getValue();


	int pixels = 200;
	int widthPixeles = pixels;
	if( ( width / pixels ) < shape->delta.getValue() ) widthPixeles = ceil( width / shape->delta.getValue() );
	double pixelWidth = double( width / widthPixeles );

	int heightPixeles = pixels;
	if( ( height / pixels ) < shape->delta.getValue() ) heightPixeles = ceil( height / shape->delta.getValue() );
	double pixelHeight = height / heightPixeles;

	QImage  sourceImage( widthPixeles, heightPixeles, QImage::Format_RGB32 );
	sourceImage.setOffset( QPoint( 0.5, 0.5 ) );
	sourceImage.fill( Qt::white  );

	QPainter painter( &sourceImage );

	QBrush brush( Qt::black );
	painter.setBrush( brush );

	QPen pen( Qt::black, Qt::NoPen );
	painter.setPen( pen );

	for( int s = 0; s < surfacesList.size(); s++ )
	{
		TShapeKit* surfaceKit = surfacesList[s].first;
		Transform surfaceTransform = surfacesList[s].second;
		Transform shapeToWorld = surfaceTransform.GetInverse();

		TShape* shapeNode = dynamic_cast< TShape* > ( surfaceKit->getPart( "shape", false ) );
		if( shapeNode )
		{
			BBox shapeBB = shapeNode->GetBBox();

			Point3D p1( shapeBB.pMin.x, shapeBB.pMin.y, shapeBB.pMin.z );
			Point3D p2( shapeBB.pMax.x, shapeBB.pMin.y, shapeBB.pMin.z );
			Point3D p3( shapeBB.pMax.x, shapeBB.pMin.y, shapeBB.pMax.z );
			Point3D p4( shapeBB.pMin.x, shapeBB.pMin.y, shapeBB.pMax.z );
			Point3D p5( shapeBB.pMin.x, shapeBB.pMax.y, shapeBB.pMin.z );
			Point3D p6( shapeBB.pMax.x, shapeBB.pMax.y, shapeBB.pMin.z );
			Point3D p7( shapeBB.pMax.x, shapeBB.pMax.y, shapeBB.pMax.z );
			Point3D p8( shapeBB.pMin.x, shapeBB.pMax.y, shapeBB.pMax.z );


			Point3D tP1 = shapeToWorld( p1 );
			Point3D tP2 = shapeToWorld( p2 );
			Point3D tP3 = shapeToWorld( p3 );
			Point3D tP4 = shapeToWorld( p4 );
			Point3D tP5 = shapeToWorld( p5 );
			Point3D tP6 = shapeToWorld( p6 );
			Point3D tP7 = shapeToWorld( p7 );
			Point3D tP8 = shapeToWorld( p8 );

			QPointF  qP1( ( tP1.x - shape->xMin.getValue() ) /pixelWidth, ( tP1.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP2( ( tP2.x - shape->xMin.getValue() ) /pixelWidth, ( tP2.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP3( ( tP3.x - shape->xMin.getValue() ) /pixelWidth, ( tP3.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP4( ( tP4.x - shape->xMin.getValue() ) /pixelWidth, ( tP4.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP5( ( tP5.x - shape->xMin.getValue() ) /pixelWidth, ( tP5.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP6( ( tP6.x - shape->xMin.getValue() ) /pixelWidth, ( tP6.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP7( ( tP7.x - shape->xMin.getValue() ) /pixelWidth, ( tP7.z - shape->zMin.getValue() ) /pixelHeight );
			QPointF qP8( ( tP8.x - shape->xMin.getValue() ) /pixelWidth, ( tP8.z - shape->zMin.getValue() ) /pixelHeight );

			QPointF polygon1[4] = { qP1, qP2, qP3, qP4 };
			QPointF polygon2[4] = { qP1, qP2, qP5, qP6 };
			QPointF polygon3[4] = { qP1, qP4, qP5, qP8 };
			QPointF polygon4[4] = { qP2, qP3, qP6, qP7 };
			QPointF polygon5[4] = { qP3, qP4, qP7, qP8 };
			QPointF polygon6[4] = { qP5, qP6, qP7, qP8 };
			painter.drawPolygon( polygon1, 4 );
			painter.drawPolygon( polygon2, 4 );
			painter.drawPolygon( polygon3, 4 );
			painter.drawPolygon( polygon4, 4 );
			painter.drawPolygon( polygon5, 4 );
			painter.drawPolygon( polygon6, 4 );
		}

	}

	int** areaMatrix = new int*[heightPixeles];
	for( int i = 0; i < heightPixeles; i++ )
	{
		areaMatrix[i] = new int[widthPixeles];
	}

	unsigned char bitmap[ widthPixeles * heightPixeles ];

	QRgb black = qRgb( 0, 0, 0);

	for( int i = 0; i < widthPixeles; i++ )
	{
		for( int j = 0; j < heightPixeles; j++ )
		{
			double pixelIntensity = ( sourceImage.pixel( i, j ) == black ) ? 1: 0;
			if( ( i - 1 ) >= 0 && ( j - 1 ) >= 0 ) pixelIntensity += ( sourceImage.pixel( i - 1, j - 1 ) == black ) ? 1: 0;
			if( ( j - 1 ) >= 0 ) pixelIntensity += ( sourceImage.pixel( i, j - 1 ) == black ) ? 1: 0;
			if( ( i + 1 ) < widthPixeles && ( j - 1) >= 0 )  pixelIntensity += ( sourceImage.pixel( i + 1, j - 1 ) == black ) ? 1: 0;
			if( ( i - 1 ) >= 0 )	pixelIntensity += ( sourceImage.pixel( i - 1, j ) == black ) ? 1: 0;
			if( ( i + 1 ) < widthPixeles )  pixelIntensity += ( sourceImage.pixel( i + 1, j ) == black ) ? 1: 0;
			if( ( i - 1 ) >= 0 && ( j + 1 ) < heightPixeles ) pixelIntensity += ( sourceImage.pixel( i - 1, j + 1 ) == black ) ? 1: 0;
			if( ( j + 1 ) < heightPixeles ) pixelIntensity += ( sourceImage.pixel( i, j + 1 ) == black ) ? 1: 0;
			if( ( i + 1 ) < widthPixeles && ( j + 1 ) < heightPixeles ) pixelIntensity += ( sourceImage.pixel( i + 1, j + 1 ) == black ) ? 1: 0;

			if( pixelIntensity > 0.0 )
			{
				areaMatrix[j][i] = 1;
				bitmap[ i * heightPixeles +  j ] = 0;
			}
			else
			{
				areaMatrix[j][i] = 0;
				bitmap[ i * heightPixeles +  j ] = 255;
			}

		}
	}

	SoTexture2* texture = static_cast< SoTexture2* >( getPart( "iconTexture", true ) );
    texture->image.setValue( SbVec2s( heightPixeles, widthPixeles ), 1, bitmap );
    texture->wrapS = SoTexture2::CLAMP;
    texture->wrapT = SoTexture2::CLAMP;

    shape->SetLightSourceArea( heightPixeles, widthPixeles, areaMatrix );

}


