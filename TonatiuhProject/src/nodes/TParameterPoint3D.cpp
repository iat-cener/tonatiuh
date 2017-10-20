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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "tf.h"

#include "TParameterPoint3D.h"

/*!
 * Creates an empty point 3D type object
 */
TParameterPoint3D::TParameterPoint3D()
:TParameter(),
 m_pointCoordinates( Point3D() )
{

}

/*!
 * Creates a point3D object with the point \a point.
 */
TParameterPoint3D::TParameterPoint3D( Point3D point )
:TParameter(),
 m_pointCoordinates( point )
{

}

/*!
 * Destroys the object.
 */
TParameterPoint3D::~TParameterPoint3D()
{

}

/*!
 * Returns the point stored in the parameter.
 */
Point3D TParameterPoint3D::GetPoint3D( ) const
{
	return ( m_pointCoordinates );
}

/*!
 * Sets the selected element to the element with the given name in \a value
 * Returns false if the name of the value is not in the current enumerator list.
 */
bool TParameterPoint3D::SetValue( QVariant value )
{

	/*
	std::string valueString = value.toString().toStdString();
	std::regex rgx( std::string(  "\\s+" ) ) ;

	std::sregex_token_iterator iter( valueString.begin(), valueString.end(), rgx, -1);
	std::sregex_token_iterator end;


	std::vector< double > pointCoordinatesValues;
	while( iter != end)
	{
		pointCoordinatesValues.push_back( stod( *iter ) );
		++iter;
	}
	*/

	std::vector< std::string > pointCoordinatesValues = tf::StringSplit( value.toString().toStdString(), "\\s+" );
	if( pointCoordinatesValues.size () != 3 )
		return ( false );

	m_pointCoordinates = Point3D( stod( pointCoordinatesValues[0] ), stod( pointCoordinatesValues[1] ), stod( pointCoordinatesValues[2] ) );
	return ( true );
}

/*!
 * Returns the name of the selected value.
 */
std::string TParameterPoint3D::ToString() const
{

	return (  std::to_string( m_pointCoordinates.x ) + std::string( ", " ) +
			std::to_string( m_pointCoordinates.y ) + ", " +
			std::to_string( m_pointCoordinates.z )  );
}
