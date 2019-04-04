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


#ifndef RANDOMRNGSTREAM_H
#define RANDOMRNGSTREAM_H

#include <string>

#include "RandomDeviate.h"

class RandomRngStream : public RandomDeviate
{
public:
	RandomRngStream ( unsigned long seedValue = 5489UL, const unsigned long arraySize = 1000000 );
	~RandomRngStream();
	void FillArray( double* array, const unsigned long arraySize );

private:
	static bool SetPackageSeed( const unsigned long seed[6] ) ;
	void ResetStartStream ();
	void ResetStartSubstream ();
	void ResetNextSubstream ();
	void SetAntithetic (bool a);
	void IncreasedPrecis (bool incp);
	bool SetSeed (const unsigned long seed[6] );
	void AdvanceState (long e, long c);
	void GetState (unsigned long seed[6]) const;
	double RandU01 ();
	int RandInt (int i, int j);
	double U01 ();
	double U01d ();

private:
	double m_cg[6], m_bg[6], m_ig[6];
	bool m_anti, m_incPrec;
	static double sm_nextSeed[6];


};

inline void RandomRngStream::FillArray( double* array, const unsigned long arraySize )
{
    for( unsigned long i = 0; i < arraySize; i++ ) array[i] = RandU01 ();
}

#endif /*RANDOMRNGSTREAM_H*/


