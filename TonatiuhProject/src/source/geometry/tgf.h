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

#ifndef TGF_H_
#define TGF_H_

class SbMatrix;
class RandomDeviate;
class SoTransform;
class Transform;

namespace tgf
{
   double AlternateBoxMuller( RandomDeviate& rand );
	SbMatrix MatrixFromTransform( const Transform& transform );
	Transform TransformFromMatrix( SbMatrix const& matrix );
	Transform TransformFromSoTransform( SoTransform* const & soTransform );
	SbMatrix MatrixFromSoTransform( SoTransform* const & soTransform );
}

#endif /*TGF_H_*/
