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
 
#include "MersenneTwister.h"

unsigned long MersenneTwister::RandomUInt()
{
	return RandomInteger();
}

void MersenneTwister::GenerateNewState()
{
  for ( int i = 0; i < ( N - M ); ++i ) m_state[i] = m_state[i + M] ^ Twiddle( m_state[i], m_state[i + 1] );
  for ( int i = N - M; i < (N - 1); ++i ) m_state[i] = m_state[i + M - N] ^ Twiddle( m_state[i], m_state[i + 1] );
  m_state[N - 1] = m_state[ M - 1 ] ^ Twiddle( m_state[N - 1], m_state[0] );
  m_p = 0; // reset position
}

void MersenneTwister::Seed( unsigned long seedValue )
{
    m_state[0] = seedValue & 0xFFFFFFFFUL; // for > 32 bit machines
    for ( int i = 1; i < N; ++i )
    {
        m_state[i] = 1812433253UL * ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) + i;
        m_state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
  }
  m_p = N; // force GenerateNewState() to be called for next random number
}

void MersenneTwister::Seed( const unsigned long* seedArray, int arraySize )
{
    Seed( 19650218UL );
    int i = 1;
    int j = 0;
    for ( int k = ( ( N > arraySize ) ? N : arraySize ); k; --k )
    {
        m_state[i] = ( m_state[i] ^ ( ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) * 1664525UL ) ) 
                     + seedArray[j] + j; // non linear
        m_state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
        ++j; j %= arraySize;
        if ( ( ++i ) == N ) 
        { 
            m_state[0] = m_state[N - 1];
            i = 1;
        }
    }
    for ( int k = N - 1; k; --k )
    {
        m_state[i] = ( m_state[i] ^ ( ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) * 1566083941UL ) ) - i;
        m_state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
        if ( (++i) == N ) 
        { 
            m_state[0] = m_state[N - 1];
      	    i = 1;
        }
    }
    m_state[0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array
    m_p = N; // force GenerateNewState() to be called for next random number
}
