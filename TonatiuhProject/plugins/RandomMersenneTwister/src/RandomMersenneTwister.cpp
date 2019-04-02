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
 
#include "RandomMersenneTwister.h"

unsigned long RandomMersenneTwister::RandomUInt()
{
	return RandomInteger();
}

void RandomMersenneTwister::GenerateNewState()
{
  for ( int i = 0; i < ( N - M ); ++i ) m_state[i] = m_state[i + M] ^ Twiddle( m_state[i], m_state[i + 1] );
  for ( int i = N - M; i < (N - 1); ++i ) m_state[i] = m_state[i + M - N] ^ Twiddle( m_state[i], m_state[i + 1] );
  m_state[N - 1] = m_state[ M - 1 ] ^ Twiddle( m_state[N - 1], m_state[0] );
  m_p = 0; // reset position
}

void RandomMersenneTwister::Seed( unsigned long seedValue )
{
    m_state[0] = seedValue & 0xFFFFFFFFUL; // for > 32 bit machines
    for ( int i = 1; i < N; ++i )
    {
        m_state[i] = 1812433253UL * ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) + i;
        m_state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
  }
  m_p = N; // force GenerateNewState() to be called for next random number
}

void RandomMersenneTwister::Seed( const unsigned long* seedArray, int arraySize )
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


