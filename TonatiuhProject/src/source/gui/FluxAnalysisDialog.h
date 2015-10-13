/*
 * FluxAnalysisDialog.h
 *
 *  Created on: 08/10/2015
 *      Author: amutuberria
 */

#ifndef FLUXANALYSISDIALOG_H_
#define FLUXANALYSISDIALOG_H_

#include <QDialog>

#include "ui_fluxanalysisdialog.h"

//class PhotonMapExportFactory;
class SceneModel;


class FluxAnalysisDialog: public QDialog, private Ui::FluxAnalysisDialog
{
	Q_OBJECT

public:
	FluxAnalysisDialog( SceneModel& currentSceneModel, /* QVector< PhotonMapExportFactory* > typeList,*/ QWidget* parent = 0 );
	~FluxAnalysisDialog();

private slots:
	void SelectSurface();

private:
	SceneModel* m_pCurrentSceneModel;
};


#endif /* FLUXANALYSISDIALOG_H_ */
