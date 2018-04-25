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


#ifndef TSUNNODE_H_
#define TSUNNODE_H_


#include "NodeLibrary.h"
#include "TContainerNode.h"
#include "TNodeType.h"


//!  TSunNode is a container class to store the information that defines a scene light.
/*!
  TSunNode class provides a structure to store sun shape model and the parameters to define light position.
  -->   "sunshape"

  In addition, this node includes two parameters:
  - azimuth: sun position azimuth angle in radians. Positive from North towards East.
  - zenith: sun position zenith angle in radians. Complementary angle of the sun elevation (measured from the horizon). Positive from the zenith.
*/

class NODE_API TSunNode : public TContainerNode
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	TSunNode(const TSunNode& ) = delete;
	TSunNode& operator=(const TSunNode&) = delete;


public:
	static std::shared_ptr< TNode > CreateInstance();
	static void Init();

	std::shared_ptr< TNode > Copy() const;

	void ChangeSunPosition( double azimuth, double zenith );

	double GetAzimuth() const;
	virtual std::string GetIcon() const;
	double GetZenith() const;
	TNodeType GetType() const;


protected:
	TSunNode();
	virtual ~TSunNode();

private:
	static TNodeType m_nodeType;
	std::string m_azimuthLabel;
	std::string m_zenithLabel;
};

//Q_DECLARE_METATYPE(TSunNode*)

#endif /* TSUNNODE_H_ */
