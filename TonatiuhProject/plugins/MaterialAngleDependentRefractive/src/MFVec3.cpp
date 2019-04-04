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

#include "MFVec3.h"

#if unix//( defined(Q_WS_X11) || defined(Q_WS_MAC) )
	SO_MFIELD_SOURCE_MALLOC(MFVec3, SbVec3d, const SbVec3d &);

	void MFVec3::initClass()
	{
		// This macro takes the name of the class and the name of the
		// parent class
			SO_MFIELD_INIT_CLASS(MFVec3, UserMField);
	}


	SbBool MFVec3::read1Value(SoInput * in, int idx)
	{
	   return ( in->read(this->values[idx][0]) &&
	    in->read(this->values[idx][1]) &&
	    in->read(this->values[idx][2]) );
	}

	void MFVec3::write1Value(SoOutput * out, int idx) const
	{
		const SbVec3d & v = (*this)[idx];

		out->write(v[0]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[1]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[2]);
	}


	void MFVec3::setValues(int start, int numarg, const double xyz[][3])
	{
		if (start+numarg > this->maxNum)
			this->allocValues(start+numarg);
		else if (start+numarg > this->num)
			this->num = start+numarg;

		for(int i=0; i < numarg; i++)
			this->values[start+i] = SbVec3d(xyz[i]);

	  this->valueChanged();
	}

	void MFVec3::set1Value(int idx, double x, double y, double z)
	{
		this->set1Value(idx, SbVec3d(x, y, z));
	}


	void MFVec3::set1Value(int idx, const double xyz[3])
	{
		this->set1Value(idx, SbVec3d(xyz));
	}

	void MFVec3::setValue(double x, double y, double z)
	{
		this->setValue(SbVec3d(x, y, z));
	}


	void MFVec3::setValue(const double xyz[3])
	{
		if (xyz == NULL) this->setNum(0);
		else this->setValue(SbVec3d(xyz));
	}

	QStringList MFVec3::GetNames( ) const
	{
		QStringList namesList;
		namesList<< m_firstDimensionName
				<< m_secondtDimensionName
				<< m_thirdDimensionName;
		return ( namesList );
	}
	void MFVec3::SetNames( QString firstDimension, QString secondDimension, QString thirdDimension )
	{
		m_firstDimensionName = firstDimension;
		m_secondtDimensionName = secondDimension;
		m_thirdDimensionName = thirdDimension;

	}

#else
	SO_MFIELD_SOURCE_MALLOC(MFVec3, SbVec3f, const SbVec3f &);

	void MFVec3::initClass()
	{
		// This macro takes the name of the class and the name of the
		// parent class
			SO_MFIELD_INIT_CLASS(MFVec3, inherited);
	}

	SbBool MFVec3::read1Value(SoInput * in, int idx)
	{
	    in->read(this->values[idx][0]) &&
	    in->read(this->values[idx][1]) &&
	    in->read(this->values[idx][2]);
	}

	void MFVec3::write1Value(SoOutput * out, int idx) const
	{
		const SbVec3f & v = (*this)[idx];

		out->write(v[0]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[1]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[2]);
	}


	void MFVec3::setValues(int start, int numarg, const float xyz[][3])
	{
		if (start+numarg > this->maxNum)
			this->allocValues(start+numarg);
		else if (start+numarg > this->num)
			this->num = start+numarg;

		for(int i=0; i < numarg; i++)
			this->values[start+i] = SbVec3f(xyz[i]);

	  this->valueChanged();
	}

	void MFVec3::set1Value(int idx, float x, float y, float z)
	{
		this->set1Value(idx, SbVec3f(x, y, z));
	}


	void MFVec3::set1Value(int idx, const float xyz[3])
	{
		this->set1Value(idx, SbVec3f(xyz));
	}

	void MFVec3::setValue(float x, float y, float z)
	{
		this->setValue(SbVec3f(x, y, z));
	}


	void MFVec3::setValue(const float xyz[3])
	{
		if (xyz == NULL) this->setNum(0);
		else this->setValue(SbVec3f(xyz));
	}

	QStringList MFVec3::GetNames( ) const
	{
		QStringList namesList;
		namesList<< m_firstDimensionName
				<< m_secondtDimensionName
				<< m_thirdDimensionName;
		return ( namesList );
	}
	void MFVec3::SetNames( QString firstDimension, QString secondDimension, QString thirdDimension )
	{
		m_firstDimensionName = firstDimension;
		m_secondtDimensionName = secondDimension;
		m_thirdDimensionName = thirdDimension;

	}
#endif
