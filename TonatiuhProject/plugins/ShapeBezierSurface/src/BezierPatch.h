/***************************************************************************
Copyright (C) 2008 by the Ekhi Software Development Team.

This file is part of Ekhi.

Ekhi program is free software: you can redistribute it and/or modify
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

The development of Ekhi was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Ekhi under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Ekhi is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Iñaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef BEZIERPATCH_H_
#define BEZIERPATCH_H_

#include <vector>

#include "Point3D.h"
#include "Vector3D.h"

#include "BBox.h"
#include "TShape.h"


class BezierPatch //: public SoNurbsSurface
{

public:
	BezierPatch();
    ~BezierPatch();

	BBox GetBBox() const { return ( m_bbox ); } ;
	Point3D GetCentroid() const { return ( m_centoid ); };
	NormalVector GetNormal( double u, double v ) const;
	Point3D GetPoint3D (double u, double v) const;

	void GeneratePrimitives( SoAction *action );
    bool Intersect( const Ray& objectRay, double* tHit, DifferentialGeometry* dg ) const;
    void SetControlPoints( std::vector< Point3D > boundedPoints );


private:
    std::vector< Vector3D > CornerDerivates( std::vector< Point3D > boundedPoints );
	Vector3D DPDU( double u, double v, std::vector< Vector3D >* controlPoint = 0 ) const;
	Vector3D DPDV( double u, double v, std::vector< Vector3D >* controlPoint = 0 ) const;

	Vector3D D2PDUU( double u, double v, std::vector< Vector3D >* controlPoint ) const;
	Vector3D D2PDUV( double u, double v, std::vector< Vector3D >* controlPoint  ) const;
	Vector3D D2PDVV( double u, double v, std::vector< Vector3D >* controlPoint  ) const;

	void SplitIPatch( std::vector< Vector3D > p, std::vector< std::vector< Vector3D > >* iPatches ) const;

	void HullSplitU( std::vector< Vector3D > p, std::vector< Vector3D >* q, std::vector< Vector3D >* r ) const;
	void HullSplitV( std::vector< Vector3D > p, std::vector< Vector3D >* q, std::vector< Vector3D >* r) const;


private:
	BBox m_bbox;
	Point3D m_centoid;
	std::vector< Point3D > m_controlPoints;

	int m_nIterations;
};

#endif /* BEZIERPATCH_H_ */
