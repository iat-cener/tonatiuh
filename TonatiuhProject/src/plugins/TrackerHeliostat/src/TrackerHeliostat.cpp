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
#include "Point3D.h"
#include "Transform.h"
#include "Vector3D.h"

#include "TParameterEnumerator.h"
#include "TParameterList.h"
#include "TrackerHeliostat.h"

TNodeType TrackerHeliostat::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Creates a new instance of the class type corresponding object.
 */
void* TrackerHeliostat::CreateInstance( )
{
  return ( new TrackerHeliostat() );
}

/*!
 * Initializes TrackerHeliostat type.
 */
void TrackerHeliostat::Init()
{

	TrackerHeliostat::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "Tracker" ), QString( "TrackerHeliostat" ), &TrackerHeliostat::CreateInstance );
}

/*!
 * Creates a tracker object.
 */
TrackerHeliostat::TrackerHeliostat()
:TTrackerNode(),
 m_aimingPointLabel( QLatin1String( "aimingPoint" ) ),
 m_aimingPointTypeLabel( QLatin1String( "typeOfAimingPoint" ) ),
 m_absoluteAimingLabel( QLatin1String( "Absolute" ) ),
 m_relativeAimingLabel( QLatin1String( "Relative" ) ),
 m_rotationTypeLabel( QLatin1String( "typeOfRotation" ) ),
 m_yxRotationLabel( QLatin1String( "YX" ) ),
 m_yzRotationLabel( QLatin1String( "YZ" ) ),
 m_xzRotationLabel( QLatin1String( "XZ" ) ),
 m_zxRotationLabel( QLatin1String( "ZX" ) )
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
	m_parametersList->Append( m_aimingPointLabel, QLatin1String( "0 0 0" ) );

	TParameterEnumerator* typeOfAimingPointEnumerator = new TParameterEnumerator;
	typeOfAimingPointEnumerator->AddValue( m_absoluteAimingLabel, true );
	typeOfAimingPointEnumerator->AddValue( m_relativeAimingLabel, false );
	QVariant typeOfAimingPointParameter;
	typeOfAimingPointParameter.setValue( typeOfAimingPointEnumerator);
	m_parametersList->Append( m_aimingPointTypeLabel, typeOfAimingPointParameter );

	TParameterEnumerator* typeOfRotationEnumerator = new TParameterEnumerator;
	typeOfRotationEnumerator->AddValue( m_yxRotationLabel, true );
	typeOfRotationEnumerator->AddValue( m_yzRotationLabel, false );
	typeOfRotationEnumerator->AddValue( m_xzRotationLabel, false );
	typeOfRotationEnumerator->AddValue( m_zxRotationLabel, false );
	QVariant typeOfRotationParameter;
	typeOfRotationParameter.setValue( typeOfRotationEnumerator);
	m_parametersList->Append( m_rotationTypeLabel, typeOfRotationParameter  );

	//Transformation
	m_parametersList->Append( QLatin1String("node_transformation"), transformationValue, false );
}

/*!
 * Destroys the object.
 */
TrackerHeliostat::~TrackerHeliostat()
{
	TParameterEnumerator* typeOfAimingPointEnumerator = m_parametersList->GetValue( m_aimingPointTypeLabel ).value<TParameterEnumerator*>();
	delete typeOfAimingPointEnumerator;
	typeOfAimingPointEnumerator = 0;

	TParameterEnumerator* typeOfRotationEnumerator = m_parametersList->GetValue( m_rotationTypeLabel ).value<TParameterEnumerator*>();
	delete typeOfRotationEnumerator;
	typeOfRotationEnumerator = 0;

}


/*!
 * Returns  the object to world transformation for the given \a sunVector. The sun vector is given in global coordinates.
 * The local y axis is parallel to the sun vector.
 */
Transform TrackerHeliostat::GetTrasformation( ) const
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

	QStringList aimingPointValues = m_parametersList->GetValue( m_aimingPointLabel ).toString().split( QRegExp("\\s+"), QString::SkipEmptyParts );
	if( aimingPointValues.count() != 3 ) 	return;
	Point3D focus( aimingPointValues[0].toDouble(), aimingPointValues[1].toDouble(), aimingPointValues[2].toDouble()  );


	TParameterEnumerator* typeOfAimingPoint = m_parametersList->GetValue( m_aimingPointTypeLabel ).value<TParameterEnumerator*>();
	Vector3D r;
	if (typeOfAimingPoint->GetSelectedName( ) == m_absoluteAimingLabel ) //Absolute
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

	TParameterEnumerator* typeOfRotation = m_parametersList->GetValue( m_rotationTypeLabel ).value<TParameterEnumerator*>();

	Vector3D Axe1;
	if ((typeOfRotation->GetSelectedName( ) == m_yxRotationLabel ) || (typeOfRotation->GetSelectedName( ) == m_yzRotationLabel ))// YX or YZ
		Axe1 = Vector3D( 0.0f, 1.0f, 0.0f );

	else if (typeOfRotation->GetSelectedName( ) == m_xzRotationLabel ) // XZ
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
	if ((typeOfRotation->GetSelectedName( )  == m_yxRotationLabel ) || (typeOfRotation->GetSelectedName( )  == m_zxRotationLabel ))// YX ou  ZX
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


	QString transformationValue( QLatin1String("") );
	for (int i = 0; i < 4; ++i)
	{
		transformationValue += QLatin1String( "[ " );
		for (int j = 0; j < 4; ++j)
		{

			if( fabs( transformMatrix.m[i][j] ) < gc::Epsilon ) transformationValue += QLatin1String( "0.0" );
			else	 transformationValue += QString::number( transformMatrix.m[i][j] );
			if (j != 3) transformationValue += QLatin1String( ", ");
		}
		transformationValue += QLatin1String( " ]\n" );
	}

	m_parametersList->SetValue( QLatin1String("node_transformation") , transformationValue );
}

/*

void TrackerHeliostat::SwitchAimingPointType()
{
	if( m_previousAimingPointType == typeOfAimingPoint.getValue() )	return;

	SoPath* nodePath= m_scene->GetSoPath( this );
	if (!nodePath) return;

	SoNodeKitPath* parentPath = static_cast< SoNodeKitPath* >( nodePath );
	parentPath->ref();
	parentPath->pop();

	Transform objectToWorld = trf::GetObjectToWorld( parentPath );

	Point3D focus( aimingPoint.getValue( )[0], aimingPoint.getValue( )[1],aimingPoint.getValue( )[2] );
	Point3D r;
	if (typeOfAimingPoint.getValue() == 1)
	{
		Transform worldToObject = objectToWorld.GetInverse();
		r = worldToObject( focus );
	}
	else
		r = objectToWorld( focus );

	aimingPoint.setValue( r.x, r.y, r.z );

	parentPath->unref();
	m_previousAimingPointType = typeOfAimingPoint.getValue();
}
*/


