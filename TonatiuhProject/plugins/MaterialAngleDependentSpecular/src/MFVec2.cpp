/*
 * MFVec2.cpp
 *
 *  Created on: 27/11/2015
 *      Author: amutuberria
 */

#include "MFVec2.h"

#if unix//( defined(Q_WS_X11) || defined(Q_WS_MAC) )
	SO_MFIELD_SOURCE_MALLOC(MFVec2, SbVec2d, const SbVec2d &);

	void MFVec2::initClass()
	{
		// This macro takes the name of the class and the name of the
		// parent class
			SO_MFIELD_INIT_CLASS(MFVec2, UserMField);
	}


	SbBool MFVec2::read1Value(SoInput * in, int idx)
	{
	    in->read(this->values[idx][0]) &&
	    in->read(this->values[idx][1]);
	}

	void MFVec2::write1Value(SoOutput * out, int idx) const
	{
		const SbVec2d & v = (*this)[idx];

		out->write(v[0]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[1]);
	}


	void MFVec2::setValues(int start, int numarg, const double xy[][2])
	{
		if (start+numarg > this->maxNum)
			this->allocValues(start+numarg);
		else if (start+numarg > this->num)
			this->num = start+numarg;

		for(int i=0; i < numarg; i++)
			this->values[start+i] = SbVec2d(xy[i]);

	  this->valueChanged();
	}

	void MFVec2::set1Value(int idx, double x, double y)
	{
		this->set1Value(idx, SbVec2d(x, y));
	}


	void MFVec2::set1Value(int idx, const double xy[2])
	{
		this->set1Value(idx, SbVec2d(xy));
	}

	void MFVec2::setValue(double x, double y)
	{
		this->setValue(SbVec2d(x, y));
	}


	void MFVec2::setValue(const double xy[2])
	{
		if (xy == NULL) this->setNum(0);
		else this->setValue(SbVec2d(xy));
	}

	QStringList MFVec2::GetNames( ) const
	{
		QStringList namesList;
		namesList<< m_firstDimensionName
				<< m_secondtDimensionName;
		return ( namesList );
	}
	void MFVec2::SetNames( QString firstDimension, QString secondDimension )
	{
		m_firstDimensionName = firstDimension;
		m_secondtDimensionName = secondDimension;

	}

#else
	SO_MFIELD_SOURCE_MALLOC(MFVec2, SbVec2f, const SbVec2f &);

	void MFVec2::initClass()
	{
		// This macro takes the name of the class and the name of the
		// parent class
			SO_MFIELD_INIT_CLASS(MFVec2, inherited);
	}

	SbBool MFVec2::read1Value(SoInput * in, int idx)
	{
	    in->read(this->values[idx][0]) &&
	    in->read(this->values[idx][1]);
	}

	void MFVec2::write1Value(SoOutput * out, int idx) const
	{
		const SbVec2f & v = (*this)[idx];

		out->write(v[0]);
		if (!out->isBinary())
			out->write(' ');
		out->write(v[1]);
	}


	void MFVec2::setValues(int start, int numarg, const float xy[][2])
	{
		if (start+numarg > this->maxNum)
			this->allocValues(start+numarg);
		else if (start+numarg > this->num)
			this->num = start+numarg;

		for(int i=0; i < numarg; i++)
			this->values[start+i] = SbVec2f(xy[i]);

	  this->valueChanged();
	}

	void MFVec2::set1Value(int idx, float x, float y)
	{
		this->set1Value(idx, SbVec2f(x, y));
	}


	void MFVec2::set1Value(int idx, const float xy[2])
	{
		this->set1Value(idx, SbVec2f(xy));
	}

	void MFVec2::setValue(float x, float y)
	{
		this->setValue(SbVec2f(x, y));
	}


	void MFVec2::setValue(const float xy[2])
	{
		if (xy == NULL) this->setNum(0);
		else this->setValue(SbVec2f(xy));
	}

	QStringList MFVec2::GetNames( ) const
	{
		QStringList namesList;
		namesList<< m_firstDimensionName
				<< m_secondtDimensionName;
		return ( namesList );
	}
	void MFVec2::SetNames( QString firstDimension, QString secondDimension )
	{
		m_firstDimensionName = firstDimension;
		m_secondtDimensionName = secondDimension;

	}
#endif

