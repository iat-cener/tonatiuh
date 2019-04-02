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

#ifndef TSHAPEKIT_H_
#define TSHAPEKIT_H_

#include <Inventor/nodekits/SoShapeKit.h>
#include <QString>

class RandomDeviate;
class Ray;

//!  TShapeKit class groups what is necessary to the shape.
/*!
  TShapeKit groups the shape geometry, material and the transformation.

*/

class TShapeKit : public SoShapeKit
{
	typedef SoShapeKit inherited;

    SO_KIT_HEADER(TShapeKit);

public:
    TShapeKit();
    static void initClass();

    bool IntersectP( const Ray& ray ) const;
	Ray* Intersect( const Ray& objectRay, bool* isShapeFront, RandomDeviate& rand ) const;

protected:
    virtual ~TShapeKit();

};

#endif /*TSHAPEKIT_H_*/
