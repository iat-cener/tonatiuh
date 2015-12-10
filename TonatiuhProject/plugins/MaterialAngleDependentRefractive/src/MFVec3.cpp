/*
 * MFVec2.cpp
 *
 *  Created on: 27/11/2015
 *      Author: amutuberria
 */

#include "MFVec3.h"

#if unix//( defined(Q_WS_X11) || defined(Q_WS_MAC) )
	SO_MFIELD_SOURCE_MALLOC(MFVec3, SbVec3d, const SbVec3d &);

	void MFVec3::initClass()
	{
		// This macro takes the name of the class and the name of the
		// parent class
			SO_MFIELD_INIT_CLASS(MFVec3, SoMField);
	}


	SbBool MFVec3::read1Value(SoInput * in, int idx)
	{
	    in->read(this->values[idx][0]) &&
	    in->read(this->values[idx][1]) &&
	    in->read(this->values[idx][2]);
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
		MFVec3->set1Value(idx, SbVec3d(x, y, z));
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
