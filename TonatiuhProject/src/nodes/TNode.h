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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel Blanco,
at the time Chair of the Department of Engineering of the University of Texas
at Brownsville. From May 2004 to August 2008 Tonatiuh's development was
supported by the Department of Energy (DOE) and the National Renewable
Energy Laboratory (NREL) under the Minority Research Associate (MURA)
Program Subcontract ACQ-4-33623-06. During 2007, NREL also contributed to
the validation of Tonatiuh under the framework of the Memorandum of
Understanding signed with the Spanish National Renewable Energy Centre (CENER)
on February, 20, 2007 (MOU#NREL-07-117). Since June 2006, the development of
Tonatiuh is being led by CENER, under the direction of Dr. Blanco, now
Manager of the Solar Thermal Energy Department of CENER.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef TNODE_H_
#define TNODE_H_

#include <QObject>
#include <QMetaType>

#include "TNodeType.h"

class TParameterList;


//!  TNode class is the base class for all Tonatiuh nodes.
/*!
  TNode class defines all the basic parameters and methods for the nodes to define a concentration system.
*/

/** Base class of all Tonatiuh nodes: TNode */
class TNode : public QObject
{
	Q_OBJECT

private:
	Q_DISABLE_COPY(TNode)

public:
	static void Init();

	TNode();
	template<class T> const T* as() const;
	template<class T> T* as();

	virtual TNode* Copy() const;

	int GetID() const;
	QString GetName() const;
	QStringList GetVisibleParametersName() const;
	QVariant GetParameterValue( QString name ) const;
	int GetReferences() const;
	virtual TNodeType GetType() const;
	void IncreaseReference();
	void RemoveReference();

	void SetName( QString name );
	bool SetParameterValue( const QString& name, const QVariant& value );


protected:
	TParameterList* m_parametersList;
	virtual ~TNode();

private:
	int m_id;
	int m_referenceCount;
	static TNodeType m_nodeType;

};

Q_DECLARE_METATYPE(TNode*)


template<class T>
const T* TNode::as() const
{
  const T* t = qobject_cast<const T*>(this);
  return t;
}


template<class T>
T* TNode::as()
{
  T* t = qobject_cast<T*>(this);
  return t;
}



#endif /* TNODE_H_ */
