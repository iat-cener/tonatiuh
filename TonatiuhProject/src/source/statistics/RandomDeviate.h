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
 
#ifndef RANDOMDEVIATE_H_
#define RANDOMDEVIATE_H_

//!  RandomDeviate is the base class for random generators.
/*!
  A random generator class can be written based on this class.
*/

class RandomDeviate
{
public:
	explicit RandomDeviate( const unsigned long arraySize = 100000 );
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
