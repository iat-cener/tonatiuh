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

#include "SunshapePillbox.h"
#include "TParameterList.h"


TNodeType SunshapePillbox::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* SunshapePillbox::CreateInstance( )
{
  return ( new SunshapePillbox() );
}


/*!
 * Initializes ShapeFlatDisk type.
 */
void SunshapePillbox::Init()
{
	SunshapePillbox::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Sunshape" ), QString( "SunshapePillbox" ), &SunshapePillbox::CreateInstance );
}

/*!
 * SunshapePillbox : public TNode
 */
SunshapePillbox::SunshapePillbox()
:TSunshape()
{
	setObjectName(GetType().GetName());

	//Translation
	m_parametersList->Append( QLatin1String("irradiance"), 1000.0 );
	m_parametersList->Append( QLatin1String("thetaMax"), 0.00465 );
}

/*!
 * Destructor.
 */
SunshapePillbox::~SunshapePillbox()
{

}

/*!
 * Creates a ray for the given sunshape model.
 */
void SunshapePillbox::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double thetaMax = m_parametersList->GetValue( QLatin1String("thetaMax") ).toDouble();

	double phi = gc::TwoPi * rand.RandomDouble();
    double theta = asin( sin( thetaMax )*sqrt( rand.RandomDouble() ) );
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
double SunshapePillbox::GetIrradiance() const
{

	return ( m_parametersList->GetValue( QLatin1String("irradiance") ).toDouble() );
}

/*!
 * Returns the maximum angle that the models could generate rays.
 */
double SunshapePillbox::GetThetaMax() const
{

	return ( m_parametersList->GetValue( QLatin1String("thetaMax") ).toDouble() );
}

/*!
 * Returns node type.
 */
TNodeType SunshapePillbox::GetType() const
{
	return ( SunshapePillbox::m_nodeType );
}

