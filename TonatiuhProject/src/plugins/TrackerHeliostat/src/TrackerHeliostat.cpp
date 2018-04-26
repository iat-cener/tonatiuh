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

#include <cmath>
#include <sstream>

#include "gc.h"
#include "Point3D.h"
#include "Transform.h"
#include "Vector3D.h"

#include "nf.h"
#include "EnumeratedTypes.h"
#include "TrackerHeliostat.h"

TNodeType TrackerHeliostat::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > TrackerHeliostat::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateTrackerHeliostat : public TrackerHeliostat { using TrackerHeliostat::TrackerHeliostat; };
	return ( std::make_shared<EnableCreateTrackerHeliostat>() );
}

/*!
 * Initializes TrackerHeliostat type.
 */
void TrackerHeliostat::Init()
{

	TrackerHeliostat::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Tracker" ), "TrackerHeliostat", &TrackerHeliostat::CreateInstance );
}

/*!
 * Creates a tracker object.
 */
TrackerHeliostat::TrackerHeliostat()
:TTracker(),
 m_aimingPointLabel(  "aimingPoint" ),
 m_aimingPointTypeLabel(  "typeOfAimingPoint" ),
 m_absoluteAimingLabel(  "Absolute" ),
 m_relativeAimingLabel(  "Relative" ),
 m_rotationTypeLabel(  "typeOfRotation" ),
 m_yxRotationLabel( "YX" ),
 m_yzRotationLabel( "YZ" ),
 m_xzRotationLabel( "XZ" ),
 m_zxRotationLabel( "ZX" ),
 m_nodeTransformationLabel( "node_transformation" )
{
	//Default object name is the name of the type
	SetName(GetType().GetName() );

	std::string transformationValue( "" );
	for (int i = 0; i < 4; ++i)
	{
		transformationValue +=  std::string( "[ " );
		for (int j = 0; j < 4; ++j)
		{
			if ( i == j )	transformationValue += std::string( "1.0" );
			else	transformationValue += std::string( "0.0" );
			if (j != 3) transformationValue += std::string( ", ");
		}
		transformationValue += std::string( " ] " );
	}


	// Define input fields and their default values
	m_pParametersList->Append<Point3D>( m_aimingPointLabel, Point3D( 0.0, 0.0, 0.0 ), true );


	EnumeratedTypes typeOfAimingPointEnumerator;
	typeOfAimingPointEnumerator.AddValue( m_absoluteAimingLabel, true );
	typeOfAimingPointEnumerator.AddValue( m_relativeAimingLabel, false );
	m_pParametersList->Append<EnumeratedTypes>( m_aimingPointTypeLabel, typeOfAimingPointEnumerator, true );

	EnumeratedTypes typeOfRotationEnumerator;
	typeOfRotationEnumerator.AddValue( m_yxRotationLabel, true );
	typeOfRotationEnumerator.AddValue( m_yzRotationLabel, false );
	typeOfRotationEnumerator.AddValue( m_xzRotationLabel, false );
	typeOfRotationEnumerator.AddValue( m_zxRotationLabel, false );
	m_pParametersList->Append<EnumeratedTypes>( m_rotationTypeLabel, typeOfRotationEnumerator, true );

	//Transformation
	m_pParametersList->Append<std::string>( m_nodeTransformationLabel, transformationValue, false );
}

/*!
 * Destroys the object.
 */
TrackerHeliostat::~TrackerHeliostat()
{

}

/*!
 * Creates a copy of tracker node.
 */
std::shared_ptr< TNode > TrackerHeliostat::Copy() const
{
	struct EnableCreateTrackerHeliostat : public TrackerHeliostat { using TrackerHeliostat::TrackerHeliostat; };
	std::shared_ptr< TrackerHeliostat > trackerNode  = std::make_unique<EnableCreateTrackerHeliostat>();
	if( trackerNode == 0 )	return ( 0  );

	//Coping the parameters.
	trackerNode->m_pParametersList->SetValue( m_aimingPointLabel, GetParameterValue<Point3D>( m_aimingPointLabel ) );
	trackerNode->m_pParametersList->SetValue( m_aimingPointTypeLabel, GetParameterValue<EnumeratedTypes>( m_aimingPointTypeLabel ) );
	trackerNode->m_pParametersList->SetValue( m_rotationTypeLabel, GetParameterValue<EnumeratedTypes>( m_rotationTypeLabel ) );
	trackerNode->m_pParametersList->SetValue( m_nodeTransformationLabel, GetParameterValue<std::string>( m_nodeTransformationLabel ) );

	return ( trackerNode );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string TrackerHeliostat::GetIcon() const
{
	return ( ":/icons/TrackerHeliostat.png" );
}

/*!
 * Returns  the object to world transformation for the given \a sunVector. The sun vector is given in global coordinates.
 * The local y axis is parallel to the sun vector.
 */
Transform TrackerHeliostat::GetTrasformation( ) const
{
	std::string transformationValue = GetParameterValue<std::string>( m_nodeTransformationLabel );
	std::vector< std::string > transformationValues = nf::StringSplit( transformationValue,
			"[\\s+,\\[\\]]" );

	double nodeTransformationMatrix[4][4];
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			nodeTransformationMatrix[i][j] = stod( transformationValues[i*4+j] ) ;

	return ( Transform( nodeTransformationMatrix ) );
}

/*!
 * Returns node type.
 */
TNodeType TrackerHeliostat::GetType() const
{
	return ( TrackerHeliostat::m_nodeType );
}

/*!
 * Updates current node transformation matrix using the \a sunVector in global coordinates and \a the parent transform \a parentWTO.
 */
void TrackerHeliostat::UpdateTrackerTransform( Vector3D sunVector, Transform parentWT0 )
{

	Vector3D i = Normalize( parentWT0( sunVector ) );

	if( i.length() == 0.0f ) return;
	i = Normalize(i);

	Point3D focus = GetParameterValue<Point3D>( m_aimingPointLabel );

	EnumeratedTypes typeOfAimingPoint = GetParameterValue<EnumeratedTypes>( m_aimingPointTypeLabel );
	Vector3D r;
	if (typeOfAimingPoint.GetSelectedName( ) == m_absoluteAimingLabel ) //Absolute
	{
		r = Vector3D( parentWT0( focus ) );
	}
	else
		r = Vector3D( focus );


	if( r.length() == 0.0f ) return;
	r = Normalize(r);

	Vector3D n = ( i + r );
	if( n.length() == 0.0f ) return;
	n = Normalize( n );

	EnumeratedTypes typeOfRotation =  GetParameterValue<EnumeratedTypes>( m_rotationTypeLabel ) ;

	Vector3D Axe1;
	if ((typeOfRotation.GetSelectedName( ) == m_yxRotationLabel ) || (typeOfRotation.GetSelectedName( ) == m_yzRotationLabel ))// YX or YZ
		Axe1 = Vector3D( 0.0f, 1.0f, 0.0f );

	else if (typeOfRotation.GetSelectedName( ) == m_xzRotationLabel ) // XZ
		Axe1 = Vector3D( 1.0f, 0.0f, 0.0f );

	else // ZX
		Axe1 = Vector3D(0.0f, 0.0f, 1.0f);

	Vector3D t = CrossProduct( n, Axe1 );
	//Vector3D t( n[2], 0.0f, -n[0] );
	if( t.length() == 0.0f ) return;
	t = Normalize(t);

	Vector3D p = CrossProduct( t, n );
	if (p.length() == 0.0f) return;
	p = Normalize(p);

	Matrix4x4 transformMatrix;
	if ((typeOfRotation.GetSelectedName( )  == m_yxRotationLabel ) || (typeOfRotation.GetSelectedName( )  == m_zxRotationLabel ))// YX ou  ZX
	{
		 transformMatrix = Matrix4x4( t[0], n[0], p[0], 0.0,
					t[1], n[1], p[1], 0.0,
					t[2], n[2], p[2], 0.0,
					0.0, 0.0, 0.0, 1.0 );

	}
	else // YZ
	{
		 transformMatrix = Matrix4x4( p[0], n[0], t[0], 0.0,
					p[1], n[1], t[1], 0.0,
					p[2], n[2], t[2], 0.0,
					0.0, 0.0, 0.0, 1.0 );

	}


	std::string transformationValue( "" );
	for (int i = 0; i < 4; ++i)
	{
		transformationValue +=  std::string( "[ " );
		for (int j = 0; j < 4; ++j)
		{

			if( fabs( transformMatrix.m[i][j] ) < gc::Epsilon ) transformationValue +=  std::string( "0.0" );
			else
			{
				std::ostringstream double_convert;
				double_convert << transformMatrix.m[i][j];
				transformationValue += double_convert.str();
			}

			if (j != 3) transformationValue +=   std::string( ", ");
		}
		transformationValue +=   std::string( " ]\n" );
	}

	m_pParametersList->SetValue( m_nodeTransformationLabel, transformationValue );
}
