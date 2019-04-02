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

#ifndef TRANSMISSIVITYFACTORYBALLESTRIN_H_
#define TRANSMISSIVITYFACTORYBALLESTRIN_H_

#include "TransmissivityBallestrin.h"
#include "TTransmissivityFactory.h"

class TransmissivityBallestrin;

class TransmissivityBallestrinFactory : public QObject, public TTransmissivityFactory
{
    Q_OBJECT
    Q_INTERFACES( TTransmissivityFactory )
#if QT_VERSION >= 0x050000 // pre Qt 5
    Q_PLUGIN_METADATA(IID "tonatiuh.TTransmissivityFactory")
#endif

public:
    QString TTransmissivityName() const;
    QIcon TTransmissivityIcon() const;
    TransmissivityBallestrin* CreateTTransmissivity( ) const;
};

#endif /* TRANSMISSIVITYBALLESTRIN_H_ */
