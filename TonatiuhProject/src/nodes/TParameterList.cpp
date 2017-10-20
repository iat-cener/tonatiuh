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

#include "TParameter.h"
#include "TParameterList.h"

/*!
 * Constructor
 */
TParameterList::TParameterList()
{
}


/*!
 * Destroys TParameter object
 */
TParameterList::~TParameterList()
{

}

/*!
 * Appends the parameter defined with the \a key and its \a value to the list.
 */
void TParameterList::Append(const std::string& key, const QVariant& value, bool visibility )
{
	if( m_parametersList.find( key ) != m_parametersList.end() )
		return;

	m_parametersList[key] = value;
	m_parametersVisibilityList[key] = visibility;
}

/*!
 * Returns true if there is a parameter with the \a name in the list. Otherwise, returns false.
 */
bool TParameterList::Contains(const std::string& name) const
{
	if( m_parametersList.find( name ) != m_parametersList.end()  )
		return ( true );

    return ( false );
}

/*!
 * Returns the value of the parameter \a name. If a parameter with defined name is not defined in the list, an empty string is returned.
 */
QVariant TParameterList::GetValue(const std::string& name) const
{
  return ( GetValue(name, QVariant() ) );
}

/*!
 * Returns the value of the parameter \a name. If there is not a parameter with the name defined \a defaultValue is returned.
 */
QVariant TParameterList::GetValue(const std::string& name, const QVariant& defaultValue) const
{
	if( Contains(name) )
		return ( m_parametersList.at( name ) );
  else
    return ( defaultValue );
}

/*!
 * Returns if true if the parameter could be used externally to the node.
 * Otherwise return false.
 */
bool TParameterList::GetVisibility(const std::string& name) const
{
	if( Contains(name) )
		return ( m_parametersVisibilityList.at( name ) );
  else
    return ( false );

}

/*!
 * Returns a list with the name of the parameters.
 */
std::vector<std::string > TParameterList::GetParametersNames() const
{
	std::vector<std::string> keys_list;
	for( auto const& element: m_parametersList )
		keys_list.push_back( element.first );


	return ( keys_list );
}


/*!
 * Returns the number of parameters in the list.
 */
int TParameterList::NumberOfParameters() const
{
	return ( m_parametersList.size() );
}

/*!
 * Removes the parameter with the name \a name from the parameter list.
 * Return false if the list does not contains a parameter with defined name or the parameter cannot be removed.
 */
bool TParameterList::RemoveParameter( const std::string& name )
{
	if( !Contains( name ) )
			return ( false );

	if( ( m_parametersList.erase( name ) > 0 ) && ( m_parametersVisibilityList.erase( name ) ) )
			return ( true );
	return ( false );
}

/*!
 * Sets to the parameter \a name the \a value.
 */

bool TParameterList::SetValue(const std::string& name, const QVariant& value )
{
	if( !Contains(name) )	return ( false );


	int typeID = m_parametersList[name].type();
	if( QMetaType::User == typeID )
	{
		TParameter* variantType = m_parametersList[name].value<TParameter*>();
		if( !variantType || !variantType->SetValue( value ) ) return ( false );
	}
	else
		m_parametersList[name] = value;

	return ( true );
}
