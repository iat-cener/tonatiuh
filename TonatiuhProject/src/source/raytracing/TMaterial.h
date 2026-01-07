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
#ifndef TMATERIAL_H_
#define TMATERIAL_H_

#include <string>

#include <Inventor/nodes/SoMaterial.h>

#include "DifferentialGeometry.h"
#include "RandomDeviate.h"
#include "Ray.h"

/*!
 * @class TMaterial
 * @brief Abstract base class for all custom materials in the scene.
 *
 * TMaterial defines the interface for materials that determine how surfaces interact
 * with light and rays in the scene.
 *
 * Subclasses of TMaterial must implement:
 * - Icon representation (`GetIcon`).
 * - Output ray scattering and shading behavior (`OutputRay`), which computes the
 *   outgoing ray based on an incident ray, surface differential geometry,
 *   and a random sample for stochastic effects.
 *
 * This class serves as a base for physically-based or custom material models
 * and can be extended to implement diffuse, specular, or complex shading behaviors.
 */
class TMaterial : public SoMaterial
{
    SO_NODE_ABSTRACT_HEADER(TMaterial);

public:
    static void initClass();

    /*!
    * @brief Get the icon associated with this material.
    *
    * Each material node can provide an icon representing it in the UI. 
    * This function must be implemented by all concrete subclasses of TMaterial.
    *
    * @return A string containing the path of the material's icon.
    */
	virtual std::string GetIcon() = 0;

    /*!
    * @brief Compute the outgoing ray based on the incident ray and intersection point data.
    *
    * This function defines the core interaction between light and the material.
    * Concrete subclasses must implement it to model scattering, reflection,
    * refraction, or other effects.
    *
    * @param incident The incoming ray striking the surface.
    * @param dg Differential geometry information at the hit point.
    * @param rand Random number generator for stochastic effects.
    * @param outputRay Pointer to store the computed outgoing ray.
    * @return True if a valid output ray is generated; false otherwise.
    */
	virtual bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const = 0;
    
    /*!
    * @brief Validate the value of a material parameter.
    *
    * Allows subclasses to enforce constraints or rules on parameter values.
    * The default implementation always returns true.
    *
    * @param name Name of the parameter.
    * @param value Value of the parameter.
    * @return True if the parameter value is valid; false otherwise.
    */
    virtual bool ValidateParamaterValue( std::string /*name*/, std::string /*value*/ ) const { return true; };

protected:
	TMaterial();
    virtual ~TMaterial();
};

#endif /*TMATERIAL_H_*/
