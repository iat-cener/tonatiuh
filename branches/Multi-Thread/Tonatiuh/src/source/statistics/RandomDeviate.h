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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez, 
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/
 
#ifndef RANDOMDEVIATE_H_
#define RANDOMDEVIATE_H_

//!  RandomDeviate is the base class for random generators.
/*!
  A random generator class can be written based on this class.
*/

class RandomDeviate
{
public:
	explicit RandomDeviate( const unsigned long arraySize = 1000000 );
    virtual ~RandomDeviate( );
    virtual void FillArray( double* array, const unsigned long arraySize )=0;
    unsigned long NumbersGenerated( ) const;
    unsigned long NumbersProvided( ) const;
    double RandomDouble( );
  
private:
     const unsigned long m_arraySize;
     double* m_randomNumber;
     unsigned long m_numbersGenerated;
     unsigned long m_nextRandomNumber;
};     

inline RandomDeviate::RandomDeviate( const unsigned long arraySize )
: m_arraySize(arraySize), m_randomNumber(0), m_numbersGenerated(0), m_nextRandomNumber(arraySize)
{
	m_randomNumber = new double[arraySize];
}

inline RandomDeviate::~RandomDeviate( )
{
	if( m_randomNumber ) delete [] m_randomNumber;
}

inline double RandomDeviate::RandomDouble( )
{
	if( m_nextRandomNumber >= m_arraySize  )
	{
		m_nextRandomNumber = 0;
		FillArray( m_randomNumber, m_arraySize );
		m_numbersGenerated += m_arraySize;
	}
	return m_randomNumber[m_nextRandomNumber++];
}

inline unsigned long RandomDeviate::NumbersGenerated( ) const
{
	return m_numbersGenerated;
}

inline unsigned long RandomDeviate::NumbersProvided( ) const
{
	return ( m_numbersGenerated > 0 )? m_numbersGenerated - m_arraySize + m_nextRandomNumber : 0;
}

#endif // RANDOMDEVIATE_H_
