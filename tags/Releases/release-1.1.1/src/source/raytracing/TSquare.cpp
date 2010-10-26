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

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QString>

#include <Inventor/SbLinear.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "NormalVector.h"
#include "Point3D.h"
#include "tgf.h"
#include "TSquare.h"


SO_NODE_SOURCE(TSquare);

void TSquare::initClass()
{
	SO_NODE_INIT_CLASS(TSquare, TShape, "TShape");
}

TSquare::TSquare( )
{
	SO_NODE_CONSTRUCTOR(TSquare);
	SO_NODE_ADD_FIELD(m_sideLength, (10.0));
}

TSquare::~TSquare()
{

}

double TSquare::GetArea() const
{
	return ( m_sideLength.getValue() * m_sideLength.getValue() );
}

QString TSquare::GetIcon() const
{
	return ":/icons/icons/node.png";
}

bool TSquare::Intersect(const Ray& /*objectRay*/, double* /*tHit*/, DifferentialGeometry* /*dg*/) const
{
	return true;
}

bool TSquare::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D TSquare::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

Point3D TSquare::GetPoint3D (double u, double v) const
{
	if ((u < 0.0 && u > 1) && (v < 0.0 && v > 1))
	{

		tgf::SevereError("Function TSquare::GetPoint3D called with invalid parameters" );
	}

	double x = (u * m_sideLength.getValue()) - (m_sideLength.getValue()/2);
	double z = v * m_sideLength.getValue() - (m_sideLength.getValue()/2);

	return Point3D(x,0,z);
}

NormalVector TSquare::GetNormal (double /*u*/, double /*v*/) const
{
	return NormalVector( 0, 1, 0 );
}

void TSquare::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/ )
{
	Point3D min = Point3D(-m_sideLength.getValue()/2,0.0, -m_sideLength.getValue()/2);
	Point3D max = Point3D(m_sideLength.getValue()/2,0.0,m_sideLength.getValue()/2);
	box.setBounds(SbVec3f( min.x, min.y, min.z ), SbVec3f( max.x, max.y, max.z ));
}

void TSquare::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;

    // Access the state from the action.
    SoState  *state = action->getState();

    // See if we have to use a texture coordinate function,
    // rather than generating explicit texture coordinates.
    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    // If we need to generate texture coordinates with a
    // function, we'll need an SoGLTextureCoordinateElement.
    // Otherwise, we'll set up the coordinates directly.
    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);


	const int rows = 50; // Number of points per row
    const int columns = 50; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

	double u_step = 1.0 /(double)(rows-1); // Size of the step in parameter u to go from 0 to 1 in the number of rows
	double v_step = 1.0 /(double)(columns-1); // Size of the step in parameter v to go from 0 to 1 in the number of columns

    for (int i = 0; i < rows; ++i)
    {
    	for ( int j = 0 ; j < columns ; ++j )
    	{
    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal = GetNormal(ui, vj);

    		vertex[h][0] = point.x;
    		vertex[h][1] = point.y;
    		vertex[h][2] = point.z;
    		vertex[h][3] = normal.x;
    		vertex[h][4] = normal.y;
    		vertex[h][5] = normal.z;
    		pv.setPoint(vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point.
    		vj += v_step; // Increase parameter vj to go from initial 0 to 1 at the end of the row (keep the same ui for the whole row)
    	}
    	vj = 0; // Re-initialize parameter vj to 0 in order to start from 0 in the next row
		ui += u_step; // Increase parameter ui to go from initial 0 to 1 in the last row.
    }

	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices];
    int k = 0;
    for( int irow = 0; irow < (rows-1); ++irow )
           for(int icolumn = 0; icolumn < (columns-1); ++icolumn )
           {
           	indices[k] = irow*columns + icolumn;
        	indices[k+1] = indices[k] + 1;
        	indices[k+3] = indices[k] + columns;
        	indices[k+2] = indices[k+3] + 1;

        	k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for(int ivert = 0; ivert<totalIndices;ivert++)
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];
		finalvertex[ivert][3] = vertex[indices[ivert]][3];
		finalvertex[ivert][4] = vertex[indices[ivert]][4];
		finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }


    float u = 1;
    float v = 1;

	beginShape(action, QUADS );
    for( int i = 0; i < totalIndices; ++i )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(finalvertex[i][3],finalvertex[i][4], finalvertex[i][5] );
    	SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u, v, 0.0, 1.0 );

    	pv.setPoint(point);
    	pv.setNormal(normal);
    	pv.setTextureCoords(texCoord);
    	shapeVertex(&pv);


    }
    endShape();
    delete[] indices;

}

