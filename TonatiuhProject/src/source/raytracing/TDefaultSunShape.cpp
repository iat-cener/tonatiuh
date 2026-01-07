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

Contributors: Javier Garcia-Barberena, Iñaki Perez, Iñigo Pagola, Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/
#include "TDefaultSunShape.h"

SO_NODE_SOURCE(TDefaultSunShape);

/*!
 * @brief Initialize the TDefaultSunShape class in the system.
 *
 * Registers the class as a node type.
 */
void TDefaultSunShape::initClass()
{
	SO_NODE_INIT_CLASS(TDefaultSunShape, TSunShape, "TSunShape");
}

/*!
 * @brief Constructor for TDefaultSunShape.
 *
 * Creates an object of a default sunshape.
 */
TDefaultSunShape::TDefaultSunShape( )
{
	SO_NODE_CONSTRUCTOR(TDefaultSunShape);
}

/*!
 * @brief Destructor for TDefaultSunShape.
 *
 * Cleans up resources specific to this implementation.
 */
TDefaultSunShape::~TDefaultSunShape()
{
}

/*!
 * @brief Generate a ray direction based on the default sunshape.
 *
 * This implementation does not generate rays and is provided only to
 * satisfy the abstract interface from TSunShape.
 *
 * @param direction Output parameter for the sampled direction (unused).
 * @param rand Random number generator used to sample the distribution (unused).
 */
void TDefaultSunShape::GenerateRayDirection( Vector3D& /* direction */, RandomDeviate& /* rand */ ) const
{
 	// Intentionally empty: no ray is generated
}

 /*!
 * @brief Returns the irradiance value.
 *
 * This implementation does not provide a valid irradiance.
 *
 * @return Returns -1.0 to indicate that no valid value is available.
 */
double TDefaultSunShape::GetIrradiance() const
{
	return -1;
}


/*!
 * @brief Get the maximum angular extent of the default sunshape.
 *
 * This implementation does not generate any rays, so the angular extent is 0.
 *
 * @return Returns 0.0.
 */
double TDefaultSunShape::GetThetaMax() const
{
	return 0.0;
}

/*!
 * @brief Create a copy of this node.
 *
 * Overrides SoNode::copy(). Useful for duplicating the sunshape node.
 * 
 * @param copyConnections If true, connections to other nodes are also copied.
 * @return Pointer to the newly created copy.
 */
SoNode* TDefaultSunShape::copy( SbBool copyConnections ) const
{
	// Use the standard version of the copy method to create
	// a copy of this instance, including its field data
	TDefaultSunShape* newSunShape = static_cast< TDefaultSunShape* >( SoNode::copy( copyConnections ) );

	return newSunShape;
}
