/*
 * UserMField.cpp
 *
 *  Created on: 01/12/2015
 *      Author: amutuberria
 */

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
	PRIVATE_FIELD_INIT_CLASS(UserMField, "UserMField", inherited, NULL);
}

void UserMField::atexit_cleanup(void)
{
  SoType::removeType(UserMField::classTypeId.getName());
  UserMField::classTypeId STATIC_SOTYPE_INIT;
}



