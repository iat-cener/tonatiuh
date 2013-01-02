/*
 * PhotonMapExportNull.h
 *
 *  Created on: 17/07/2012
 *      Author: amutuberria
 */

#ifndef PHOTONMAPEXPORTNULLFACTORY_H_
#define PHOTONMAPEXPORTNULLFACTORY_H_

#include "PhotonMapExportFactory.h"
#include "PhotonMapExportNull.h"
#include "PhotonMapExportParametersWidget.h"

class PhotonMapExportNullFactory: public QObject, public PhotonMapExportFactory
{
    Q_OBJECT
    Q_INTERFACES(PhotonMapExportFactory)

public:
   	QString GetName() const;
   	QIcon GetIcon() const;
   	PhotonMapExportNull* GetExportPhotonMapMode() const;
   	PhotonMapExportParametersWidget* GetExportPhotonMapModeWidget() const;
};


#endif /* PHOTONMAPEXPORTNULLFACTORY_H_ */
