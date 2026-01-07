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
#ifndef TSUNSHAPE_H_
#define TSUNSHAPE_H_

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>

#include "RandomDeviate.h"
#include "Vector3D.h"

/**
 * @class TSunShape
 * @brief Abstract base class representing class representing the solar radiation angular intensity distribution.
 *
 * The TSunShape class defines the interface for model the angular intensity distributio. The use of this class is
 * for simulation where solar ray generation is required. 
 *
 * This class is **abstract** and cannot be instantiated directly. Concrete
 * subclasses must implement ray-generation behavior and provide irradiance and
 * angular-limit information.
 *
 * ### Responsibilities of Derived Classes
 * - Provide a directional ray generator based on a specified angular emission
 *   distribution and he maximum polar angle (theta) beyond which no rays are emitted.
 * - Define the total irradiance emitted by the sun model.
 *
 * ### Usage
 * A ray-tracing or sampling system will typically:
 * 1. Request a new ray direction via GenerateRayDirection().
 * 2. Apply bounding constraints using GetThetaMax().
 * 3. Scale the resulting contribution using the irradiance value.
 */
class TSunShape : public SoNode
{
	  typedef SoNode inherited;

	SO_NODE_ABSTRACT_HEADER(TSunShape);

public:
    static void initClass();
    /*!
    * @brief Generate a random ray direction according to the sunshape.
    *
    * Pure virtual: must be implemented by derived classes to produce
    * ray directions following the specific angular distribution of the sunshape.
    * 
    * @param direction Output parameter that will contain the sampled direction.
    * @param rand Random number generator used to sample the distribution.
    */
	virtual void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const = 0;

    /*!
    * @brief Get the irradiance for the sunshape.
    *
    * Pure virtual: derived classes must return the corresponding irradiance
    * for their angular distribution.
    * 
    * @return Irradiance value in appropriate units.
    */
	virtual double GetIrradiance() const = 0;   
    
    /*!
    * @brief Get the maximum angular extent of the sunshape.
    *
    * Pure virtual: derived classes must provide the angular limit
    * beyond which the sunshape contributes no radiation.
    * 
    * @return Maximum polar angle (theta) in radians.
    */
    virtual double GetThetaMax() const = 0;

protected:
    TSunShape();
    virtual ~TSunShape();
};

#endif /*TSUNSHAPE_H_*/
