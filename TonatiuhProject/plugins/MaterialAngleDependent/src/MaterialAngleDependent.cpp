/*
 * MaterialAngleDependent.cpp
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */




#include <QMessageBox>
#include <QString>

#include <Inventor/lists/SoFieldList.h>
#include <Inventor/fields/SoFieldContainer.h>

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialAngleDependent.h"
#include "RandomDeviate.h"
#include "Ray.h"
#include "tgf.h"
#include "Transform.h"


SO_NODE_SOURCE( MaterialAngleDependent );

void MaterialAngleDependent::initClass()
{
	 SO_NODE_INIT_CLASS( MaterialAngleDependent, TMaterial, "Material" );
}

MaterialAngleDependent::MaterialAngleDependent()
: m_reflectivityFrontValuesSensor( 0 ),
  m_reflectivityBackValuesSensor( 0 ),
  m_ambientColorSensor( 0 ),
  m_diffuseColorSensor( 0 ),
  m_specularColorSensor( 0 ),
  m_emissiveColorSensor( 0 ),
  m_shininessSensor( 0 ),
  m_transparencySensor( 0 )
{
	SO_NODE_CONSTRUCTOR( MaterialAngleDependent );

	SO_NODE_ADD_FIELD(reflectivityFront, (TRUE) );
	SO_NODE_ADD_FIELD(reflectivityFrontValues, (0.0f, 0.0f) );

	SO_NODE_ADD_FIELD(reflectivityBack, (TRUE) );
	SO_NODE_ADD_FIELD(reflectivityBackValues, (0.0f, 0.0f) );



	SO_NODE_ADD_FIELD( sigmaSlope, (2.0) );
	SO_NODE_DEFINE_ENUM_VALUE(Distribution, PILLBOX);
  	SO_NODE_DEFINE_ENUM_VALUE(Distribution, NORMAL);
  	SO_NODE_SET_SF_ENUM_TYPE( distribution, Distribution);
	SO_NODE_ADD_FIELD( distribution, (PILLBOX) );


	SO_NODE_ADD_FIELD( ambient_Color, (0.2f, 0.2f, 0.2f) );
	SO_NODE_ADD_FIELD( diffuse_Color, (0.8f, 0.8f, 0.8f) );
	SO_NODE_ADD_FIELD( specular_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( emissive_Color, (0.0f, 0.0f, 0.0f) );
	SO_NODE_ADD_FIELD( shininessValue, (0.2f) );
	SO_NODE_ADD_FIELD( transparencyValue, (0.0) );

	//m_reflectivityFrontSensor = new SoFieldSensor(  updateReflectivityFront,  this );
	//m_reflectivityFrontSensor->setPriority( 1 );
	//m_reflectivityFrontSensor->attach( &reflectivityFront );

	m_reflectivityFrontValuesSensor = new SoFieldSensor(  updateReflectivityFront,  this );
	m_reflectivityFrontValuesSensor->setPriority( 1 );
	m_reflectivityFrontValuesSensor->attach( &reflectivityFrontValues );

	//m_reflectivityBackSensor = new SoFieldSensor(  updateReflectivityBack,  this );
	//m_reflectivityBackSensor->setPriority( 1 );
	//m_reflectivityBackSensor->attach( &reflectivityBack );

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

MaterialAngleDependent::~MaterialAngleDependent()
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

QString MaterialAngleDependent::getIcon()
{
	return QString(":icons/MaterialAngleDependent.png");
}


/*!
 * Linear interpolation of the vectors \a incidenceAnglesList and \a valuesList to obtaine the property value for the angle \a incidenceAngle.
 */
// New function OutputPropertyValue (JMQ)
double MaterialAngleDependent::OutputPropertyValue( std::vector< double > incidenceAnglesList,  std::vector< double > valuesList, double incidenceAngle ) const
{


	// Size incidenceAnglesList vector
	int m = incidenceAnglesList.size();


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
void MaterialAngleDependent::updateReflectivityFront( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );

	int numberOfValues = material->reflectivityFrontValues.getNum();

	material->m_frontReflectivityIncidenceAngle.clear();
	material->m_frontReflectivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		SbVec2f  vect2 = material->reflectivityFrontValues[i];
		material->m_frontReflectivityIncidenceAngle.push_back( vect2[0] );
		material->m_frontReflectivityValue.push_back( vect2[1] );
	}
}

/*!
 * Updates the internal vectors of the back parameters  with the values in the inputs.
 */
void MaterialAngleDependent::updateReflectivityBack( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );

	int numberOfValues = material->reflectivityBackValues.getNum();

	material->m_backReflectivityIncidenceAngle.clear();
	material->m_backReflectivityValue.clear();
	for( int i = 0; i < numberOfValues; i++ )
	{
		SbVec2f  vect2 = material->reflectivityBackValues[i];
		material->m_backReflectivityIncidenceAngle.push_back( vect2[0] );
		material->m_backReflectivityValue.push_back( vect2[1] );
	}
}

/*!
 * Updates the material ambient color values.
 */
void MaterialAngleDependent::updateAmbientColor( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->ambientColor.setValue( material->ambient_Color.getValue() );
}

void MaterialAngleDependent::updateDiffuseColor( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->diffuseColor.setValue( material->diffuse_Color.getValue() );
}

void MaterialAngleDependent::updateSpecularColor( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->specularColor.setValue( material->specular_Color.getValue() );
}

void MaterialAngleDependent::updateEmissiveColor( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->emissiveColor.setValue( material->emissive_Color.getValue() );
}

void MaterialAngleDependent::updateShininess( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->shininess.setValue( material->shininessValue.getValue() );
}

void MaterialAngleDependent::updateTransparency( void* data, SoSensor* )
{
	MaterialAngleDependent* material = static_cast< MaterialAngleDependent* >( data );
 	material->transparency.setValue( material->transparencyValue.getValue() );
}

/*
 * Calculates the output ray \a outputRay for the \a incident ray for the intersection parameters \a dg.
 *
 * Returns FALSE if there is not output ray.
 */
bool MaterialAngleDependent::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
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


