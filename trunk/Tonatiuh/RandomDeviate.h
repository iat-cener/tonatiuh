/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#ifndef RANDOMDEVIATE_H_
#define RANDOMDEVIATE_H_

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
