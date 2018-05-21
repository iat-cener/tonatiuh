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

#include "gc.h"

#include "DifferentialGeometry.h"
#include "MaterialBasicRefractive.h"
#include "RandomDeviate.h"
#include "Ray.h"
//#include "tgf.h"
#include "sf.h"
#include "Transform.h"


TNodeType MaterialBasicRefractive::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > MaterialBasicRefractive::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateMaterialBasicRefractive : public MaterialBasicRefractive { using MaterialBasicRefractive::MaterialBasicRefractive; };
	return std::make_shared<EnableCreateMaterialBasicRefractive>();
}

/*!
 * Initializes MaterialStandardSpecular type.
 */
void MaterialBasicRefractive::Init()
{

	MaterialBasicRefractive::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Material" ), "MaterialBasicRefractive", &MaterialBasicRefractive::CreateInstance );
}

/*!
 * Creates a material object
 */
MaterialBasicRefractive::MaterialBasicRefractive()
:m_sigmaOpt( 0 ),
 m_reflectivityFrontLabel( "reflectivityFront" ),
 m_reflectivityBackLabel( "reflectivityBack" ),
 m_transmissivityFrontLabel( "transmissivityFront" ),
 m_transmissivityBackLabel( "transmissivityBack" ),
 m_nFrontLabel( "nFront" ),
 m_nBackLabel( "nBack" ),
 m_sigmaSlopeLabel( "sigmaSlope" ),
 m_distributionLabel( "distribution" ),
 m_colorLabel( "color" ),
 m_transparencyLabel( "transparency" )
{
	SetName(GetType().GetName() );


	//Translation
	m_pParametersList->Append<double>( m_reflectivityFrontLabel, 0.0, true );
	m_pParametersList->Append<double>( m_reflectivityBackLabel, 0.0, true );
	m_pParametersList->Append<double>( m_transmissivityFrontLabel, 0.0, true );
	m_pParametersList->Append<double>( m_transmissivityBackLabel, 0.0, true );
	m_pParametersList->Append<double>( m_nFrontLabel, 0.0, true );
	m_pParametersList->Append<double>( m_nBackLabel, 0.0, true );
	m_pParametersList->Append<double>( m_sigmaSlopeLabel, 2.0, true );

	EnumeratedTypes distributionEnumerator;
	distributionEnumerator.AddValue( "NORMAL", true );
	m_pParametersList->Append<EnumeratedTypes>( m_distributionLabel, distributionEnumerator, true );

	m_pParametersList->Append<std::string>(m_colorLabel,  "0.2 0.2 0.2", true );
	m_pParametersList->Append<double>( m_transparencyLabel, 0.0, true );

}

/*!
 * Destructor.
 */
MaterialBasicRefractive::~MaterialBasicRefractive()
{
}

/*!
 * Creates a copy of material node.
 */
std::shared_ptr< TNode > MaterialBasicRefractive::Copy() const
{
	//shared_prt needs a public constructor
	struct EnableCreateMaterialBasicRefractive : public MaterialBasicRefractive { using MaterialBasicRefractive::MaterialBasicRefractive; };
	std::shared_ptr<MaterialBasicRefractive> materialNode = std::make_unique<EnableCreateMaterialBasicRefractive>();
	if( materialNode == 0 )	return ( 0  );

	 //Coping node parts.
	 //NO parts

	 //Coping the parameters.
	 materialNode->m_pParametersList->SetValue( m_reflectivityFrontLabel, GetParameterValue<double>( m_reflectivityFrontLabel ) );
	 materialNode->m_pParametersList->SetValue( m_reflectivityBackLabel, GetParameterValue<double>( m_reflectivityBackLabel ) );
	 materialNode->m_pParametersList->SetValue( m_transmissivityFrontLabel, GetParameterValue<double>( m_transmissivityFrontLabel ) );
	 materialNode->m_pParametersList->SetValue( m_transmissivityBackLabel, GetParameterValue<double>( m_transmissivityBackLabel ) );
	 materialNode->m_pParametersList->SetValue( m_nFrontLabel, GetParameterValue<double>( m_nFrontLabel ) );
	 materialNode->m_pParametersList->SetValue( m_nBackLabel, GetParameterValue<double>( m_nBackLabel ) );
	 materialNode->m_pParametersList->SetValue( m_sigmaSlopeLabel, GetParameterValue<double>( m_sigmaSlopeLabel ) );
	 materialNode->m_pParametersList->SetValue( m_distributionLabel, GetParameterValue<EnumeratedTypes>( m_distributionLabel ) );
	 materialNode->m_pParametersList->SetValue( m_colorLabel, GetParameterValue<std::string>( m_colorLabel ) );
	 materialNode->m_pParametersList->SetValue( m_transparencyLabel, GetParameterValue<double>( m_transparencyLabel ) );

	 return ( materialNode );
 }

/*!
 * Returns the material icon filename.
 */
std::string  MaterialBasicRefractive::GetIcon()
{
	return std::string (":icons/MaterialBasicRefractive.png");
}

/*!
 * Returns material type.
 */
TNodeType MaterialBasicRefractive::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Checks if the material generates an \a outputRay for givem \a incident ray and intersection properties defined in \a dg.
 */
bool MaterialBasicRefractive::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
{
	//return ( false );

	double reflectivityFront = GetParameterValue<double>( m_reflectivityFrontLabel );
	double reflectivityBack = GetParameterValue<double>( m_reflectivityBackLabel );
	double transmissivityFront = GetParameterValue<double>( m_transmissivityFrontLabel );
	double transmissivityBack = GetParameterValue<double>( m_transmissivityBackLabel );
	//double sigmaSlopeMRAD = GetParameterValue<double>( m_sigmaSlopeLabel );
	//EnumeratedTypes typeOfDistribution = GetParameterValue<EnumeratedTypes>( m_distributionLabel );


	double randomNumber = rand.RandomDouble();
	bool shapeFrontSide = ( DotProduct( dg->normal, incident.direction() ) > 0 ) ? false : true;
	if( shapeFrontSide )
	{
		if ( randomNumber < reflectivityFront  )
		{
			*outputRay = ReflectedRay( incident, dg, rand );
			return true;
		}
		else if ( randomNumber < ( reflectivityFront + transmissivityFront ) )
		{
			*outputRay = RefractedtRay( incident, dg, rand );
			return true;
		}
		else return false;
	}
	else
	{
		if ( randomNumber < reflectivityBack )
		{
			*outputRay = ReflectedRay( incident, dg, rand );
			return true;
		}
		else if ( randomNumber < ( reflectivityBack + transmissivityBack ) )
		{
			*outputRay = RefractedtRay( incident, dg, rand );
			return true;
		}
		else return false;

	}
}

Ray MaterialBasicRefractive::ReflectedRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand ) const
{
	NormalVector dgNormal;
	bool shapeFrontSide = ( DotProduct( dg->normal, incident.direction() ) > 0 ) ? false : true;
	if( shapeFrontSide )
		dgNormal = dg->normal;
	else
		dgNormal = - dg->normal;


	//Compute reflected ray (local coordinates )
	Ray reflected;
	reflected.origin = dg->point;

	NormalVector normalVector;

	double sigmaSlopeMRAD = GetParameterValue<double>( m_sigmaSlopeLabel );
	EnumeratedTypes typeOfDistribution = GetParameterValue<EnumeratedTypes>( m_distributionLabel );
	double sSlope = sigmaSlopeMRAD / 1000;
	if( sSlope > 0.0 )
	{
		NormalVector errorNormal;
		std::string typeOfDistributionValue = typeOfDistribution.GetSelectedName();
		if( typeOfDistributionValue == "PILLBOX" )
		{
			/*
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
			*/

		}
		else if( typeOfDistributionValue == "NORMAL" )
		{
			 errorNormal.x = sSlope * sf::AlternateBoxMuller( rand );
			 errorNormal.y = 1.0;
			 errorNormal.z = sSlope * sf::AlternateBoxMuller( rand );

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

Ray MaterialBasicRefractive::RefractedtRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& /* rand */  ) const
{

	NormalVector s;
	double n1;
	double n2;
	bool shapeFrontSide = ( DotProduct( dg->normal, incident.direction() ) > 0 ) ? false : true;
	if( shapeFrontSide )
	{
		s = dg->normal;
		n1 = GetParameterValue<double>( m_nFrontLabel );
		n2 = GetParameterValue<double>( m_nBackLabel );
	}
	else
	{
		s = - dg->normal;
		n1 = GetParameterValue<double>( m_nBackLabel );
		n2 = GetParameterValue<double>( m_nFrontLabel );
	}

	//Compute refracted ray (local coordinates )
	Ray refracted;
	refracted.origin = dg->point;


	double cosTheta = DotProduct( -incident.direction(), s );
/*
	double disc = ( cosTheta * cosTheta ) * ( ( n2 / n1 ) * ( n2 / n1 ) ) - 1;
	if( n1 > n2 )
	{
		if( disc > 0 )refracted->setDirection( ( n1 / n2 ) * ( incident.direction() + ( cosTheta - sqrt( disc ) )* s ) );
		else
			refracted->setDirection( Normalize( incident.direction() + 2.0 * cosTheta * s ) );
	}
	else
		refracted->setDirection( ( n1 / n2 ) * ( incident.direction() + ( cosTheta - sqrt( disc ) )* s ) );
	*/

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
