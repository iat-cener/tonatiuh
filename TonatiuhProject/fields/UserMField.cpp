/*
 * UserMField.cpp
 *
 *  Created on: 01/12/2015
 *      Author: amutuberria
 */

#include <iostream>
#include "UserMField.h"

SoType UserMField::classTypeId STATIC_SOTYPE_INIT;


/*!
 * UserMField
 */
SoType UserMField::getClassTypeId(void)
{
	return UserMField::classTypeId;
}

UserMField::~UserMField()
{
}


void UserMField::initClass(void)
{
	std::cout<<"UserMField::inherited: "<<inherited::getClassTypeId().getName().getString()<<std::endl;
	std::cout<<"UserMField::inherited 2 : "<<UserMField::getClassTypeId().getName().getString()<<std::endl;
	std::cout<<"UserMField::inherited 3 : "<<UserMField::getClassTypeId().getKey()<<std::endl;
	//std::cout<<"UserMField::inherited: "<<SoType::badType().getClassTypeId().getName().getString()<<std::endl;
	PRIVATE_FIELD_INIT_CLASS(UserMField, "UserMField", inherited, NULL);
}

void UserMField::atexit_cleanup(void)
{
  SoType::removeType(UserMField::classTypeId.getName());
  UserMField::classTypeId STATIC_SOTYPE_INIT;
}



