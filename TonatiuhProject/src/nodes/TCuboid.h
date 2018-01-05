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

#ifndef TCUBOID_H_
#define TCUBOID_H_

#include <memory>

#include "NodeLibrary.h"

#include "TShape.h"

class DifferentialGeometry;

//!  TCuboid class is the class to describe cuboid geometry.
/*!
*  TCuboid class is the a cuboid geometry aligned with global coordinates system which is composed with six rectangular faces. The centre of the cube is in the (0,0,0) coordinates and the dimensions are defined in the following:
* 	- width: in the x axis
* 	- height: in the y axis
* 	- depth: in the z axis
*/

class NODE_API TCuboid : public TShape
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	TCuboid(const TCuboid&) = delete;
	TCuboid& operator=(const TCuboid&) = delete;

public:
	static void* CreateInstance();
	static void Init();

	//std::shared_ptr<TCuboid> CopyCuboid() const;
	TCuboid* Copy() const;

	std::string GetIcon() const;
	BBox GetBondingBox() const;
	TNodeType GetType() const;
	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const;
	void Draw() const;

protected:
	TCuboid();
	~TCuboid() noexcept;
	//std::shared_ptr<TNode> CreateCopy() const final override;


private:
	static TNodeType m_nodeType;
	std::string m_depthLabel;
	std::string m_heightLabel;
	std::string m_widthLabel;
};

#endif /*TCUBOID_H_*/

