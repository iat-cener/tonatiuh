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
 
#ifndef TSHAPEFACTORY_H_
#define TSHAPEFACTORY_H_

#include <QtPlugin>
#include <QVector>
#include <QVariant>

class QString;
class QIcon;
class TShape;

class TShapeFactory
{
public:
    virtual ~TShapeFactory() {}
    virtual QString TShapeName() const  = 0;
    virtual QIcon TShapeIcon() const = 0;
    virtual TShape* CreateTShape( ) const = 0;
    virtual TShape* CreateTShape( int /*numberofParameters*/, QVector< QVariant > /*parametersList*/ ) const
    {
    	return ( CreateTShape() );
    }
    virtual bool IsFlat() = 0;
};

Q_DECLARE_INTERFACE( TShapeFactory, "tonatiuh.TShapeFactory")

#endif /*UISHAPEFACTORY_H_*/
