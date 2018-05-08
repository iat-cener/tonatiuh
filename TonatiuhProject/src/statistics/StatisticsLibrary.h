/*
 * StatisticsLibrary.h
 *
 *  Created on: 24 oct. 2017
 *      Author: amutuberria
 */

#ifndef STATISTICSLIBRARY_H_
#define STATISTICSLIBRARY_H_


#ifdef WIN32
	#ifdef STATISTICSLIBRARY_EXPORTS
		#define STATISTICS_API __declspec(dllexport)
	#else
		#define STATISTICS_API __declspec(dllimport)
	#endif
#else
	#define STATISTICS_API
#endif



#endif /* STATISTICSLIBRARY_H_ */
