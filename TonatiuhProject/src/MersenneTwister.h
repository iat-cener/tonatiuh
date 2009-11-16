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
 
#ifndef MERSENNETWISTER_H_
#define MERSENNETWISTER_H_

#include "RandomDeviate.h"

const double LongIntegerToDouble = 1.0 / 4294967296.0;

class MersenneTwister : public RandomDeviate
{
	
public:
    MersenneTwister( unsigned long seedValue = 5489UL, long int randomNumberArraySize = 1000000 );
    MersenneTwister( const unsigned long* seedArray, int seedArraySize, long int randomNumberArraySize = 1000000 );
    virtual ~MersenneTwister( );
    void FillArray( double* array, const unsigned long arraySize ); 
    unsigned long RandomUInt();
 
private:
   enum { N = 624, M = 397 };
   
   unsigned long m_state[N];
   int m_p;
   bool m_init; 

   void Seed( unsigned long seedValue ); 
   void Seed( const unsigned long* seedArray, int arraySize);
   unsigned long int RandomInteger( );
   double Random01( );
   unsigned long Twiddle( unsigned long u, unsigned long v );
   void GenerateNewState();
  
   MersenneTwister( const MersenneTwister& );
   void operator=( const MersenneTwister& );
   
};

inline MersenneTwister::MersenneTwister( unsigned long seedValue, long int randomNumberArraySize )
: RandomDeviate( randomNumberArraySize ), m_p(0)
{
	Seed( seedValue ); 
    m_init = true;
}

inline MersenneTwister::MersenneTwister( const unsigned long* seedArray, int seedArraySize, long int randomNumberArraySize  )
: RandomDeviate( randomNumberArraySize ), m_p(0)
{
    Seed( seedArray, seedArraySize );
    m_init = true;	
}

inline MersenneTwister::~MersenneTwister( )
{
}

inline void MersenneTwister::FillArray( double* array, const unsigned long arraySize )
{
    for( unsigned int i = 0; i < arraySize; i++ ) array[i] = Random01( );
}

inline unsigned long MersenneTwister::Twiddle( unsigned long u, unsigned long v )
{
    return ( ( ( u & 0x80000000UL ) | ( v & 0x7FFFFFFFUL ) ) >> 1 ) 
           ^ ( ( v & 1UL) ? 0x9908B0DFUL : 0x0UL );
}

inline unsigned long int MersenneTwister::RandomInteger( )
{
  if ( m_p == N ) GenerateNewState(); // new state vector needed
  unsigned long x = m_state[m_p++];
  x ^= (x >> 11);
  x ^= (x << 7) & 0x9D2C5680UL;
  x ^= (x << 15) & 0xEFC60000UL;
  return x ^ (x >> 18);
}

inline double MersenneTwister::Random01( )
{
	return ( static_cast<double>( RandomInteger() ) + 0.5) * LongIntegerToDouble; // divided by 2^32
}

#endif
