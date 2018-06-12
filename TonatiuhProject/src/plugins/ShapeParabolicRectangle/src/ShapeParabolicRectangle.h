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

#ifndef SHAPEPARABOLICRECTANGLE_H_
#define SHAPEPARABOLICRECTANGLE_H_

#include "TNodeType.h"
#include "TShape.h"


//!  ShapeParabolicRectangle is the class to represent a surface of rectangular paraboloid geometry.
/*!
  ShapeParabolicRectangle class provides the methods for a rectangular parabolic surface. The parameters to define the surface are:
  * - focusLength: the focal length of the parabola.
  * - widthX: length on x axis of the rectangle.
  * - widthZ: length on x azis of the rectangle.
  *
  * The parabola is defined in the y-axis.
*/

class ShapeParabolicRectangle : public TShape
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	ShapeParabolicRectangle(const ShapeParabolicRectangle&) = delete;
	ShapeParabolicRectangle& operator=(const ShapeParabolicRectangle&) = delete;

public:
	static std::shared_ptr< TNode > CreateInstance();
	static void Init();

	std::shared_ptr< TNode > Copy() const;

	BBox GetBondingBox() const;
	std::string GetIcon() const;
	TNodeType GetType() const;
	bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg, bool* isShapeFront ) const;
	void Draw() const;

protected:
	ShapeParabolicRectangle( );
	~ShapeParabolicRectangle();

private:
	static TNodeType m_nodeType;
	std::string m_focusLengthLabel;
	std::string m_widthXLabel;
	std::string m_widthZLabel;

/*
protected:
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;
	*/


};

#endif /*RECTANGULARPARABOLICFACET_H_*/
