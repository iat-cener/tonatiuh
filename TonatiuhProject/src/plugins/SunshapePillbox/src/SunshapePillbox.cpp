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

#include "RandomDeviate.h"
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
	SunshapePillbox::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Sunshape" ), "SunshapePillbox", &SunshapePillbox::CreateInstance );
}

/*!
 * SunshapePillbox : public TNode
 */
SunshapePillbox::SunshapePillbox()
:TSunshape(),
 m_irradianceLabel( "irradiance" ),
 m_thetaMaxLabel( "thetaMax" )
{
	SetName(GetType().GetName() );

	//Translation
	m_pParametersList->Append<double>( m_irradianceLabel, 1000.0, true );
	m_pParametersList->Append<double>( m_thetaMaxLabel, 0.00465, true  );
}

/*!
 * Destructor.
 */
SunshapePillbox::~SunshapePillbox()
{

}

/*!
 * Creates a copy of sunshape node.
 */
SunshapePillbox* SunshapePillbox::Copy() const
{
	SunshapePillbox* sunshapeNode = new SunshapePillbox;
	if( sunshapeNode == 0 )	return ( 0  );

	//Coping node parts.
	//NO parts

	//Coping the parameters.
	sunshapeNode->m_pParametersList->SetValue( m_irradianceLabel, GetParameterValue<double>( m_irradianceLabel ) );
	sunshapeNode->m_pParametersList->SetValue( m_thetaMaxLabel, GetParameterValue<double>( m_thetaMaxLabel ) );

	return ( sunshapeNode );
}

/*!
 * Creates a ray for the given sunshape model.
 */
void SunshapePillbox::GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const
{
	double thetaMax = GetParameterValue<double>( m_thetaMaxLabel );

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
 * Returns the filename that stores the shape icon.
 */
std::string SunshapePillbox::GetIcon() const
{
	return ( ":/icons/SunshapePillbox.png" );
}

/*!
 * Returns irradiance value.
 */
double SunshapePillbox::GetIrradiance() const
{

	return ( GetParameterValue<double>( m_irradianceLabel ) );
}

/*!
 * Returns the maximum angle that the models could generate rays.
 */
double SunshapePillbox::GetThetaMax() const
{

	return ( GetParameterValue<double>( m_thetaMaxLabel ) );
}

/*!
 * Returns node type.
 */
TNodeType SunshapePillbox::GetType() const
{
	return ( SunshapePillbox::m_nodeType );
}

