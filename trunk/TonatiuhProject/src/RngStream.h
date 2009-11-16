
#ifndef RNGSTREAM_H
#define RNGSTREAM_H

#include <string>

#include "RandomDeviate.h"

class RngStream : public RandomDeviate
{
public:
	RngStream ( const unsigned long arraySize = 1000000 );
	void FillArray( double* array, const unsigned long arraySize );

private:
	static bool SetPackageSeed (const unsigned long seed[6]);
	void ResetStartStream ();
	void ResetStartSubstream ();
	void ResetNextSubstream ();
	void SetAntithetic (bool a);
	void IncreasedPrecis (bool incp);
	bool SetSeed (const unsigned long seed[6]);
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

inline void RngStream::FillArray( double* array, const unsigned long arraySize )
{
    for( unsigned long i = 0; i < arraySize; i++ ) array[i] = RandU01 ();
}

#endif


