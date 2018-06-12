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

#ifndef TRANSMISSIVITYFATMPARAMETERS_H_
#define TRANSMISSIVITYFATMPARAMETERS_H_


#include "TTransmissivity.h"
#include "TNodeType.h"


//!  TransmissivityATMParameters is the class for transmivity model defined by polynomial equation of degree 4.
/*!
  TransmissivityATMParameters class provides a transmissivity functionality taking into account a polynomial equation of degree 4 .
  */


class TransmissivityATMParameters : public TTransmissivity
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	TransmissivityATMParameters(const TransmissivityATMParameters& ) = delete;
	TransmissivityATMParameters& operator=(const TransmissivityATMParameters&) = delete;

public:
	static std::shared_ptr< TNode > CreateInstance();
	static void Init();

	virtual std::shared_ptr< TNode > Copy() const;

	std::string GetIcon() const;
	virtual TNodeType GetType() const;
	bool IsTransmitted( double distance, RandomDeviate& rand ) const;

protected:
	TransmissivityATMParameters();
    virtual ~TransmissivityATMParameters();

private:
	static TNodeType m_nodeType;
	std::string m_atm1Label;
	std::string m_atm2Label;
	std::string m_atm3Label;
	std::string m_atm4Label;

};

#endif /* TRANSMISSIVITYFATMPARAMETERS_H_ */
