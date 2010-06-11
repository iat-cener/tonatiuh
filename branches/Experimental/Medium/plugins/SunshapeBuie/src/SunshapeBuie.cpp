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

#include <Inventor/sensors/SoFieldSensor.h>

#include "SunshapeBuie.h"
#include "tgc.h"

SO_NODE_SOURCE(SunshapeBuie);

void SunshapeBuie::initClass()
{
	SO_NODE_INIT_CLASS(SunshapeBuie, TSunShape, "TSunShape");
}

SunshapeBuie::SunshapeBuie( )
{
	SO_NODE_CONSTRUCTOR( SunshapeBuie );

	SO_NODE_ADD_FIELD( irradiance, (1000));
	SO_NODE_ADD_FIELD( crs, ( 0.01 ) );

	SoFieldSensor* irradianceSensor = new SoFieldSensor( updateCRS, this );
	irradianceSensor->attach( &irradiance );

	SoFieldSensor* crsSensor = new SoFieldSensor( updateCRS, this );
	crsSensor->attach( &crs );

	k = 0.9 * log( 13.5 * crs.getValue() ) * pow( crs.getValue(), -0.3 );
	y = 2.2 * log( 0.52 * crs.getValue() ) * pow( crs.getValue(), 0.43 )  - 0.1;

	// Integrate[ Cos[0.326 *theta]/Cos[0.308 *theta]*Sin[theta/1000], {theta, 0, 4.65}] = 9.22474
	cte = 9.22474 + ( exp( k )/( 2 + y ) ) * ( pow ( 43.6, y+2 ) - pow( 4.65, y +2 ) );

}

SunshapeBuie::~SunshapeBuie()
{
}

//Light Interface
void SunshapeBuie::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double phi = tgc::TwoPi * rand.RandomDouble();
    double theta = Theta( rand );
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

SoNode* SunshapeBuie::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	SunshapeBuie* newSunShape = dynamic_cast< SunshapeBuie* >( SoNode::copy( copyConnections ) );

	newSunShape->irradiance = irradiance;
	newSunShape->crs = crs;

	return newSunShape;
}


void SunshapeBuie::updateCRS(void *data, SoSensor *)
{
	SunshapeBuie* sunshape = ( SunshapeBuie* ) data;
	double crs0 = sunshape->crs.getValue();

	sunshape->k = 0.9 * log( 13.5 * crs0 ) * pow( crs0, -0.3 );
	sunshape->y = 2.2 * log( 0.52 * crs0 ) * pow( crs0, 0.43 )  - 0.1;
	// Integrate[ Cos[0.326 *theta]/Cos[0.308 *theta]*Sin[theta/1000], {theta, 0, 4.65}] = 9.22474

	sunshape->cte = 9.22474 + ( exp( sunshape->k )/(2 + sunshape->y ) ) * ( pow ( 43.6, sunshape->y+2 ) - pow( 4.65, sunshape->y +2 ) );

}

double SunshapeBuie::Theta( RandomDeviate& rand ) const
{
	double a = 0.0;
	double b = 43.6;

	double m = 0.0004;
	double w =- tgc::Infinity;


	bool cont = true;
	while (cont)
	{
		double theta = ( ( b - a ) * rand.RandomDouble() ) + a;
		double f = m * rand.RandomDouble();

		if( f <= ThetaProbiblityFunction( theta ) )
		{
			w = theta;
			cont  = false;
		}
	}
	return w/1000;
}

double SunshapeBuie::ThetaProbiblityFunction( double theta ) const
{

	double phi;
	if( theta < 4.65 )
		phi = cos( 0.326 * theta )/ cos( 0.308 * theta);
	else
	{
		phi =  exp( k ) * pow( theta, y );
	}

	return ( phi * sin( theta/1000 ) * cte );

}
