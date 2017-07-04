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


#include "gc.h"

#include "SunshapeBuie.h"
#include "TParameterList.h"


TNodeType SunshapeBuie::m_nodeType = TNodeType::CreateEmptyType();
const double SunshapeBuie::m_minCRSValue = 0.000001;
const double SunshapeBuie::m_maxCRSValue = 0.849;


/*!
 * Creates a new instance of the class type corresponding object.
 */
void* SunshapeBuie::CreateInstance( )
{
  return ( new SunshapeBuie() );
}


/*!
 * Initializes ShapeFlatDisk type.
 */
void SunshapeBuie::Init()
{
	SunshapeBuie::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Sunshape" ), QString( "SunshapeBuie" ), &SunshapeBuie::CreateInstance );
}

/*!
 * SunshapePillbox : public TNode
 */
SunshapeBuie::SunshapeBuie()
:TSunshape(),
 m_thetaCS ( 0.0436 ),
 m_thetaSD( 0.00465 ),
 m_integralA (9.224724736098827/1000000.0 ),
 m_alpha( 0.0 ),
 m_deltaThetaCSSD( 0.0 ),
 m_etokTimes1000toGamma( 0.0 ),
 m_gamma( 0.0 ),
 m_heightRectangle1( 0.0 ),
 m_heightRectangle2( 0.0 ),
 m_probabilityRectangle1( 0.0 )
{

	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("irradiance"), 1000.0 );
	m_parametersList->Append( QLatin1String("csr"), 0.02 );

	m_deltaThetaCSSD = m_thetaCS - m_thetaSD;
	UpdateState( 0.02 );

}

/*!
 * Destructor.
 */
SunshapeBuie::~SunshapeBuie()
{

}

/*!
 * Creates a ray for the given sunshape model.
 */
void SunshapeBuie::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{

	double phi = gc::TwoPi * rand.RandomDouble();
    double theta = ZenithAngle( rand );
    double sinTheta = sin( theta );
    double cosTheta = cos( theta );
    double cosPhi = cos( phi );
    double sinPhi = sin( phi );

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

/*!
 * Returns irradiance value.
 */
double SunshapeBuie::GetIrradiance() const
{

	return ( m_parametersList->GetValue( QLatin1String("irradiance") ).toDouble() );
}

/*!
 * Returns the maximum angle that the models could generate rays.
 */
double SunshapeBuie::GetThetaMax() const
{

	return ( m_parametersList->GetValue( QLatin1String("thetaMax") ).toDouble() );
}

/*!
 * Returns node type.
 */
TNodeType SunshapeBuie::GetType() const
{
	return ( SunshapeBuie::m_nodeType );
}

/*!
 * Gives the probability density function evaluated at \a theta in radians.
 */
double SunshapeBuie::PDFTheta( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	return ( m_alpha * Phi( theta ) * sin( theta ) );
}

double SunshapeBuie::Phi( double theta  ) const
{
	// The parameter theta is the zenith angle in radians
	if ( theta < m_thetaSD ) return PhiSolarDisk( theta );
	else return PhiCircumSolarRegion( theta );
}

double SunshapeBuie::PhiCircumSolarRegion( double theta  ) const
{
	// The parameter theta is the zenith angle in radians
	return m_etokTimes1000toGamma * pow( theta, m_gamma );
}

double SunshapeBuie::PhiSolarDisk( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	return cos( 326 * theta ) / cos( 308 * theta );
}

/*!
 * Updates internal values.
 */
void SunshapeBuie::UpdateState( double csrValue )
{

    //m_chi = chiValue( csrValue );

    double chi = 0.0;
	if( csrValue > 0.035 )
		chi = 0.022652077593662934 + csrValue * ( 0.5252380349996234 + ( 2.5484334534423887 - 0.8763755326550412 * csrValue ) * csrValue );

	else if ( !( csrValue > 0.035 ) &&  ( csrValue > 0.145 ) )
		chi = -0.04419909985804843 + csrValue * ( 1.401323894233574 + csrValue * ( -0.3639746714505299 + csrValue * ( -0.9579768560161194 + 1.1550475450828657 * csrValue ) ) );
	else
		chi = 0.004733749294807862 + csrValue * (4.716738065192151 + csrValue * (-463.506669149804 + csrValue * ( 24745.88727411664+
				csrValue * (-606122.7511711778 + 5521693.445014727 * csrValue ) ) ) );

	//m_k = kValue( m_chi );
	double k = 0.9 * log( 13.5 * chi ) * pow( chi, -0.3 );

	//m_gamma = gammaValue( m_chi );
	m_gamma = 2.2 * log( 0.52 * chi ) * pow( chi, 0.43 ) - 0.1;
	m_etokTimes1000toGamma = exp( k ) * pow( 1000, m_gamma );

    //m_integralB = intregralB( m_k, m_gamma, m_thetaCS, m_thetaSD );
	double gammaPlusTwo = m_gamma + 2.0;
	double integralB = ( exp( k ) * pow( 1000, m_gamma ) / gammaPlusTwo ) * ( pow( m_thetaCS, gammaPlusTwo ) - pow( m_thetaSD, gammaPlusTwo ) );

	m_alpha = 1.0/( m_integralA + integralB );
	double theta = 0.0038915695846209047;
    m_heightRectangle1 = 1.001 * (  m_alpha * Phi( theta ) * sin( theta ) );

    m_heightRectangle2 = PDFTheta( m_thetaSD );

    //m_probabilityRectangle1 = probabilityRectangle1(  m_thetaSD, m_heightRectangle1, (m_thetaCS - m_thetaSD), m_heightRectangle2 );
	double areaR1 = m_thetaSD * m_heightRectangle1;
	double areaR2 = (m_thetaCS - m_thetaSD) * m_heightRectangle2;
	m_probabilityRectangle1 = ( areaR1 / ( areaR1 + areaR2 ) );

}

/*!
 * Gives the next zenith angle of the ray in radians for this ditribution function.
 */
double SunshapeBuie::ZenithAngle( RandomDeviate& rand ) const
{
	double theta;
	double value;

	do
	{
		if ( rand.RandomDouble() < m_probabilityRectangle1 )
		{
			theta = rand.RandomDouble() * m_thetaSD;
			value = rand.RandomDouble() * m_heightRectangle1;
		}
		else
		{
			theta = m_thetaSD + rand.RandomDouble() * m_deltaThetaCSSD;
			value = rand.RandomDouble() * m_heightRectangle2;
		}
	} while ( value > PDFTheta( theta ) );

	return theta;
}

