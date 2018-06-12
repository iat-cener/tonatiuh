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
along with this program.  If
	BBox GetBBox() const;not, see <http://www.gnu.org/licenses/>.


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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef SHAPESPHERICALPOLYGON_H_
#define SHAPESPHERICALPOLYGON_H_


#include "TNodeType.h"
#include "TShape.h"

//!  ShapeSphericalPolygon is the class to represent a spherical cap surface formed by arcs defined with polygonal  geometry.
/*!
  ShapeSphericalPolygon class provides the methods for a speherical cap surface cutted by a regular polygon defined in a escribed circle. The parameters to define the surface are:
  * - sphereRadius: radius of the sepehrical cap
  * - radius: radius of the escribed circle of the polygon
  * - polygonSides: number of sides of the regular polygon
*/

class ShapeSphericalPolygon : public TShape
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	ShapeSphericalPolygon(const ShapeSphericalPolygon&) = delete;
	ShapeSphericalPolygon& operator=(const ShapeSphericalPolygon&) = delete;

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
	ShapeSphericalPolygon( );
	~ShapeSphericalPolygon();

private:
	static TNodeType m_nodeType;
	std::string m_sphereRadiusLabel;
	std::string m_radiusLabel;
	std::string m_polygonSidesLabel;

/*
protected:
	Point3D GetPoint3D ( double u, double v ) const;
	NormalVector GetNormal( double u, double v ) const;
	bool OutOfRange( double u, double v ) const;
	*/


};

#endif /*SHAPESPHERICALPOLYGON_H_*/
