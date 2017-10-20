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

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "Ray.h"
#include "DifferentialGeometry.h"
#include "MaterialStandardSpecular.h"

#include "NormalVector.h"
#include "RandomDeviate.h"
#include "sf.h"
#include "Transform.h"

#include "TParameterEnumerator.h"
#include "TParameterList.h"

TNodeType MaterialStandardSpecular::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* MaterialStandardSpecular::CreateInstance( )
{
  return ( new MaterialStandardSpecular() );
}

/*!
 * Initializes MaterialStandardSpecular type.
 */
void MaterialStandardSpecular::Init()
{

	MaterialStandardSpecular::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Material" ), "MaterialStandardSpecular", &MaterialStandardSpecular::CreateInstance );
}

/*!
 * Creates a material object
 */
MaterialStandardSpecular::MaterialStandardSpecular()
:TMaterial()
{
	//setObjectName(GetType().GetName().c_str() );
	SetName(GetType().GetName() );


	//Translation
	m_parametersList->Append( "reflectivity", 0.0 );
	m_parametersList->Append( "sigmaSlope", 2.0 );

	TParameterEnumerator*  distributionEnumerator = new TParameterEnumerator;
	distributionEnumerator->AddValue( "PILLBOX", true );
	distributionEnumerator->AddValue( "NORMAL", false );

	QVariant distributionParameter;
	distributionParameter.setValue( distributionEnumerator);
	m_parametersList->Append( "distribution", distributionParameter );

	m_parametersList->Append( "color", "0.2 0.2 0.2" );
	m_parametersList->Append( "transparency", 0.0 );
}

/*!
 * Destructor.
 */
MaterialStandardSpecular::~MaterialStandardSpecular()
{
	TParameterEnumerator* typeOfDistribution = m_parametersList->GetValue( "distribution" ).value<TParameterEnumerator*>();
	delete typeOfDistribution;
	typeOfDistribution = 0;
}

/*!
 * Returns the material icon filename.
 */
std::string MaterialStandardSpecular::GetIcon()
{
	return ( ":icons/MaterialStandardSpecular.png" );
}

/*!
 * Returns material type.
 */
TNodeType MaterialStandardSpecular::GetType() const
{
	return ( m_nodeType );
}

/*!
 * Checks if the material generates an \a outputRay for givem \a incident ray and intersection properties defined in \a dg.
 */
bool MaterialStandardSpecular::OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const
{

	double reflectivity = m_parametersList->GetValue( "reflectivity" ).toDouble();
	double sigmaSlopeMRAD = m_parametersList->GetValue(  "sigmaSlope" ).toDouble();
	TParameterEnumerator* typeOfDistribution = m_parametersList->GetValue( "distribution" ).value<TParameterEnumerator*>();

	double randomNumber = rand.RandomDouble();
	if ( randomNumber >= reflectivity  ) return ( false );

	//Compute reflected ray (local coordinates )
	outputRay->origin = dg->point;

	NormalVector normalVector;
	double sigmaSlope = sigmaSlopeMRAD / 1000;
	if( sigmaSlope > 0.0 )
	{
		NormalVector errorNormal;
		std::string typeOfDistributionValue = typeOfDistribution->GetSelectedName();
		if( typeOfDistributionValue == "PILLBOX" )
		{
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sigmaSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
		}
		else if( typeOfDistributionValue == "NORMAL" )
		{
			errorNormal.x = sigmaSlope * sf::AlternateBoxMuller( rand );
			errorNormal.y = 1.0;
			errorNormal.z = sigmaSlope * sf::AlternateBoxMuller( rand );
		}

		Vector3D r = dg->normal;
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
		normalVector = dg->normal;
	}

	double cosTheta = DotProduct( normalVector, incident.direction() );
	outputRay->setDirection( Normalize( incident.direction() - 2.0 * normalVector * cosTheta ) );
	return ( true );
}
