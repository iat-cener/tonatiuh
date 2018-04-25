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

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#ifndef SUNSHAPEBUIE_H_
#define SUNSHAPEBUIE_H_

#include "TNodeType.h"
#include "TSunshape.h"

class SunshapeBuie : public TSunshape
{

private:
	//No copy constructor. Use Copy to create a copy of the node
	SunshapeBuie(const SunshapeBuie&) = delete;
	SunshapeBuie& operator=(const SunshapeBuie&) = delete;
public:
	static std::shared_ptr< TNode > CreateInstance();
	static void Init();

	std::shared_ptr< TNode > Copy() const;

	void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand ) const;
	std::string GetIcon() const;
	double GetIrradiance() const;
    double GetThetaMax() const;
	TNodeType GetType() const;

protected:
	SunshapeBuie( );
	virtual ~SunshapeBuie();

	double PDFTheta( double theta ) const;
	double Phi( double theta ) const;
	double PhiCircumSolarRegion( double theta ) const;
	double PhiSolarDisk( double theta ) const;
	//void UpdateState( double csrValue );
	bool UpdateState( );
	double ZenithAngle( RandomDeviate& rand ) const;

private:
	static TNodeType m_nodeType;
	std::string m_irradianceLabel;
	std::string m_csrLabel;

	 const double m_thetaCS;
	 const double m_thetaSD;
	 const double m_integralA;
	 double m_alpha;
	 double m_deltaThetaCSSD;
	 double m_etokTimes1000toGamma;
	 double m_gamma;
	 double m_heightRectangle1;
	 double m_heightRectangle2;
	 double m_probabilityRectangle1;

	 static const double m_minCRSValue;// = 0.001;
	 static const double m_maxCRSValue;// = 0.8;
};


#endif /* SUNSHAPEBUIE_H_ */
