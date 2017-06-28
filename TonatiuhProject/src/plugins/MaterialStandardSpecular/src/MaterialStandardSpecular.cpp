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
#include "TParameterList.h"
#include "Transform.h"

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

	MaterialStandardSpecular::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Material" ), QString( "MaterialStandardSpecular" ), &MaterialStandardSpecular::CreateInstance );
}

/*!
 * Creates a material object
 */
MaterialStandardSpecular::MaterialStandardSpecular()
:TMaterial()
//,
 //m_nodeType(TNodeType::FromName( QLatin1String( "MaterialStandardSpecular" ) ) )
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("reflectivity"), 0.0 );
	m_parametersList->Append( QLatin1String("sigmaSlope"), 2.0 );
	m_parametersList->Append( QLatin1String("distribution"), QLatin1String( "PILLBOX" ) );

	m_parametersList->Append( QLatin1String("color"), QLatin1String( "0.2 0.2 0.2" ) );
	m_parametersList->Append( QLatin1String("transparency"), 0.0 );
}


/*!
 * Destructor.
 */
MaterialStandardSpecular::~MaterialStandardSpecular()
{

}

/*!
 * Returns the material icon filename.
 */
QString MaterialStandardSpecular::GetIcon()
{
	return ( QLatin1String(":icons/MaterialStandardSpecular.png") );
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

	double reflectivity = m_parametersList->Get( QLatin1String( "reflectivity" ) ).toDouble();
	double sigmaSlopeMRAD = m_parametersList->Get( QLatin1String( "sigmaSlope" ) ).toDouble();
	QString distribution = m_parametersList->Get( QLatin1String( "distribution" ) ).toString();

	double randomNumber = rand.RandomDouble();
	if ( randomNumber >= reflectivity  ) return ( false );

	//Compute reflected ray (local coordinates )
	outputRay->origin = dg->point;

	NormalVector normalVector;
	double sigmaSlope = sigmaSlopeMRAD / 1000;
	if( sigmaSlope > 0.0 )
	{
		NormalVector errorNormal;
		if( distribution == QLatin1String( "PILLBOX" )  )
		{
			double phi = gc::TwoPi * rand.RandomDouble();
			double theta = sigmaSlope * rand.RandomDouble();

			errorNormal.x = sin( theta ) * sin( phi ) ;
			errorNormal.y = cos( theta );
			errorNormal.z = sin( theta ) * cos( phi );
		 }
		 else if ( distribution== QLatin1String( "NORMAL" )  )
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
