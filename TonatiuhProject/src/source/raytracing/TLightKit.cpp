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

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
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
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(tsunshape, TSunShape, TDefaultSunShape, TRUE, transformGroup, "", TRUE);


	SO_NODE_ADD_FIELD( azimuth, (0.0) );
	SO_NODE_ADD_FIELD( zenith, (0.0) );
	SO_NODE_ADD_FIELD( distance, (200.0) );
	SO_NODE_ADD_FIELD( automaticallyResizable, ( TRUE ) );

	SO_KIT_INIT_INSTANCE();

	SoDirectionalLight* light = static_cast<SoDirectionalLight*>( getPart( "light", true ) );
	light->direction.setValue( SbVec3f( 0, -1, 0 ) );

	SoTransform* transform = new SoTransform;
	setPart( "transform", transform );

	SoMaterial* lightMaterial = static_cast<SoMaterial*>( getPart( "iconMaterial", true ) );
	lightMaterial->diffuseColor.setValue( 0.933, 0.91, 0.666);
	lightMaterial->transparency = 0.5;
	setPart( "iconMaterial", lightMaterial );
}

/**
 * Destructor.
 */
TLightKit::~TLightKit()
{

}

/*!
 * Computes the sun position local coordinates for the \a longitude and \a latitude in the \a newTime.
 * The light is positioned at these coordinates.
 * \a longitude  and \a latitude are in degrees.
 */
void TLightKit::ChangePosition( QDateTime newTime, double longitude, double latitude )
{
	m_time = newTime;
	m_longitude = longitude;
	m_latitude = latitude;

	int year = m_time.date().year();
	int month = m_time.date().month();
	int day = m_time.date().day();

	double hours = m_time.time().hour ();
	double minutes = m_time.time().minute ();
	double seconds = m_time.time().second ();
	cTime myTime = { year, month, day, hours, minutes, seconds };

	//Localization
	double lon = m_longitude.getValue();
	double lat = m_latitude.getValue();
    cLocation myLocation = {lon , lat };

	//Calculate sun position
	cSunCoordinates results;
	sunpos( myTime, myLocation, &results );

	azimuth.setValue( results.dAzimuth * tgc::Degree );
	zenith = results.dZenithAngle * tgc::Degree;
	UpdateSunPosition();

	//connect( buttonBox, SIGNAL( accepted() ), this, SLOT() );
}

/*!
 * Changes the light position to \a azimuth, \a zenith, and \a distance from the scene centre.
 * Azimuth and Zenith are in radians.
 * \sa redo().
 */
void TLightKit::ChangePosition( double newAzimuth, double newZenith, double newDistance )
{
	azimuth = newAzimuth;
	zenith = newZenith;
	distance = newDistance;
	UpdateSunPosition();

}

/*!
 * Returns local time at \a time, \a zenith, and the localization \a longitue and \a latitude.
 * \a longitude  and \a latitude are in degrees.
 *
 * \sa ChangePosition()
 */
void TLightKit::GetPositionData( QDateTime* time, double* longitude, double* latitude )
{
	*time = m_time;
	*longitude = m_longitude.getValue();
	*latitude = m_latitude.getValue();
}

/*!
 * Resizes the light to cover the \a box projection.
 */
void TLightKit::ResizeToBBox( BBox box )
{
	SoTransform* lightTransform = static_cast< SoTransform* >( this->getPart( "transform", true ) );
	Transform lTW = tgf::TransformFromSoTransform( lightTransform );
	Transform wTL = lTW.GetInverse();
	BBox localBox = wTL( box );

	Vector3D vMin( localBox.pMin.x, 0.0, localBox.pMin.z );
	Vector3D pMin = Normalize( vMin ) * ( vMin.length() + tan( 2.5 * tgc::Degree ) * distance.getValue() );

	Vector3D vMax( localBox.pMax.x, 0.0, localBox.pMax.z );
	Vector3D pMax = Normalize( vMax ) * ( vMax.length() + tan( 5 * tgc::Degree ) * distance.getValue() );

	TShape* shape = static_cast< TShape* >( this->getPart( "icon", true ) );
	BBox shapeBB = shape->GetBBox();

	Point3D sMin = shapeBB.pMin;
	Point3D sMax = shapeBB.pMax;

	double xScaleFactor  = std::max( fabs( pMin.x/sMin.x ), fabs( pMin.x/sMax.x ) );
	double zScaleFactor  = std::max( fabs( pMax.z/sMin.z ), fabs( pMax.z/sMax.z ) );


	lightTransform->scaleFactor.setValue( xScaleFactor, 1, zScaleFactor );

}

void TLightKit::Update()
{
	double oldAzimuth = azimuth.getValue();
	azimuth.setValue( 0 );
	azimuth.setValue( oldAzimuth );
	double oldZenith = zenith.getValue();
	zenith.setValue( 0 );
	zenith.setValue( oldZenith );

}

void TLightKit::UpdateSunPosition()
{
	Vector3D direction = Vector3D( sin( zenith.getValue() ) * sin( azimuth.getValue() ), cos( zenith.getValue() ), -sin( zenith.getValue() )*cos( azimuth.getValue() ) );

	Vector3D center = direction * distance.getValue();
	Transform translate = Translate( center.x, center.y, center.z );

	Transform rotX  = RotateX( -zenith.getValue() );
	Transform rotY = RotateY( -azimuth.getValue() );

	Transform transform = translate * rotY * rotX;

	SoTransform* lightTransform = static_cast< SoTransform* >( this->getPart( "transform", true ) );
	lightTransform->setMatrix( tgf::MatrixFromTransform( transform ) );
}
