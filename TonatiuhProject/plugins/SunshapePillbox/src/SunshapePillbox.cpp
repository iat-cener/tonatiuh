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

#include "gc.h"

#include "SunshapePillbox.h"

SO_NODE_SOURCE(SunshapePillbox);

void SunshapePillbox::initClass()
{
	SO_NODE_INIT_CLASS(SunshapePillbox, TSunShape, "TSunShape");
}

SunshapePillbox::SunshapePillbox( )
{
	SO_NODE_CONSTRUCTOR( SunshapePillbox );
	SO_NODE_ADD_FIELD( irradiance, ( 1000.0 ) );
	SO_NODE_ADD_FIELD( thetaMax, (0.00465));

}

SunshapePillbox::~SunshapePillbox()
{
}

//Light Interface
void SunshapePillbox::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double phi = gc::TwoPi * rand.RandomDouble();
    double theta = asin( sin( thetaMax.getValue() )*sqrt( rand.RandomDouble() ) );
    double sinTheta = sin( theta );
    double cosTheta = cos( theta );
    double cosPhi = cos( phi );
    double sinPhi = sin( phi );

    direction.x = sinTheta*sinPhi;
    direction.y = -cosTheta;
    direction.z = sinTheta*cosPhi;
}

double SunshapePillbox::GetIrradiance( void ) const
{
	return irradiance.getValue();
}

double SunshapePillbox::GetThetaMax() const
{
	return thetaMax.getValue();
}

SoNode* SunshapePillbox::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	SunshapePillbox* newSunShape = dynamic_cast< SunshapePillbox* >( SoNode::copy( copyConnections ) );

	// Copy the m_thetaMin, m_thetaMax private members explicitly
	newSunShape->irradiance = irradiance;
	newSunShape->thetaMax = thetaMax;

	return newSunShape;
}
