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


#ifndef PARALLELRANDOMDEVIATE_H_
#define PARALLELRANDOMDEVIATE_H_

#include <QMutex>
#include <QObject>

#include "RandomDeviate.h"


class ParallelRandomDeviate :  public QObject, public RandomDeviate
{
	Q_OBJECT

public:
	ParallelRandomDeviate( RandomDeviate* rand, QMutex* mutex, unsigned long arraySize = 100000, QObject* parent = 0 );
	virtual ~ParallelRandomDeviate( );
    void FillArray( double* array, const unsigned long arraySize );

private:
    RandomDeviate* m_pRand;

    QMutex* m_mutex;

};

#endif /* PARALLELRANDOMDEVIATE_H_ */
