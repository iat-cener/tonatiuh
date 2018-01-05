/*
 * NodeLibrary.h
 *
 *  Created on: 25 oct. 2017
 *      Author: amutuberria
 */

#ifndef NODELIBRARY_H_
#define NODELIBRARY_H_


#ifdef WIN32
	#ifdef NODELIBRARY_EXPORTS
		#define NODE_API __declspec(dllexport)
	#else
		#define NODE_API __declspec(dllimport)
	#endif
#else
	#define NODE_API
#endif


#endif /* NODELIBRARY_H_ */
