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

#ifndef TPARAMETER_H_
#define TPARAMETER_H_

#include <functional>
#include <string>


#include <optional>
#include <variant>


#include "EnumeratedTypes.h"
#include "NodeLibrary.h"
#include "Point3D.h"


//!  TParameter class is the base class for all Tonatiuh parameter types.
/*!
 * TParameter is the abstraction of the new types of parameters. All the new types of parameters in Tonatiuh must be inherited by this class.
*/

/** Base class of all Tonatiuh parameters: TParameter*/


typedef std::optional<std::variant<std::string, int, double, Point3D, EnumeratedTypes> > tonatiuh_variant;

class NODE_API TParameter
{

public:
	TParameter();
	TParameter(const TParameter& parameter);
	virtual ~TParameter();

	bool FromString( const std::string& value );
	tonatiuh_variant GetValue() const;

	bool SetValue( const tonatiuh_variant& value );
	void SetConnectedFuntion( std::function<bool()> function );
	std::string ToString() const;

private:
	tonatiuh_variant m_variant;
	std::function<bool()> m_connectedFunction;
};


#endif /* TPARAMETER_H_ */
