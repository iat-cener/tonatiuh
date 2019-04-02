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
***************************************************************************/

#include <Inventor/sensors/SoFieldSensor.h>

#include "gc.h"

#include "SunshapeBuie.h"

const double SunshapeBuie::m_minCRSValue = 0.000001;
const double SunshapeBuie::m_maxCRSValue = 0.849;


SO_NODE_SOURCE(SunshapeBuie);

void SunshapeBuie::initClass()
{
	SO_NODE_INIT_CLASS(SunshapeBuie, TSunShape, "TSunShape");
}

SunshapeBuie::SunshapeBuie( )
{
	SO_NODE_CONSTRUCTOR( SunshapeBuie );
	SO_NODE_ADD_FIELD( irradiance, ( 1000 ) );
	SO_NODE_ADD_FIELD( csr, ( 0.02f ) );

	m_csrSensor = new SoFieldSensor( updateCSR, this );
	m_csrSensor->setPriority( 1 );
	m_csrSensor->attach( &csr );

	double csrValue = csr.getValue();
	if( csrValue >= m_minCRSValue && csrValue <= m_maxCRSValue ) updateState( csrValue );
}

void SunshapeBuie::updateState( double csrValue )
{
    m_thetaSD = 0.00465;
    m_thetaCS = 0.0436;
    m_deltaThetaCSSD = m_thetaCS - m_thetaSD;
    m_integralA = 9.224724736098827/1000000.0;
	m_chi = chiValue( csrValue );
	m_k = kValue( m_chi );
	m_gamma = gammaValue( m_chi );
	m_etokTimes1000toGamma = exp( m_k ) * pow( 1000, m_gamma );
    m_integralB = intregralB( m_k, m_gamma, m_thetaCS, m_thetaSD );
    m_alpha = 1.0/( m_integralA + m_integralB );
    m_heightRectangle1 = 1.001 * pdfTheta( 0.0038915695846209047 );
    m_heightRectangle2 = pdfTheta( m_thetaSD );
    m_probabilityRectangle1 = probabilityRectangle1(  m_thetaSD, m_heightRectangle1, (m_thetaCS - m_thetaSD), m_heightRectangle2 );
}

SunshapeBuie::~SunshapeBuie()
{
	delete m_csrSensor;
}

//Light Interface
void SunshapeBuie::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double phi = gc::TwoPi * rand.RandomDouble();
    double theta = zenithAngle( rand );
    double sinTheta = sin( theta );
    double cosTheta = cos( theta );
    double cosPhi = cos( phi );
    double sinPhi = sin( phi );

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

double SunshapeBuie::GetIrradiance( void ) const
{
	return irradiance.getValue();
}

double SunshapeBuie::GetThetaMax() const
{
	return m_thetaCS;
}

SoNode* SunshapeBuie::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	SunshapeBuie* newSunShape = dynamic_cast< SunshapeBuie* >( SoNode::copy( copyConnections ) );

	newSunShape->irradiance = irradiance;
	newSunShape->csr = csr;

	newSunShape->m_chi = m_chi;
	newSunShape->m_k = m_k;
	newSunShape->m_gamma = m_gamma;
	newSunShape->m_etokTimes1000toGamma = m_etokTimes1000toGamma;
	newSunShape->m_thetaSD = m_thetaSD;
	newSunShape->m_thetaCS = m_thetaCS;
	newSunShape->m_deltaThetaCSSD = m_deltaThetaCSSD;
	newSunShape->m_integralA = m_integralA;
	newSunShape->m_integralB = m_integralB;
	newSunShape->m_alpha = m_alpha;
	newSunShape->m_heightRectangle1 = m_heightRectangle1;
	newSunShape->m_heightRectangle2 = m_heightRectangle2;
	newSunShape->m_probabilityRectangle1 = m_probabilityRectangle1;

	return newSunShape;
}

void SunshapeBuie::updateCSR(void *data, SoSensor *)
{
	SunshapeBuie* sunshape = ( SunshapeBuie* ) data;
	double csrValue = sunshape->csr.getValue();
	if( csrValue >= m_minCRSValue && csrValue <= m_maxCRSValue ) sunshape->updateState( csrValue );
}

double SunshapeBuie::zenithAngle( RandomDeviate& rand ) const
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
	} while ( value > pdfTheta( theta ) );

	return theta;
}

double SunshapeBuie::chiValue( double csr ) const
{
	if( csr > 0.145 )
		return -0.04419909985804843 + csr * ( 1.401323894233574 + csr * ( -0.3639746714505299 + csr * ( -0.9579768560161194 + 1.1550475450828657 * csr ) ) );

	if( csr > 0.035 )
		return 0.022652077593662934 + csr * ( 0.5252380349996234 + ( 2.5484334534423887 - 0.8763755326550412 * csr ) * csr );

	return 0.004733749294807862 + csr * (4.716738065192151 + csr * (-463.506669149804 + csr * ( 24745.88727411664+
	          csr * (-606122.7511711778 + 5521693.445014727 * csr ) ) ) );

	/*return -4.7172422329669 * (-1.4070438092190156 + csr) * (0.0313462488977661 + csr) * (0.08846465256738223 + csr) *
			( 0.7843687910540035 + (-1.2947513603654814 + csr ) * csr );
			*/
}

double SunshapeBuie::phiSolarDisk( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	return cos( 326 * theta ) / cos( 308 * theta );
}

double SunshapeBuie::phiCircumSolarRegion( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	return m_etokTimes1000toGamma * pow( theta, m_gamma );
}

double SunshapeBuie::phi( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	if ( theta < m_thetaSD ) return phiSolarDisk( theta );
	else return phiCircumSolarRegion( theta );
}

double SunshapeBuie::pdfTheta( double theta ) const
{
	// The parameter theta is the zenith angle in radians
	return m_alpha * phi( theta ) * sin( theta );
}

double SunshapeBuie::kValue( double chi ) const
{
	return 0.9 * log( 13.5 * chi ) * pow( chi, -0.3 );
}

double SunshapeBuie::gammaValue( double chi ) const
{
	return 2.2 * log( 0.52 * chi ) * pow( chi, 0.43 ) - 0.1;
}

double SunshapeBuie::intregralB( double k, double gamma, double thetaCS, double thetaSD ) const
{
	double gammaPlusTwo = gamma + 2.0;
	return ( exp( k ) * pow( 1000, gamma ) / gammaPlusTwo ) * ( pow( thetaCS, gammaPlusTwo ) - pow( thetaSD, gammaPlusTwo ) );
}

double SunshapeBuie::probabilityRectangle1( double widthR1, double heightR1, double widthR2, double heightR2 ) const
{
	double areaR1 = widthR1 * heightR1;
	double areaR2 = widthR2 * heightR2;
	return areaR1 / ( areaR1 + areaR2 );
}
