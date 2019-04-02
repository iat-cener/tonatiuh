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
***************************************************************************/

#ifndef TMATERIAL_H_
#define TMATERIAL_H_

#include <Inventor/nodes/SoMaterial.h>

struct DifferentialGeometry;
class RandomDeviate;
class Ray;
class QString;

class TMaterial : public SoMaterial
{
    SO_NODE_ABSTRACT_HEADER(TMaterial);

public:
    static void initClass();

	virtual QString getIcon() = 0;
	virtual bool OutputRay( const Ray& incident, DifferentialGeometry* dg, RandomDeviate& rand, Ray* outputRay  ) const = 0;

protected:
	TMaterial();
    virtual ~TMaterial();
};

#endif /*TMATERIAL_H_*/
