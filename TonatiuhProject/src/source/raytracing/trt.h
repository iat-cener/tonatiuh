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

#ifndef TRT_H_
#define TRT_H_


#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec3d.h>
#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec2d.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSubField.h>




namespace trt
{
	#if unix//( defined(Q_WS_X11) || defined(Q_WS_MAC) )
		typedef SoSFDouble TONATIUH_REAL;
		typedef SoMFVec3d TONATIUH_CONTAINERREALVECTOR3;
		typedef SoSFVec3d TONATIUH_REALVECTOR3;
		typedef SoSFVec2d TONATIUH_REALVECTOR2;

	#else
		typedef SoSFFloat TONATIUH_REAL;
		typedef SoMFVec3f TONATIUH_CONTAINERREALVECTOR3;
		typedef SoSFVec3f TONATIUH_REALVECTOR3;
		typedef SoSFVec2f TONATIUH_REALVECTOR2;
	#endif

	typedef SoSFBool TONATIUH_BOOL;

};


#endif /* trt_H_ */
