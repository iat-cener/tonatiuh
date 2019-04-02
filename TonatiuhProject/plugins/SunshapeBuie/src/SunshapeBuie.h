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


#ifndef SUNSHAPEBUIE_H_
#define SUNSHAPEBUIE_H_

#include "TSunShape.h"
#include "trt.h"

class SoSensor;
class SoFieldSensor;

class SunshapeBuie : public TSunShape
{
	SO_NODE_HEADER(SunshapeBuie);

public:
	SunshapeBuie( );
    static void initClass();
	SoNode* copy( SbBool copyConnections ) const;

    //Sunshape Interface
    void GenerateRayDirection( Vector3D& direction, RandomDeviate& rand) const;
	double GetIrradiance() const;
    double GetThetaMax() const;

	trt::TONATIUH_REAL irradiance;
	trt::TONATIUH_REAL csr;

protected:
	static void updateCSR(void *data, SoSensor *);
	 ~SunshapeBuie();
private:
	 double chiValue( double csr ) const;
	 double phiSolarDisk( double theta ) const;
	 double phiCircumSolarRegion( double theta ) const;
	 double phi( double theta ) const;
	 double pdfTheta( double theta ) const;
	 double zenithAngle( RandomDeviate& rand ) const;
	 double kValue( double chi ) const;
	 double gammaValue( double chi ) const;
	 double intregralB( double k, double gamma, double thetaCS, double thetaSD ) const;
	 double probabilityRectangle1( double widthR1, double heightR1, double widthR2, double heightR2 ) const;
	 void updateState( double csrValue );

	 SoFieldSensor* m_csrSensor;

	 double m_chi;
	 double m_k;
	 double m_gamma;
	 double m_etokTimes1000toGamma;
	 double m_thetaSD;
	 double m_thetaCS;
	 double m_deltaThetaCSSD;
	 double m_integralA;
	 double m_integralB;
	 double m_alpha;
	 double m_heightRectangle1;
	 double m_heightRectangle2;
	 double m_probabilityRectangle1;
	 static const double m_minCRSValue;// = 0.001;
	 static const double m_maxCRSValue;// = 0.8;
};

#endif /* SUNSHAPEBUIE_H_ */
