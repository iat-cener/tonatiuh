/*
 * GeometryLibrary.h
 *
 *  Created on: 24 oct. 2017
 *      Author: amutuberria
 */

#ifndef GEOMETRYLIBRARY_H_
#define GEOMETRYLIBRARY_H_


#ifdef WIN32
	#ifdef GEOMETRYLIBRARY_EXPORTS
		#define GEOMETRY_API __declspec(dllexport)
	#else
		#define GEOMETRY_API __declspec(dllimport)
	#endif
#else
	#define GEOMETRY_API
#endif



#endif /* GEOMETRYLIBRARY_H_ */
