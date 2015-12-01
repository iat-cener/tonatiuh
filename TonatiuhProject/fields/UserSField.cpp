/*
 * UserSField.cpp
 *
 *  Created on: 01/12/2015
 *      Author: amutuberria
 */



#include "UserSField.h"

SoType UserSField::classTypeId STATIC_SOTYPE_INIT;


/*!
 * UserSField
 */
SoType UserSField::getClassTypeId(void)
{
	return UserSField::classTypeId;
}

UserSField::~UserSField()
{
}


void UserSField::initClass(void)
{
  PRIVATE_FIELD_INIT_CLASS(UserSField, "UserSField", inherited, NULL);
}

void UserSField::atexit_cleanup(void)
{
  SoType::removeType(UserSField::classTypeId.getName());
  UserSField::classTypeId STATIC_SOTYPE_INIT;
}
