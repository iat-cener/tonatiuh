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

#include "gc.h"
#include "Transform.h"
#include "Vector3D.h"

#include "nf.h"
#include "TrackerParabolicDish.h"

TNodeType TrackerParabolicDish::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
std::shared_ptr< TNode > TrackerParabolicDish::CreateInstance( )
{
	//shared_prt needs a public constructor
	struct EnableCreateTrackerParabolicDish : public TrackerParabolicDish { using TrackerParabolicDish::TrackerParabolicDish; };
	return std::make_shared<EnableCreateTrackerParabolicDish>();
}

/*!
 * Initializes TrackerParabolicDish type.
 */
void TrackerParabolicDish::Init()
{

	TrackerParabolicDish::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Tracker" ),  "TrackerParabolicDish", &TrackerParabolicDish::CreateInstance );
}

/*!
 * Creates a tracker object.
 */
TrackerParabolicDish::TrackerParabolicDish()
:TTracker(),
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

	//Transformation
	m_pParametersList->Append<std::string>( m_nodeTransformationLabel, transformationValue, false );

}

/*!
 * Destroys the object.
 */
TrackerParabolicDish::~TrackerParabolicDish()
{

}

/*!
 * Creates a copy of tracker node.
 */
std::shared_ptr< TNode > TrackerParabolicDish::Copy() const
{
	struct EnableCreateTrackerParabolicDish : public TrackerParabolicDish { using TrackerParabolicDish::TrackerParabolicDish; };
	std::shared_ptr< TrackerParabolicDish > trackerNode  = std::make_unique<EnableCreateTrackerParabolicDish>();
	if( trackerNode == 0 )	return ( 0  );

	 //Coping the parameters.
	 trackerNode->m_pParametersList->SetValue( m_nodeTransformationLabel, GetParameterValue<std::string>( m_nodeTransformationLabel ) );

	 return ( trackerNode );
}

/*!
 * Returns the filename that stores the shape icon.
 */
std::string TrackerParabolicDish::GetIcon() const
{
	return ( ":/icons/TrackerParabolicDish.png" );
}

/*!
 * Returns  the object to world transformation for the given \a sunVector. The sun vector is given in global coordinates.
 * The local y axis is parallel to the sun vector.
 */
Transform TrackerParabolicDish::GetTrasformation( ) const
{
	//GetParameterValue only for visible parameters
	//std::string transformationValue = GetParameterValue<std::string>( m_nodeTransformationLabel );

	std::string transformationValue =  std::get<std::string>( m_pParametersList->GetValue( m_nodeTransformationLabel ).value() );


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
TNodeType TrackerParabolicDish::GetType() const
{
	return ( TrackerParabolicDish::m_nodeType );
}

/*!
 * Updates current node transformation matrix using the \a sunVector in global coordinates and \a the parent transform \a parentWTO.
 */
void TrackerParabolicDish::UpdateTrackerTransform( Vector3D sunVector, Transform parentWT0 )
{
	Matrix4x4 nodeTransformation;

	if( std::abs( 1.0 - DotProduct( sunVector,  Vector3D( 0.0f, 1.0f, 0.0f ) ) ) < 0.000001 )
	{
		nodeTransformation = Matrix4x4( 1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0 );
	}
	else
	{
		Vector3D i = Normalize( parentWT0( sunVector ) );
		Vector3D axe1 = Vector3D( 0.0f, 1.0f, 0.0f );
		Vector3D t = Normalize( CrossProduct( i, axe1 ) );
		Vector3D p = Normalize( CrossProduct( t, i ) );

		nodeTransformation = Matrix4x4( t[0], i[0], p[0], 0.0,
											t[1], i[1], p[1], 0.0,
											t[2], i[2], p[2], 0.0,
											0.0, 0.0, 0.0, 1.0 );
	}

	std::string transformationValue( "" );
	for (int i = 0; i < 4; ++i)
	{
		transformationValue += std::string( "[ " );
		for (int j = 0; j < 4; ++j)
		{

			if( fabs( nodeTransformation.m[i][j] ) < gc::Epsilon ) transformationValue += std::string( "0.0" );
			else
			{
				std::ostringstream double_convert;
				double_convert << nodeTransformation.m[i][j];
				transformationValue += double_convert.str();
			}
			if (j != 3) transformationValue += std::string( ", ");
		}
		transformationValue += std::string( " ]\n" );
	}

	m_pParametersList->SetValue( m_nodeTransformationLabel, transformationValue );

}
