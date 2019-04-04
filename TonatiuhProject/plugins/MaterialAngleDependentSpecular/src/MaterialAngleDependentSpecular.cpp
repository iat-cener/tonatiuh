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
#include "MaterialAngleDependentSpecular.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE( MaterialAngleDependentSpecular );

void MaterialAngleDependentSpecular::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialAngleDependentSpecular, TMaterial, "Material" );
}

MaterialAngleDependentSpecular::MaterialAngleDependentSpecular()
: m_reflectivityFrontValuesSensor( 0 ),
  m_reflectivityBackValuesSensor( 0 ),
  m_ambientColorSensor( 0 ),
  m_diffuseColorSensor( 0 ),
  m_specularColorSensor( 0 ),
  m_emissiveColorSensor( 0 ),
  m_shininessSensor( 0 ),
  m_transparencySensor( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialAngleDependentSpecular );

	SO_NODE_ADD_FIELD(reflectivityFront, (TRUE) );
	SO_NODE_ADD_FIELD(reflectivityFrontValues, (0.0f, 0.0f) );
	reflectivityFrontValues.SetNames( QObject::tr("Angle [rad]" ), QObject::tr( "Reflectivity[0-1]" ) );

	SO_NODE_ADD_FIELD(reflectivityBack, (TRUE) );
	SO_NODE_ADD_FIELD(reflectivityBackValues, (0.0f, 0.0f) );
	reflectivityBackValues.SetNames( QObject::tr("Angle [rad]" ), QObject::tr( "Reflectivity[0-1]" ) );


	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	//SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution);
	//SO_NODE_ADD_FIELD( distribution, (PILLBOX) );


	SO_NODE_ADD_FIELD( ambient_Color, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( diffuse_Color, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( specular_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( emissive_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( shininessValue, (0.2f) );
	SO_NODE_ADD_FIELD( transparencyValue, (0.0) );



	m_reflectivityFrontValuesSensor = new SoFieldSensor(  updateReflectivityFront,  this );
	m_reflectivityFrontValuesSensor->setPriority( 1 );
	m_reflectivityFrontValuesSensor->attach( &reflectivityFrontValues );


	m_reflectivityBackValuesSensor = new SoFieldSensor(  updateReflectivityBack,  this );
	m_reflectivityBackValuesSensor->setPriority( 1 );
	m_reflectivityBackValuesSensor->attach( &reflectivityBackValues );

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

MaterialAngleDependentSpecular::~MaterialAngleDependentSpecular()
{
	delete m_reflectivityFrontValuesSensor;
	delete m_reflectivityBackValuesSensor;

	delete m_ambientColorSensor;
	delete m_diffuseColorSensor;
	delete m_specularColorSensor;
	delete m_emissiveColorSensor;
	delete m_shininessSensor;
	delete m_transparencySensor;
}

QString MaterialAngleDependentSpecular::getIcon()
{
	return QString(":icons/MaterialAngleDependentSpecular.png");
}


/*!
 * Linear interpolation of the vectors \a incidenceAnglesList and \a valuesList to obtaine the property value for the angle \a incidenceAngle.
 */
// New function OutputPropertyValue (JMQ)
double MaterialAngleDependentSpecular::OutputPropertyValue( std::vector< double > incidenceAnglesList,  std::vector< double > valuesList, double incidenceAngle ) const
{
	// Size incidenceAnglesList vector
	int m = incidenceAnglesList.size();

	//Classify element's incident Angle lowest to biggest
	for (int i = 0; i < (m - 1); i++)
	{
		for (int j = i + 1; j < m; j++)
		{
			if (incidenceAnglesList[j] < incidenceAnglesList[i])
			{
				double aux1 = incidenceAnglesList[j];
				double aux2 = valuesList[j];
				incidenceAnglesList[j] = incidenceAnglesList[i];
				valuesList[j] = valuesList[i];
				incidenceAnglesList[i] = aux1;
				valuesList[i] = aux2;
			}
		}
	}


	//Incidence angle can not be higher than 0.5 * Pi
	if( incidenceAngle > 0.5* gc::Pi )	incidenceAngle = 0.5 * gc:: Pi;

	double propValue = 0.0;
	for(  int i = 0;  i < m; i++ )
	{
		if( incidenceAnglesList[i] >= incidenceAngle )
		{
			double interpol = (incidenceAngle - incidenceAnglesList[i-1] ) /( incidenceAnglesList[i]  - incidenceAnglesList[i-1] );
			propValue = valuesList[i-1] + interpol * ( valuesList[i] - valuesList[i-1] );
			// Return reflectivity

			break;
		}
	}
	return ( propValue );

}

/*!
 * Updates the internal vectors of the front parameters  with the values in the inputs.
 */
void MaterialAngleDependentSpecular::updateReflectivityFront( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );

	std::vector< double > oldFrontReflectivityIncidenceAngle = material->m_frontReflectivityIncidenceAngle;
	std::vector< double > oldFrontReflectivityValue = material->m_frontReflectivityValue;


	int numberOfValues = material->reflectivityFrontValues.getNum();

	material->m_frontReflectivityIncidenceAngle.clear();
	material->m_frontReflectivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		material->m_frontReflectivityIncidenceAngle.push_back( material->reflectivityFrontValues[i][0] );
		material->m_frontReflectivityValue.push_back( material->reflectivityFrontValues[i][1] );
	}
}

/*!
 * Updates the internal vectors of the back parameters  with the values in the inputs.
 */
void MaterialAngleDependentSpecular::updateReflectivityBack( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );


	//std::vector< double > m_backReflectivityIncidenceAngle;
	//std::vector< double > m_backReflectivityValue;
	int numberOfValues = material->reflectivityBackValues.getNum();

	material->m_backReflectivityIncidenceAngle.clear();
	material->m_backReflectivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		material->m_backReflectivityIncidenceAngle.push_back( material->reflectivityBackValues[i][0] );
		material->m_backReflectivityValue.push_back( material->reflectivityBackValues[i][1] );
	}
}

/*!
 * Updates the material ambient color values.
 */
void MaterialAngleDependentSpecular::updateAmbientColor( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->ambientColor.setValue( material->ambient_Color.getValue() );
}

void MaterialAngleDependentSpecular::updateDiffuseColor( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->diffuseColor.setValue( material->diffuse_Color.getValue() );
}

void MaterialAngleDependentSpecular::updateSpecularColor( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->specularColor.setValue( material->specular_Color.getValue() );
}

void MaterialAngleDependentSpecular::updateEmissiveColor( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->emissiveColor.setValue( material->emissive_Color.getValue() );
}

void MaterialAngleDependentSpecular::updateShininess( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->shininess.setValue( material->shininessValue.getValue() );
}

void MaterialAngleDependentSpecular::updateTransparency( void* data, SoSensor* )
{
	MaterialAngleDependentSpecular* material = static_cast< MaterialAngleDependentSpecular* >( data );
 	material->transparency.setValue( material->transparencyValue.getValue() );
}

/*
 * Calculates the output ray \a outputRay for the \a incident ray for the intersection parameters \a dg.
 *
 * Returns FALSE if there is not output ray.
 */
bool MaterialAngleDependentSpecular::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
{
	double reflectivity = 0.0;

	NormalVector dgNormal;
	if( dg->shapeFrontSide )
	{
		if( !reflectivityFront.getValue() )	return ( false );
		dgNormal = dg->normal;

		// Product
		double incidenceAngle  = acos( DotProduct( -incident.direction(), dgNormal ) );
		reflectivity = OutputPropertyValue( m_frontReflectivityIncidenceAngle,  m_frontReflectivityValue, incidenceAngle );
	}
	else
	{
		if( !reflectivityBack.getValue() )	return ( false );

		dgNormal = - dg->normal;
		double incidenceAngle  = acos( DotProduct( -incident.direction(), dgNormal) );
		reflectivity = OutputPropertyValue( m_backReflectivityIncidenceAngle,  m_backReflectivityValue, incidenceAngle );

	}

	double randomNumber = rand.RandomDouble();
	if ( randomNumber >= reflectivity  ) return ( false );

	//Compute reflected ray (local coordinates )
	//Ray* reflected = new Ray();
	//reflected.origin = dg->point;
	outputRay->origin = dg->point;

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
	outputRay->setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );
	return ( true );

}


