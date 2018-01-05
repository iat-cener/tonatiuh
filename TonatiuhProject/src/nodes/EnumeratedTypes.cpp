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

#include "EnumeratedTypes.h"


#include <algorithm>


/*!
 * Creates an empty enumerator type
 */
EnumeratedTypes::EnumeratedTypes()
:m_selectedIndex( -1 )
{

}

/*!
 * Destroys the enumerator object.
 */
EnumeratedTypes::~EnumeratedTypes()
{
	m_nameList.clear();
	m_selectedIndex = -1;
}

/*
EnumeratedTypes& EnumeratedTypes::operator=(const EnumeratedTypes& other )
{
    if (this != &other) // protect against invalid self-assignment
    {
    	/*
    	 * // 1: allocate new memory and copy the elements
         int * new_array = new int[other.count];
         std::copy(other.array, other.array + other.count, new_array);

         // 2: deallocate old memory
         delete [] array;

         // 3: assign the new memory to the object
         array = new_array;
         count = other.count;

     }
     // by convention, always return *this
     return *this;
}
*/

/*!
 * Adds a new element to the enumerator list with the given \a name.
 * If \a isDefault is true, the current element with be the selected element by defualt.
 */
void EnumeratedTypes::AddValue( std::string name, bool isDefault )
{

	if( std::find( m_nameList.begin(), m_nameList.end(), name  ) == m_nameList.end() )
		m_nameList.push_back( name );

	if( isDefault )
		m_selectedIndex = distance( m_nameList.begin(), std::find( m_nameList.begin(), m_nameList.end(), name  ) );

}

/*!
 * Gives the name of the selected element.
 */
std::string EnumeratedTypes::GetSelectedName() const
{
	return ( m_nameList[m_selectedIndex] );
}

/*!
 * Sets the selected element to the element with the given name in \a value
 * Returns false if the name of the value is not in the current enumerator list.
 */
bool EnumeratedTypes::SetValue( const std::string& value )
{
	std::vector<std::string>::iterator it = std::find( m_nameList.begin(), m_nameList.end(), value  );
	if( it == m_nameList.end() )
		return ( false );


	m_selectedIndex = distance( m_nameList.begin(),it );
	return ( true );
}

