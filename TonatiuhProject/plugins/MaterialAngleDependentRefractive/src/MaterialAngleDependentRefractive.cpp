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

#include <QMessageBox>
#include <QString>

#include <Inventor/lists/SoFieldList.h>
#include <Inventor/fields/SoFieldContainer.h>

#include "gc.h"
#include "trt.h"

#include "DifferentialGeometry.h"
#include "MaterialAngleDependentRefractive.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE( MaterialAngleDependentRefractive );

void MaterialAngleDependentRefractive::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialAngleDependentRefractive, TMaterial, "Material" );
}

MaterialAngleDependentRefractive::MaterialAngleDependentRefractive()
: m_frontOpticValuesSensor( 0 ),
  m_backOpticValuesSensor( 0 ),
  m_ambientColorSensor( 0 ),
  m_diffuseColorSensor( 0 ),
  m_specularColorSensor( 0 ),
  m_emissiveColorSensor( 0 ),
  m_shininessSensor( 0 ),
  m_transparencySensor( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialAngleDependentRefractive );

	//SO_NODE_ADD_FIELD(reflectivityFront, (TRUE) );
	SO_NODE_ADD_FIELD( nFront, (0.0) );
	SO_NODE_ADD_FIELD(frontOpticValues, (0.0f, 0.0f, 0.0f) );
	frontOpticValues.SetNames( QObject::tr("Angle [rad]" ), QObject::tr( "Reflectivity[0-1]" ), QObject::tr( "Transmissivity[0-1]" ) );

	//SO_NODE_ADD_FIELD(reflectivityBack, (TRUE) );
	SO_NODE_ADD_FIELD( nBack, (0.0) );
	SO_NODE_ADD_FIELD(backOpticValues, (0.0f, 0.0f, 0.0f) );
	backOpticValues.SetNames( QObject::tr("Angle [rad]" ), QObject::tr( "Reflectivity[0-1]" ), QObject::tr( "Transmissivity[0-1]" ) );


	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	//SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (NORMAL) );


	SO_NODE_ADD_FIELD( ambient_Color, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( diffuse_Color, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( specular_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( emissive_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( shininessValue, (0.2f) );
	SO_NODE_ADD_FIELD( transparencyValue, (0.0) );



	m_frontOpticValuesSensor = new SoFieldSensor(  updateOpticFront,  this );
	m_frontOpticValuesSensor->setPriority(0 );
	m_frontOpticValuesSensor->attach( &frontOpticValues );


	m_backOpticValuesSensor = new SoFieldSensor(  updateOpticBack,  this );
	m_backOpticValuesSensor->setPriority( 0 );
	m_backOpticValuesSensor->attach( &backOpticValues );

	m_ambientColorSensor = new SoFieldSensor( updateAmbientColor, this );
	m_ambientColorSensor->setPriority( 1 );
	m_ambientColorSensor->attach( &ambient_Color );
	m_diffuseColorSensor = new SoFieldSensor( updateDiffuseColor, this );
	m_diffuseColorSensor->setPriority( 1 );
	m_diffuseColorSensor->attach( &diffuse_Color );
	m_specularColorSensor = new SoFieldSensor( updateSpecularColor, this );
	m_specularColorSensor->setPriority( 1 );
	m_specularColorSensor->attach( &specular_Color );
	m_emissiveColorSensor = new SoFieldSensor( updateEmissiveColor, this );
	m_emissiveColorSensor->setPriority( 1 );
	m_emissiveColorSensor->attach( &emissive_Color );
	m_shininessSensor = new SoFieldSensor( updateShininess, this );
	m_shininessSensor->setPriority( 1 );
	m_shininessSensor->attach( &shininessValue );
	m_transparencySensor = new SoFieldSensor( updateTransparency, this );
	m_transparencySensor->setPriority( 1 );
	m_transparencySensor->attach( &transparencyValue );


}

MaterialAngleDependentRefractive::~MaterialAngleDependentRefractive()
{
	delete m_frontOpticValuesSensor;
	delete m_backOpticValuesSensor;

	delete m_ambientColorSensor;
	delete m_diffuseColorSensor;
	delete m_specularColorSensor;
	delete m_emissiveColorSensor;
	delete m_shininessSensor;
	delete m_transparencySensor;
}

QString MaterialAngleDependentRefractive::getIcon()
{
	return QString(":icons/MaterialAngleDependentSpecular.png");
}


/*!
 * Linear interpolation of the vectors \a incidenceAnglesList and \a valuesList to obtaine the property value for the angle \a incidenceAngle.
 */
//New function for interpolation reflectivity and transmissivity

std::vector < double > MaterialAngleDependentRefractive::OutputPropertyValue (std::vector < double > incidenceAnglesList, std::vector < double > reflectivityValuesList, std::vector < double > transmissivityValuesList, double incidenceAngle) const
{
	int m = incidenceAnglesList.size();

	std::vector < double > propValue;
	propValue.push_back(0.0);
	propValue.push_back(0.0);


	if( incidenceAngle > 0.5* gc::Pi )	incidenceAngle = 0.5 * gc:: Pi;

		for(  int i = 0;  i < m; i++ )
		{
			if( incidenceAnglesList[i] >= incidenceAngle )
			{
				double interpol = (incidenceAngle - incidenceAnglesList[i-1] ) /( incidenceAnglesList[i]  - incidenceAnglesList[i-1] );
				propValue[0] = reflectivityValuesList[i-1] + interpol * ( reflectivityValuesList[i] - reflectivityValuesList[i-1] );
				propValue[1] = transmissivityValuesList[i-1] + interpol * ( transmissivityValuesList[i] - transmissivityValuesList[i-1] );

				break;
			}
		}
		return ( propValue );
}

/*!
 * Updates the internal vectors of the front parameters  with the values in the inputs.
 */
void MaterialAngleDependentRefractive::updateOpticFront( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );

	std::vector< double > oldFrontReflectivityIncidenceAngle = material->m_frontReflectivityIncidenceAngle;
	std::vector< double > oldFrontReflectivityValue = material->m_frontReflectivityValue;
	std::vector< double > oldFrontTransmissivityValue = material->m_frontTransmissivityValue;


	int numberOfValues = material->frontOpticValues.getNum();

	material->m_frontReflectivityIncidenceAngle.clear();
	material->m_frontReflectivityValue.clear();
	material->m_frontTransmissivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		material->m_frontReflectivityIncidenceAngle.push_back( material->frontOpticValues[i][0] );
		material->m_frontReflectivityValue.push_back( material->frontOpticValues[i][1] );
		material->m_frontTransmissivityValue.push_back( material->frontOpticValues[i][2] );
	}
}

/*!
 * Updates the internal vectors of the back parameters  with the values in the inputs.
 */
void MaterialAngleDependentRefractive::updateOpticBack( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );


	//std::vector< double > m_backReflectivityIncidenceAngle;
	//std::vector< double > m_backReflectivityValue;
	int numberOfValues = material->backOpticValues.getNum();

	material->m_backReflectivityIncidenceAngle.clear();
	material->m_backReflectivityValue.clear();
	material->m_backTransmissivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		material->m_backReflectivityIncidenceAngle.push_back( material->backOpticValues[i][0] );
		material->m_backReflectivityValue.push_back( material->backOpticValues[i][1] );
		material->m_backTransmissivityValue.push_back( material->backOpticValues[i][2] );
	}
}

/*!
 * Updates the material ambient color values.
 */
void MaterialAngleDependentRefractive::updateAmbientColor( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->ambientColor.setValue( material->ambient_Color.getValue() );
}

void MaterialAngleDependentRefractive::updateDiffuseColor( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->diffuseColor.setValue( material->diffuse_Color.getValue() );
}

void MaterialAngleDependentRefractive::updateSpecularColor( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->specularColor.setValue( material->specular_Color.getValue() );
}

void MaterialAngleDependentRefractive::updateEmissiveColor( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->emissiveColor.setValue( material->emissive_Color.getValue() );
}

void MaterialAngleDependentRefractive::updateShininess( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->shininess.setValue( material->shininessValue.getValue() );
}

void MaterialAngleDependentRefractive::updateTransparency( void* data, SoSensor* )
{
	MaterialAngleDependentRefractive* material = static_cast< MaterialAngleDependentRefractive* >( data );
 	material->transparency.setValue( material->transparencyValue.getValue() );
}


bool MaterialAngleDependentRefractive::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
{
	NormalVector dgNormal;
	std::vector< double>  reflectivityTransmissivity;
	if( dg->shapeFrontSide )
	{

		dgNormal = dg->normal;
		double incidenceAngle = acos( DotProduct( -incident.direction(), dgNormal ) );
		reflectivityTransmissivity = OutputPropertyValue( m_frontReflectivityIncidenceAngle,  m_frontReflectivityValue, m_frontTransmissivityValue, incidenceAngle );
	}
	else
	{

		dgNormal = - dg->normal;
		double incidenceAngle = acos( DotProduct( -incident.direction(), dgNormal ) );
		reflectivityTransmissivity = OutputPropertyValue( m_backReflectivityIncidenceAngle,  m_backReflectivityValue, m_backTransmissivityValue, incidenceAngle );
	}
	double randomNumber = rand.RandomDouble();
	double reflectivity = reflectivityTransmissivity[0];
	double transmissivity = reflectivityTransmissivity[1];

	if ( randomNumber < reflectivity )
	{
		*outputRay = ReflectedRay( incident, dg, rand );
		return true;
	}
	else if ( randomNumber < ( reflectivity + transmissivity ) )
	{
		*outputRay = RefractedtRay( incident, dg, rand );
		return true;
	}
	else return false;
}

Ray MaterialAngleDependentRefractive::ReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand ) const
{
	NormalVector dgNormal;

	if( dg->shapeFrontSide )	dgNormal = dg->normal;
	else	dgNormal = - dg->normal;

	//Compute reflected ray (local coordinates )
	Ray reflected;
	reflected.origin = dg->point;

	NormalVector normalVector;
	double sSlope = sigmaSlope.getValue() / 1000;
	if( sSlope > 0.0 )
	{
		NormalVector errorNormal;
		if ( distribution.getValue() == 0 )
		{
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
		 }
		 else if (distribution.getValue() == 1 )
		 {
			 errorNormal.x = sSlope * tgf::AlternateBoxMuller( rand );
			 errorNormal.y = 1.0;
			 errorNormal.z = sSlope * tgf::AlternateBoxMuller( rand );

		 }
		Vector3D r = dgNormal;
		Vector3D s = Normalize( dg->dpdu );
		Vector3D t = Normalize( dg->dpdv );
		Transform trasform( s.x, s.y, s.z, 0.0,
								r.x, r.y, r.z, 0.0,
								t.x, t.y, t.z, 0.0,
								0.0, 0.0, 0.0, 1.0);

		NormalVector normalDirection = trasform.GetInverse()( errorNormal );
		normalVector = Normalize( normalDirection );
	}
	else
	{
		normalVector = dgNormal;
	}

	double cosTheta = DotProduct( normalVector, incident.direction() );
	reflected.setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );
	return reflected;

}

Ray MaterialAngleDependentRefractive::RefractedtRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& /* rand */  ) const
{
	NormalVector s;
	double n1;
	double n2;

	if( dg->shapeFrontSide )
	{
		s = dg->normal;
		n1 = nFront.getValue();
		n2 = nBack.getValue();
	}
	else
	{
		s = - dg->normal;
		n1 = nBack.getValue();
		n2 = nFront.getValue();
	}

	//Compute refracted ray (local coordinates )
	Ray refracted;
	refracted.origin = dg->point;

	double cosTheta = DotProduct( -incident.direction(), s );

	double sin2Theta = ( n1/ n2 ) * ( n1/ n2 )  * ( 1 - cosTheta  * cosTheta );
	if( n1 > n2 )
	{
		if( sin2Theta < 1.0 ) refracted.setDirection( Normalize( ( n1/ n2 ) * incident.direction() + ( ( n1/ n2 ) * cosTheta -  sqrt(1 - sin2Theta ) ) * s ) );
		else //Total Internal Reflection
			refracted.setDirection( Normalize( incident.direction() + 2.0 * cosTheta * s ) );
	}
	else
		refracted.setDirection( Normalize( ( n1/ n2 ) * incident.direction() + ( ( n1/ n2 ) * cosTheta -  sqrt(1 - sin2Theta ) ) * s ) );

	return refracted;

}


