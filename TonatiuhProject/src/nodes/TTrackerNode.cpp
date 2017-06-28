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

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include "TTrackerNode.h"

/******************************
 * TTrackerNode
 ******************************/

TNodeType TTrackerNode::m_nodeType = TNodeType::CreateEmptyType();

/*!
 * Initializes TSurfaceNode type.
 */
void TTrackerNode::Init()
{
	TTrackerNode::m_nodeType = TNodeType::CreateType( TNodeType::FromName( "GroupNode" ), QString( "Tracker" ) );

}

/*!
 * TGroupNode : public TNode
 */
TTrackerNode::TTrackerNode()
:TGroupNode()
{

	setObjectName(GetType().GetName());


}

/*!
 * Destructor.
 */
TTrackerNode::~TTrackerNode()
{

}


/*!
 * Returns the object to world transformation for the parameters defined for this node.
 */
/*
Transform TTrackerNode::GetTrasformation() const
{
	TParameterList* parametersList = GetParameters();
	if( !parametersList || parametersList->NumberOfParameters() == 0 )	return ( Transform() );

	QStringList translationValues = parametersList->Get( QLatin1String("translation") ).toString().split( QRegExp("\\s+"), QString::SkipEmptyParts );
	if( translationValues.count() != 3 ) 	return ( Transform() );
	Transform translation = Translate( translationValues[0].toDouble(), translationValues[1].toDouble(), translationValues[2].toDouble()  );

	QStringList rotationValues = parametersList->Get( QLatin1String("rotation") ).toString().split( QRegExp("\\s+"), QString::SkipEmptyParts );
	if( rotationValues.count() != 4 ) return ( Transform() );
	Vector3D rotationAxis( rotationValues[0].toDouble(), rotationValues[1].toDouble(), rotationValues[2].toDouble());
	if( fabs( 1.0 - rotationAxis.length() ) > gc::Epsilon  )	return ( Transform() );
	Transform rotation = Rotate( rotationValues[3].toDouble(), rotationAxis   );

	QStringList scaleValues = parametersList->Get( QLatin1String("scale") ).toString().split( QRegExp("\\s+"), QString::SkipEmptyParts );
	if( scaleValues.count() != 3 ) return ( Transform() );
	Transform scale = Scale( scaleValues[0].toDouble(), scaleValues[1].toDouble(), scaleValues[2].toDouble() );

	//First scaled, the rotated and finally is translated
	Transform transformOTW = translation*rotation*scale;

	return ( transformOTW );
}
*/

/*!
 * Returns the type of node.
 */
TNodeType TTrackerNode::GetType() const
{
	return ( TTrackerNode::m_nodeType );
}


