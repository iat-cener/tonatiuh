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

#include <QString>

#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialAngularDependingSpecular.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"
#include "Vector3D.h"


SO_NODE_SOURCE(MaterialAngularDependingSpecular);

void MaterialAngularDependingSpecular::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialAngularDependingSpecular, TMaterial, "Material" );
}

MaterialAngularDependingSpecular::MaterialAngularDependingSpecular()
{
	SO_NODE_CONSTRUCTOR( MaterialAngularDependingSpecular );
	SO_NODE_ADD_FIELD( reflectivity, (0.0) );
	SO_NODE_ADD_FIELD( reflectivityCoef, (0.0902380168243675) );
	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	SO_NODE_ADD_FIELD( sigmaSpecularity, (0.0) );

	SO_NODE_DEFINE_ENUM_VALUE( Distribution, PILLBOX );
  	SO_NODE_DEFINE_ENUM_VALUE( Distribution, NORMAL );
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution) ;
	SO_NODE_ADD_FIELD( distribution, (PILLBOX) );

	SO_NODE_ADD_FIELD( mAmbientColor, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( mDiffuseColor, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( mSpecularColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( mEmissiveColor, (0.0, 0.0, 0.0) );
	SO_NODE_ADD_FIELD( mShininess, (0.2f) );
	SO_NODE_ADD_FIELD( mTransparency, (0.0f) );

	SoFieldSensor* m_reflectivitySensor = new SoFieldSensor( updateReflectivity, this );
	m_reflectivitySensor->setPriority( 1 );
	m_reflectivitySensor->attach( &reflectivity );

	SoFieldSensor* m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->setPriority( 1 );
	m_ambientColorSensor->attach( &mAmbientColor );
	SoFieldSensor* m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->setPriority( 1 );
	m_diffuseColorSensor->attach( &mDiffuseColor );
	SoFieldSensor* m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->setPriority( 1 );
	m_specularColorSensor->attach( &mSpecularColor );
	SoFieldSensor* m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->setPriority( 1 );
	m_emissiveColorSensor->attach( &mEmissiveColor );
	SoFieldSensor* m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->setPriority( 1 );
	m_shininessSensor->attach( &mShininess );
	SoFieldSensor* m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->setPriority( 1 );
	m_transparencySensor->attach( &mTransparency );
}

MaterialAngularDependingSpecular::~MaterialAngularDependingSpecular()
{
}

QString MaterialAngularDependingSpecular::getIcon()
{
	return QString(":icons/MaterialAngularDependingSpecular.png");
}

void MaterialAngularDependingSpecular::updateReflectivity( void* data, SoSensor* )
{
	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
	if( material->reflectivity.getValue() < 0.0 ) material->reflectivity = 0.0;
   	if( material->reflectivity.getValue() > 1.0 ) material->reflectivity = 1.0;
}

void MaterialAngularDependingSpecular::updateAmbientColor( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->ambientColor.setValue( material->mAmbientColor[0] );
}

void MaterialAngularDependingSpecular::updateDiffuseColor( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->diffuseColor.setValue( material->mDiffuseColor[0] );
}

void MaterialAngularDependingSpecular::updateSpecularColor( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->specularColor.setValue( material->mSpecularColor[0] );
}

void MaterialAngularDependingSpecular::updateEmissiveColor( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->emissiveColor.setValue( material->mEmissiveColor[0] );
}

void MaterialAngularDependingSpecular::updateShininess( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->shininess.setValue( material->mShininess[0] );
}

void MaterialAngularDependingSpecular::updateTransparency( void* data, SoSensor* )
{
   	MaterialAngularDependingSpecular* material = static_cast< MaterialAngularDependingSpecular* >( data );
 	material->transparency.setValue( material->mTransparency[0] );
}

bool MaterialAngularDependingSpecular::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay ) const
{
	double randomNumber = rand.RandomDouble();

	NormalVector normalVector = dg->normal;

	double cosTheta = DotProduct( normalVector, incident.direction() );

	double actualReflectivity = reflectivity.getValue() * ( 2 - pow( cosTheta , ( (1 - cosTheta) * reflectivityCoef.getValue() )) );

	if ( randomNumber >= actualReflectivity  ) return false;

	//Compute reflected ray (local coordinates )
	outputRay->origin = dg->point;

	double sigmaNormal = sigmaSlope.getValue() / 1000;
	if( sigmaNormal > 0.0 )
	{
		NormalVector errorNormal = Normalize( NormalVector( ComputeErrorVector( sigmaNormal, rand ) ) );
		Vector3D r = dg->normal;
		Vector3D s = Normalize( dg->dpdu );
		Vector3D t = Normalize( dg->dpdv );
		Transform trasform( s.x, s.y, s.z, 0.0,
							r.x, r.y, r.z, 0.0,
							t.x, t.y, t.z, 0.0,
							0.0, 0.0, 0.0, 1.0);

		normalVector = trasform.GetInverse()( errorNormal );
	}

	outputRay->setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );

	//Add error to reflected ray
	double sigmaReflected= sigmaSpecularity.getValue() / 1000;
	if( sigmaReflected > 0.0 )
	{
		Vector3D errorReflectedRay = ComputeErrorVector( sigmaReflected, rand );

		Vector3D r = outputRay->direction();
		Vector3D s = Normalize( CrossProduct( outputRay->direction(), dg->normal ) );
		Vector3D t = Normalize( CrossProduct( outputRay->direction(), s ) );
		Transform trasform( s.x, s.y, s.z, 0.0,
								r.x, r.y, r.z, 0.0,
								t.x, t.y, t.z, 0.0,
								0.0, 0.0, 0.0, 1.0);

		Vector3D errorReflectedRayDirection = trasform.GetInverse()( errorReflectedRay );
		outputRay->setDirection( errorReflectedRayDirection );
	}

	return true;
}

Vector3D MaterialAngularDependingSpecular::ComputeErrorVector( double simgaError, RandomDeviate& rand ) const
{
	Vector3D errorVector;
	if( distribution.getValue() == 0 )
	{
		double phi = gc::TwoPi * rand.RandomDouble();
		double theta = simgaError * rand.RandomDouble();

		errorVector.x = sin( theta ) * sin( phi ) ;
		errorVector.y = cos( theta );
		errorVector.z = sin( theta ) * cos( phi );
	 }
	 else if( distribution.getValue() == 1 )
	 {
		 errorVector.x = simgaError * tgf::AlternateBoxMuller( rand );
		 errorVector.y = 1.0;
		 errorVector.z = simgaError * tgf::AlternateBoxMuller( rand );

	 }
	return errorVector;
}
