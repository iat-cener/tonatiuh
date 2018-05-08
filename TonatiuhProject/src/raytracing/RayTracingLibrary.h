/*
 * RayTracingLibrary.h
 *
 *  Created on: 24 oct. 2017
 *      Author: amutuberria
 */

#ifndef RAYTRACINGLIBRARY_H_
#define RAYTRACINGLIBRARY_H_


#ifdef WIN32
	#ifdef RAYTRACINGLIBRARY_EXPORTS
		#define RAYTRACING_API __declspec(dllexport)
	#else
		#define RAYTRACING_API __declspec(dllimport)
	#endif
#else
	#define RAYTRACING_API
#endif



#endif /* RAYTRACINGLIBRARY_H_ */
