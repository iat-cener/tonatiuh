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



