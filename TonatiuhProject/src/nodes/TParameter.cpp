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

//#include <iostream>
#include <sstream>

#include "nf.h"
#include "TParameter.h"

/*!
 * Creates a TParameter object.
 */
TParameter::TParameter()
{
	m_connectedFunction = 0;
}

TParameter::TParameter(const TParameter& parameter)

{
	m_variant = parameter.m_variant;
	m_connectedFunction = parameter.m_connectedFunction;
}

/*
 * Destroys TParameter object.
 */
TParameter::~TParameter()
{

}

/*!
 * Sets to the parameter the value in \a value stored as a string.
 */
bool TParameter::FromString( const std::string& value )
{
	//string
	if( m_variant.value().index() == 0 )
	{
		m_variant = value;
	}
	//int
	else if( m_variant.value().index() == 1 )
	{
		int iValue;
		std::stringstream{ value } >> iValue;
		m_variant = iValue;
	}
	//double
	//stod depends of locale
	else if( m_variant.value().index() == 2 )
	{
		double dValue;
		std::stringstream{ value } >> dValue;
		m_variant = dValue;
	}
	//bool
	else if( m_variant.value().index() == 3 )
	{
		bool bValue;
		std::stringstream{ value } >> bValue;
		m_variant = bValue;
	}
	//Point3D
	else if( m_variant.value().index() == 4 )
	{
		std::vector< std::string > pointCoordinatesValues = nf::StringSplit( value, "\\s+" );
		if( pointCoordinatesValues.size () != 3 )
			return ( false );


		double xValue, yValue, zValue;
		std::stringstream{ pointCoordinatesValues[0] } >> xValue;
		std::stringstream{ pointCoordinatesValues[1] } >> yValue;
		std::stringstream{ pointCoordinatesValues[2] } >> zValue;

		m_variant =  Point3D( xValue, yValue, zValue );

	}
	//EnumeratedTypes
	else if( m_variant.value().index() == 5 )
	{
		EnumeratedTypes enumType = std::get<EnumeratedTypes>(m_variant.value());
		enumType.SetValue( value );
		m_variant = enumType;
	}
	else
		return ( false );


	if( m_connectedFunction != 0  )
		m_connectedFunction();
	return ( true );
}


/*!
 * Returns the parameter value as a variant.
 */
tonatiuh_variant TParameter::GetValue() const
{
	return ( m_variant );
}

/*!
 * Sets the parameter value to \a value. This value could be of different types.
 */
bool TParameter::SetValue( const tonatiuh_variant& value )
{
	if( ( m_variant != std::nullopt )  &&  ( m_variant.value().index() != value.value().index() ) )
	{
		return ( false );
	}

	tonatiuh_variant old_value = m_variant;
	m_variant = value;
	if( m_connectedFunction != 0  )
		if( !m_connectedFunction( ) )
		{
			m_variant = old_value ;
			return ( false );
		}

	return ( true );
}


void TParameter::SetConnectedFuntion( std::function<bool()> function )
{
	m_connectedFunction = function;
}

/*!
 * Returns parameter value in a string.
 */
std::string TParameter::ToString() const
{
	if(  m_variant == std::nullopt )
		return ( std::string() );


	//string
	if( m_variant.value().index() == 0 )
	{
		return ( std::get<std::string>( m_variant.value() ) );
	}

	//int
	else if( m_variant.value().index() == 1 )
	{

		std::ostringstream int_convert;
		int_convert << std::get<int>(m_variant.value());
		return ( int_convert.str() );
	}

	//double
	else if( m_variant.value().index() == 2 )
	{
		std::ostringstream double_convert;
		double_convert << std::get<double>(m_variant.value());
		return ( double_convert.str() );
	}

	//bool
	else if( m_variant.value().index() == 3 )
	{
		std::ostringstream double_convert;
		double_convert << std::get<bool>(m_variant.value());
		return ( double_convert.str() );
	}

	//Point3D
	else if( m_variant.value().index() == 4 )
	{
		Point3D pointCoordinates = std::get<double>(m_variant.value());
		std::stringstream ss;
		ss<<pointCoordinates.x ;
		ss<<", ";
		ss<<pointCoordinates.y ;
		ss<<", ";
		ss<<pointCoordinates.z ;
		return ( ss.str() );
	}
	//EnumeratedTypes
	else if( m_variant.value().index() == 5 )
	{
		EnumeratedTypes enumType = std::get<EnumeratedTypes>(m_variant.value());
		return ( enumType.GetSelectedName() );
	}

	return ( std::string() );

}
