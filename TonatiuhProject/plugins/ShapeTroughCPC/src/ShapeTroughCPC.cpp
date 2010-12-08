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

#include <algorithm>
#include <vector>

#include <QIcon>
#include <QMap>
#include <QMessageBox>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "DifferentialGeometry.h"
#include "Ray.h"
#include "ShapeTroughCPC.h"
#include "tgf.h"
#include "tgc.h"
#include "Vector3D.h"


double fPart( double theta, double a, double thetaI, Ray ray )
{

	//Caso 1a: RIGHT, If= FALSE

	double xrd = ray.direction.x;
	double xro = ray.origin.x;
	double yrd = ray.direction.y;
	double yro = ray.origin.y;

	return ( 1 /(-1 + cos( theta ))) * ( (a + xro ) * yrd - xrd * yro + (-xro * yrd + xrd * yro ) * cos(theta)
					+ a * ( -yrd * ( cos(theta - 2 * thetaI ) + 2 * sin( theta - thetaI ))
					+ 2 * xrd * cos( theta - thetaI ) * (1 + sin(thetaI) ) ) );

}

SO_NODE_SOURCE(ShapeTroughCPC);

void ShapeTroughCPC::initClass()
{
	SO_NODE_INIT_CLASS(ShapeTroughCPC, TShape, "TShape");
}

ShapeTroughCPC::ShapeTroughCPC()
{
	SO_NODE_CONSTRUCTOR(ShapeTroughCPC);
	SO_NODE_ADD_FIELD( a, (0.5) );
	SO_NODE_ADD_FIELD( cMax, (2.0) );
	SO_NODE_ADD_FIELD(lengthXMin, (1.0 ) );
	SO_NODE_ADD_FIELD(lengthXMax, (1.0 ) );
	SO_NODE_ADD_FIELD( height, (1.0) );

	m_thetaI = asin( 1 / cMax.getValue() );
	m_thetaMin = 2 * m_thetaI;

	SoFieldSensor* m_aSensor = new SoFieldSensor(updateHeightValues, this);
	m_aSensor->attach( &a );
	SoFieldSensor* m_cMaxSensor = new SoFieldSensor(updateCMaxValues, this);
	m_cMaxSensor->attach( &cMax );
	SoFieldSensor* m_heightSensor = new SoFieldSensor(updateHeightValues, this);
	m_heightSensor->attach( &height );


}

ShapeTroughCPC::~ShapeTroughCPC()
{
}


double ShapeTroughCPC::GetArea() const
{
	return -1;
}

QString ShapeTroughCPC::GetIcon() const
{
	return ":/icons/ShapeTroughCPC.png";
}

bool ShapeTroughCPC::Intersect(const Ray& objectRay, double *tHit, DifferentialGeometry *dg) const
{
	QMap< double, double > vectorTr;

	double inf = 2 * m_thetaI;
	double sup = ( tgc::Pi / 2 ) + m_thetaI;
	double ts11;
	bool isRoot1 = findRoot( fPart, objectRay, a.getValue(), m_thetaI, inf, sup, 0, 500, &ts11 );
	if( isRoot1 )
	{

		double tr11 = findThit( objectRay, ts11, true );
		vectorTr.insert( tr11, ts11 );
		double ts12;
		if ( findRoot( fPart, objectRay, a.getValue(), m_thetaI, inf, ts11 - 0.00001, 0, 500, &ts12 ) )
		{
			double tr12  = findThit( objectRay, ts12, true );
			vectorTr.insert( tr12, ts12 );
		}
		else
		{
			if( findRoot( fPart, objectRay, a.getValue(), m_thetaI, ts11 + 0.00001, sup, 0, 500, &ts12 ) )
			{
				double tr12 = findThit( objectRay, ts12, true );
				vectorTr.insert( tr12, ts12 );
			}
		}

	}


	int nIntersections =  vectorTr.size();
	if ( nIntersections == 0 ) return false;
	QList< double > keys = vectorTr.keys();
	std::vector<double> array = QVector<double>::fromList( keys ).toStdVector() ;


 	std::sort(array.begin(), array.end());
	double thit = array[0];
	int intersection = 0;
	bool valid = false;
	Point3D hitPoint;

	double xmin = a.getValue();
	double xmax = ( (2 * a.getValue() * (1 + sin(m_thetaI) ) * sin(m_thetaMin-m_thetaI) )
			 / ( 1 - cos(m_thetaMin) ) )- a.getValue();
	double ymin = 0.0;
	double ymax = ( 2 * a.getValue() * cos(m_thetaI)* (1 + sin(m_thetaI ) ) )/(1 - cos( 2 * m_thetaI ) );

	while(  ( intersection < vectorTr.size() ) && !valid )
	{

		thit = array[intersection];

		if ( thit < 0 ) valid = false;
		else
		{
			// Compute intersection distance along ray
				//Evaluate Tolerance
			double tol = 0.0001;
			double theta = vectorTr.value( thit );
			if( ( fabs( thit ) < tol ) || ( theta > ( tgc::Pi / 2 + m_thetaI )  ) ) valid = false;
			else
			{
				if( thit> objectRay.maxt || thit < objectRay.mint ) valid = false;
		    	else
		    	{
			    	// Compute possible collector hit position
					hitPoint = objectRay( thit );

					// Test intersection against clipping parameters
					double m =  ( lengthXMax.getValue() / 2- lengthXMin.getValue() / 2 ) / ( xmax - xmin );
					double zmax = ( lengthXMin.getValue()  / 2 )+ m * ( hitPoint.x - xmin );
					double zmin = -zmax;

					if( hitPoint.z < zmin || hitPoint.z > zmax ||
						hitPoint.y > ymax || hitPoint.y < ymin ||
						hitPoint.x > xmax || hitPoint.x < xmin )	valid = false;
					else valid = true;
		    	}
			}
		}
		intersection++;
	}
	if( !valid ) return false;

	double theta = vectorTr.value( thit );

	// Now check if the fucntion is being called from IntersectP,
	// in which case the pointers tHit and dg are 0
	if( ( tHit == 0 ) && ( dg == 0 ) ) return true;
	else if( ( tHit == 0 ) || ( dg == 0 ) ) tgf::SevereError( "Function Cylinder::Intersect(...) called with null pointers" );


	// Find parametric representation of CPC concentrator hit
	double u = ( theta - 2 * m_thetaI ) / ( tgc::Pi / 2 - m_thetaI );

	double m =  (  ( lengthXMax.getValue()- lengthXMin.getValue() ) / 2 ) /  ( xmax - xmin );
	double zmax = (lengthXMin.getValue() / 2 ) + m* ( hitPoint.x - xmin );
	double v = ( ( hitPoint.z / zmax ) + 1 )/ 2;


	// Compute  \dpdu and \dpdv
	double dpduX = - 0.5 * a.getValue() * (tgc::Pi - 2 * m_thetaI )* pow( (1 / cos( 0.25 * (-2 + u) * ( tgc::Pi - 2 * m_thetaI  ) ) ), 3 )
							* sin( 0.25 * u * ( tgc::Pi  - 2 * m_thetaI ) ) * (1 + sin( m_thetaI ) );

	double dpduY = - 0.5 * a.getValue() * (tgc::Pi - 2 * m_thetaI )* pow( (1 / cos( 0.25 * (-2 + u) * ( tgc::Pi - 2 * m_thetaI  ) ) ), 3 )
								* cos( 0.25 * u * ( tgc::Pi  - 2 * m_thetaI ) ) * (1 + sin( m_thetaI ) );

	Vector3D dpdu(dpduX, dpduY, 0.0);
	Vector3D dpdv(0.0, 0.0, 1.0);

	// Compute cylinder \dndu and \dndv
	double dpduuX = 0.125 * a.getValue() * (tgc::Pi - 2 * m_thetaI ) * (tgc::Pi - 2 * m_thetaI ) * pow( (1 / cos( 0.25 * (-2 + u) * ( tgc::Pi - 2 * m_thetaI  ) ) ), 4 )
								* ( -2 * sin( m_thetaI ) + sin( u * tgc::Pi  / 2  + m_thetaI - u * m_thetaI ) ) * (1 + sin( m_thetaI ) );

	double dpduuY = 0.125 * a.getValue() * (tgc::Pi - 2 * m_thetaI ) * (tgc::Pi - 2 * m_thetaI ) * pow( (1 / cos( 0.25 * (-2 + u) * ( tgc::Pi - 2 * m_thetaI  ) ) ), 4 )
									* ( 2 * cos( m_thetaI ) + cos( u * tgc::Pi  / 2  + m_thetaI - u * m_thetaI ) ) * (1 + sin( m_thetaI ) );

	Vector3D d2Pduu(dpduuX , dpduuY, 0);

	Vector3D d2Pduv( 0.0, 0.0, 0.0 );
	Vector3D d2Pdvv( 0.0, 0.0, 0.0 );

	// Compute coefficients for fundamental forms
	double E = DotProduct( dpdu, dpdu );
	double F = DotProduct( dpdu, dpdv );
	double G = DotProduct( dpdv, dpdv );
	Vector3D N = Normalize( CrossProduct( dpdu, dpdv ) );
	double e = DotProduct( N, d2Pduu );
	double f = DotProduct( N, d2Pduv );
	double g = DotProduct( N, d2Pdvv );

		// Compute \dndu and \dndv from fundamental form coefficients
	double invEGF2 = 1.0 / (E*G - F*F);
	Vector3D dndu = (f*F - e*G) * invEGF2 * dpdu +
			        (e*F - f*E) * invEGF2 * dpdv;
	Vector3D dndv = (g*F - f*G) * invEGF2 * dpdu +
	                (f*F - g*E) * invEGF2 * dpdv;

	// Initialize _DifferentialGeometry_ from parametric information
	*dg = DifferentialGeometry( hitPoint ,
		                        dpdu,
								dpdv,
		                        dndu,
								dndv,
		                        u, v, this );

    // Update _tHit_ for quadric intersection
    *tHit = thit;

	return true;
}

bool ShapeTroughCPC::IntersectP( const Ray& objectRay ) const
{
	return Intersect( objectRay, 0, 0 );
}

Point3D ShapeTroughCPC::Sample( double u, double v ) const
{
	return GetPoint3D( u, v );
}

void ShapeTroughCPC::updateCMaxValues( void *data, SoSensor *)
{
	ShapeTroughCPC* shapeTroughCPC = (ShapeTroughCPC *) data;
	shapeTroughCPC->m_thetaI = asin( 1 / shapeTroughCPC->cMax.getValue() );

	double theta = 2 * shapeTroughCPC->m_thetaI;
	double theta1 = tgc::Pi/2 + shapeTroughCPC->m_thetaI;
	double y1 = 0;
	double theta2 = 2 * shapeTroughCPC->m_thetaI;
	double y2 = ( 2 * shapeTroughCPC->a.getValue() * (1 + sin(shapeTroughCPC->m_thetaI) ) *cos( theta - shapeTroughCPC->m_thetaI ) )
			/( 1 - cos(theta2) );


	double y0 = shapeTroughCPC->height.getValue();

	int iterations = 0;
	while( ( fabs( theta1 - theta2 ) > 0.00001 ) && ( iterations < 100 ) )
	{
		double theta = ( theta1 + theta2 ) / 2 ;

		double y = ( 2 * shapeTroughCPC->a.getValue() * (1 + sin(shapeTroughCPC->m_thetaI) ) *cos( theta - shapeTroughCPC->m_thetaI ) )
				/( 1 - cos(theta) );

		if( ( y >= y0 ) && ( y2 > y1 ) ) 	theta2 = theta;
		else if( ( y >= y0 ) && ( y2 < y1 ) ) 		theta1 = theta;
		else if( ( y < y0 ) && ( y2 > y1 ) ) 		theta1 = theta;
		else	theta2 = theta;

	}
	shapeTroughCPC->m_thetaMin = ( theta1 + theta2 ) / 2;
}

void ShapeTroughCPC::updateHeightValues( void *data, SoSensor *)
{
	ShapeTroughCPC* shapeTroughCPC = (ShapeTroughCPC *) data;

	double theta = 2 * shapeTroughCPC->m_thetaI;
	double yMax = ( 2 * shapeTroughCPC->a.getValue() * (1 + sin(shapeTroughCPC->m_thetaI) ) *cos( theta- shapeTroughCPC->m_thetaI ) )
					/( 1 - cos(theta) );
	if( shapeTroughCPC->height.getValue() > yMax )
	{
			QString message = QString( "y must be equal or smaller than %1. " ).arg( yMax );
			QMessageBox::warning( 0, QString( "Tonatiuh" ), message );
			shapeTroughCPC->height.setValue( yMax  );
			shapeTroughCPC->m_thetaMin = 2 * shapeTroughCPC->m_thetaI;
	}
	else if( shapeTroughCPC->height.getValue() < 0.0 )
	{
			QString message = QString( "y must be positive number. " );
			QMessageBox::warning( 0, QString( "Tonatiuh" ), message );
			shapeTroughCPC->height.setValue( yMax  );
			shapeTroughCPC->m_thetaMin = 2 * shapeTroughCPC->m_thetaI;
	}
	else
	{
		double theta1 = tgc::Pi/2 + shapeTroughCPC->m_thetaI;
		double y1 = 0;
		double theta2 = 2 * shapeTroughCPC->m_thetaI;
		double y2 = ( 2 * shapeTroughCPC->a.getValue() * (1 + sin(shapeTroughCPC->m_thetaI) ) *cos( theta - shapeTroughCPC->m_thetaI ) )
				/( 1 - cos(theta2) );


		double y0 = shapeTroughCPC->height.getValue();

		int iterations = 0;
		while( ( fabs( theta1 - theta2 ) > 0.00001 ) && ( iterations < 100 ) )
		{
			double theta = ( theta1 + theta2 ) / 2 ;

			double y = ( 2 * shapeTroughCPC->a.getValue() * (1 + sin(shapeTroughCPC->m_thetaI) ) *cos( theta - shapeTroughCPC->m_thetaI ) )
					/( 1 - cos(theta) );

			if( ( y >= y0 ) && ( y2 > y1 ) ) 	theta2 = theta;
			else if( ( y >= y0 ) && ( y2 < y1 ) ) 		theta1 = theta;
			else if( ( y < y0 ) && ( y2 > y1 ) ) 		theta1 = theta;
			else	theta2 = theta;

		}
		shapeTroughCPC->m_thetaMin = ( theta1 + theta2 ) / 2;
	}
}


Point3D ShapeTroughCPC::GetPoint3D( double u, double v ) const
{
	if ( OutOfRange( u, v ) ) tgf::SevereError( "Function Poligon::GetPoint3D called with invalid parameters" );


	double xMin = a.getValue();
	double xMax = ( (2 * a.getValue() * (1 + sin(m_thetaI) ) * sin(m_thetaMin-m_thetaI) )
			 / ( 1 - cos(m_thetaMin) ) )- a.getValue();

	double theta =  u* ( ( tgc::Pi / 2 + m_thetaI ) - m_thetaMin )+ m_thetaMin;
	double x = ( (2 * a.getValue() * (1 + sin(m_thetaI) ) * sin(theta-m_thetaI) )
				 / ( 1 - cos(theta) ) )- a.getValue();
	double y = ( 2 * a.getValue() * (1 + sin(m_thetaI) ) *cos(theta- m_thetaI) )
				/( 1 - cos(theta) );

	double m =  ( ( lengthXMax.getValue()/ 2 )- ( lengthXMin.getValue()/ 2 ) ) / ( xMax - xMin );
	double zMax = ( lengthXMin.getValue()/ 2 ) + m* ( x - xMin );

	double z = zMax * ( 2 * v -1 );


	return Point3D (x, y, z);

}

NormalVector ShapeTroughCPC::GetNormal (double u ,double /* v */) const
{
	double dpduX = - 0.5 * a.getValue() * (tgc::Pi + 2 * m_thetaI - 2 * m_thetaMin )* pow( (1 / sin( 0.25 * ( u * ( tgc::Pi + 2 * m_thetaI - 2 * m_thetaMin ) + 2 * m_thetaMin ) ) ), 3 )
						* sin( 0.25 *( - tgc::Pi * u - 2 * ( -2 + u ) * m_thetaI  + 2 * ( -1 + u ) * m_thetaMin ) ) * (1 + sin( m_thetaI ) );

	double dpduY = - 0.5 * a.getValue() * (tgc::Pi + 2 * m_thetaI - 2 * m_thetaMin)* pow( (1 / sin( 0.25 * ( u * ( tgc::Pi + 2 * m_thetaI - 2 * m_thetaMin ) + 2 * m_thetaMin ) ) ), 3 )
							* cos( 0.25 *( - tgc::Pi * u - 2 * ( -2 + u ) * m_thetaI  + 2 * ( -1 + u ) * m_thetaMin ) ) * (1 + sin( m_thetaI ) );

	Vector3D dpdu(dpduX, dpduY, 0.0);

	Vector3D dpdv(0.0, 0.0, 1.0);

	return Normalize( NormalVector( CrossProduct( dpdu, dpdv ) ) );

}

bool ShapeTroughCPC::OutOfRange( double u, double v ) const
{
	return ( ( u < 0.0 ) || ( u > 1.0 ) || ( v < 0.0 ) || ( v > 1.0 ) );
}

void ShapeTroughCPC::computeBBox(SoAction*, SbBox3f& box, SbVec3f& /*center*/)
{
	double xMin = a.getValue();
	double xMax =( (2 * a.getValue() * (1 + sin(m_thetaI) ) * sin(m_thetaMin-m_thetaI) )
			 / ( 1 - cos(m_thetaMin) ) )- a.getValue();
	double yMin = 0.0;
	double yMax = ( 2 * a.getValue() * cos(m_thetaMin-m_thetaI)* (1 + sin(m_thetaI ) ) )/(1 - cos( m_thetaMin ) );

	double zMin = - lengthXMax.getValue() /2;
	double zMax = lengthXMax.getValue() /2;
	box.setBounds(SbVec3f( xMin, yMin, zMin ), SbVec3f( xMax, yMax, zMax ) );
}

void ShapeTroughCPC::generatePrimitives(SoAction *action)
{
    SoPrimitiveVertex   pv;
    SoState  *state = action->getState();

    SbBool useTexFunc = ( SoTextureCoordinateElement::getType(state) ==
                          SoTextureCoordinateElement::FUNCTION );

    const SoTextureCoordinateElement* tce = 0;
    if ( useTexFunc ) tce = SoTextureCoordinateElement::getInstance(state);

	const int rows = 20; // Number of points per row
    const int columns = 10; // Number of points per column
    const int totalPoints = (rows)*(columns); // Total points in the grid

    float vertex[totalPoints][6];

    int h = 0;
    double ui = 0;
	double vj = 0;

    for ( int i = 0; i < rows; ++i )
    {
    	ui =( 1.0 /(double)(rows-1) ) * i;

    	for ( int j = 0 ; j < columns ; ++j )
    	{

    		vj = ( 1.0 /(double)(columns-1) ) * j;

    		Point3D point = GetPoint3D(ui, vj);
    		NormalVector normal = GetNormal(ui, vj);

    		vertex[h][0] = point.x;
    		vertex[h][1] = point.y;
    		vertex[h][2] = point.z;
    		vertex[h][3] = normal.x;
    		vertex[h][4] = normal.y;
    		vertex[h][5] = normal.z;

    		pv.setPoint( vertex[h][0], vertex[h][1], vertex[h][2] );
    		h++; //Increase h to the next point.
    	}
    }

	const int totalIndices  = (rows-1)*(columns-1)*4;
    int32_t* indices = new int32_t[totalIndices];
    int k = 0;
    for( int irow = 0; irow < (rows-1); ++irow )
           for( int icolumn = 0; icolumn < (columns-1); ++icolumn )
           {
				indices[k] = irow*columns + icolumn;
				indices[k+1] = indices[k] + 1;
				indices[k+3] = indices[k] + columns;
				indices[k+2] = indices[k+3] + 1;

				k+=4; //Set k to the first point of the next face.
           }

    float finalvertex[totalIndices][6];
    for( int ivert = 0; ivert<totalIndices; ++ivert )
    {
    	finalvertex[ivert][0] = vertex[indices[ivert]][0];
    	finalvertex[ivert][1] = vertex[indices[ivert]][1];
    	finalvertex[ivert][2] = vertex[indices[ivert]][2];
    	finalvertex[ivert][3] = vertex[indices[ivert]][3];
    	finalvertex[ivert][4] = vertex[indices[ivert]][4];
    	finalvertex[ivert][5] = vertex[indices[ivert]][5];
    }
    delete[] indices;

    float u = 1;
    float v = 1;

	beginShape(action, QUADS);
    for( int i = 0; i < totalIndices; ++i )
    {
    	SbVec3f  point( finalvertex[i][0], finalvertex[i][1],  finalvertex[i][2] );
    	SbVec3f normal(-finalvertex[i][3],-finalvertex[i][4], -finalvertex[i][5] );
		SbVec4f texCoord = useTexFunc ? tce->get(point, normal): SbVec4f( u,v, 0.0, 1.0 );

		pv.setPoint(point);
		pv.setNormal(normal);
		pv.setTextureCoords(texCoord);
		shapeVertex(&pv);
    }
    endShape();
}

double ShapeTroughCPC::findThit( Ray ray, double theta, bool right ) const
{

	double tHit;
	if( ray.direction.x > 0 )
	{
		double x = ((2 * a.getValue() * (1 + sin(m_thetaI) ) * sin( theta-m_thetaI ))
				/( 1 - cos(theta ))) - a.getValue();

		if( !right )	x *= -1;
		tHit = ( x - ray.origin.x ) / ray.direction.x;
	}
	else
	{
		double y = ( 2 * a.getValue() * (1 + sin(m_thetaI) ) *cos( theta-m_thetaI ) )
				/( 1 - cos(theta) );
		tHit = ( y - ray.origin.y ) / ray.direction.y;
	}

	return tHit;
}

bool ShapeTroughCPC::findRoot( double (*funcion)( double, double, double, Ray ), Ray ray, double r, double thetaI, double a, double b, double y0, int max, double* theta )
{

	double y1 = funcion( a, r, thetaI, ray  );
	double y2 = funcion( b, r, thetaI, ray );

	double delta = fabs( a - b ) / 100;
	while( ( ( y1 * y2 ) > 0.0 ) && ( a < b ) )
	{
		a += delta;
		y1 = funcion( a, r, thetaI, ray  );
	}

	if( ( ( y1 * y2 ) > 0.0 ) || ( a > b ) ) return false;

	int iterations = 0;
	while( ( fabs( a - b ) > 0.000000000001 ) && ( iterations < max ) )
	{
		double x = ( a + b ) / 2.0 ;
		double y = funcion( x, r, thetaI, ray );

		if( y == y0 )
		{
			*theta = x;
			return true;
		}

		if( ( y > y0 ) && ( y2 > y1 ) ) 	b= x;
		else if( ( y > y0 ) && ( y2 < y1 ) ) 		a = x;
		else if( ( y < y0 ) && ( y2 > y1 ) ) 		a= x;
		else	b = x;
		iterations++;

	}

	if( iterations >= max ) return false;

	*theta  = ( a + b ) / 2 ;
	return true;
}
