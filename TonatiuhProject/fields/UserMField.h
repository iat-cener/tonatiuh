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

#ifndef USERMFIELD_H_
#define USERMFIELD_H_

#include <QStringList>

#include <Inventor/fields/SoSubField.h>
#include <Inventor/fields/SoMField.h>

class FieldEditor;

class UserMField: public SoMField
{
	typedef SoMField inherited;

public:
	virtual ~UserMField();
	static void initClass(void);
	static SoType getClassTypeId(void);
	static void atexit_cleanup(void);
	virtual FieldEditor* GetEditor() const = 0;

	QStringList GetNames( ) const
	{
		return ( m_dimensionsNames );
	}

	void SetNames( QStringList dimensionsNames )
	{
		m_dimensionsNames = dimensionsNames;
	}

private:
	QStringList m_dimensionsNames;
	FieldEditor* m_pEditor;
	static SoType classTypeId;
};



#endif /* USERMFIELD_H_ */
