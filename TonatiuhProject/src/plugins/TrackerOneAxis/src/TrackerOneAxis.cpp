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



#include <cmath>

#include "gc.h"
#include "TParameterList.h"
#include "TrackerOneAxis.h"
#include "Transform.h"
#include "Vector3D.h"


TNodeType TrackerOneAxis::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* TrackerOneAxis::CreateInstance( )
{
  return ( new TrackerOneAxis() );
}

/*!
 * Initializes TrackerParabolicDish type.
 */
void TrackerOneAxis::Init()
{

	TrackerOneAxis::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Tracker" ), QString( "TrackerOneAxis" ), &TrackerOneAxis::CreateInstance );
}

/*!
 * Creates a tracker object.
 */
TrackerOneAxis::TrackerOneAxis()
:TTrackerNode()
{
	//Default object name is the name of the type
	setObjectName(GetType().GetName());

	QString transformationValue( QLatin1String("") );
	for (int i = 0; i < 4; ++i)
	{
		transformationValue += QLatin1String( "[ " );
		for (int j = 0; j < 4; ++j)
		{
			if ( i == j )	transformationValue += QLatin1String( "1.0" );
			else	transformationValue += QLatin1String( "0.0" );
			if (j != 3) transformationValue += QLatin1String( ", ");
		}
		transformationValue += QLatin1String( " ] " );
	}


	// Define input fields and their default values
	m_parametersList->Append( QLatin1String( "axis" ), QLatin1String( "X" ) );

	//Transformation
	m_parametersList->Append( QLatin1String("node_transformation"), transformationValue, false );

}

/*!
 * Destroys the object.
 */
TrackerOneAxis::~TrackerOneAxis()
{
	setObjectName( GetType().GetName());

}

/*!
 * Returns  the object to world transformation for the given \a sunVector. The sun vector is given in global coordinates.
 * The local y axis is parallel to the sun vector.
 */
Transform TrackerOneAxis::GetTrasformation( ) const
{
	QString transformationValue = m_parametersList->GetValue( QLatin1String("node_transformation") ).toString();

	QStringList transformationValues = transformationValue.split( QRegExp("[\\s+,\\[\\]]"), QString::SkipEmptyParts );
	double nodeTransformationMatrix[4][4];
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			nodeTransformationMatrix[i][j] = transformationValues[i*4+j].toDouble();

	return ( Transform( nodeTransformationMatrix ) );
}

/*!
 * Returns node type.
 */
TNodeType TrackerOneAxis::GetType() const
{
	return ( TrackerOneAxis::m_nodeType );
}

/*!
 * Updates current node transformation matrix using the \a sunVector in global coordinates and \a the parent transform \a parentWTO.
 */
void TrackerOneAxis::UpdateTrackerTransform( Vector3D sunVector, Transform parentWT0 )
{

	if( m_parametersList->GetValue( QLatin1String("axis") ).toString() == QLatin1String( "X" ) )
	{
		Vector3D s = Normalize( parentWT0( sunVector ) );
		Vector3D p( 1.0f, 0.0f, 0.0f);

		Vector3D n;
		Vector3D t;
		if( fabs( DotProduct( s, p ) ) < 1.0 )
		{

			t = Normalize( CrossProduct( p, s ) );
			n = Normalize( CrossProduct( t, p ) );
		}
		else
		{
			t = Vector3D( 0.0f, 0.0f, 1.0f );
			n = Normalize( CrossProduct( t, p ) );
		}


		Matrix4x4 nodeTransformation = Matrix4x4( t[0], n[0], p[0], 0.0,
											t[1], n[1], p[1], 0.0,
											t[2], n[2], p[2], 0.0,
											0.0, 0.0, 0.0, 1.0 );




		QString transformationValue( QLatin1String("") );
		for (int i = 0; i < 4; ++i)
		{
			transformationValue += QLatin1String( "[ " );
			for (int j = 0; j < 4; ++j)
			{

				if( fabs( nodeTransformation.m[i][j] ) < gc::Epsilon ) transformationValue += QLatin1String( "0.0" );
				else	 transformationValue += QString::number( nodeTransformation.m[i][j] );
				if (j != 3) transformationValue += QLatin1String( ", ");
			}
			transformationValue += QLatin1String( " ]\n" );
		}

		m_parametersList->SetValue( QLatin1String("node_transformation") , transformationValue );
	}
	else if( m_parametersList->GetValue( QLatin1String("axis") ).toString() == QLatin1String( "Y" ) )
	{
		//NOT IMPLEMENTED YET
	}
	else if( m_parametersList->GetValue( QLatin1String("axis") ).toString() == QLatin1String( "Z" ) )
	{
		//NOT IMPLEMENTED YET
	}


}
