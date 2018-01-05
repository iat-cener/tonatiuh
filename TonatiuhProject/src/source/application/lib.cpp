/*
 * lib.cpp
 *
 *  Created on: 25 oct. 2017
 *      Author: amutuberria
 */

#include <any>
#include <iostream>

#include "Vector3D.h"

void do_stuff_with_any(const std::any& obj)
{
	if( obj.type() ==typeid( std::string) )
		std::cout << std::any_cast<const std::string&>(obj) << "\n";
	else if( obj.type() ==typeid( Vector3D ) )
	{
		std::cout << obj.type().name() << "\n";
		std::cout << typeid( Vector3D ).name()  << "\n";
		std::cout << &obj.type() << "\n";
		std::cout << &typeid( Vector3D )  << "\n";

		std::cout << std::any_cast<const Vector3D&>(obj) << "\n";
	}
}

