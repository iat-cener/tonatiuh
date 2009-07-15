/*
 * MainWindow.h
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_


#include <QMainWindow>

#include <ui_MainWindow.h>

class PhotonMapAnalysis;

class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
	MainWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
    ~MainWindow();

public slots:

	//File menu actions
	void on_actionOpen_triggered();

private slots:
	void on_openButton_clicked();
	void on_saveButton_clicked();

	void RunAnalysis( bool );

private:
	bool StartOver( const QString& fileName );
	bool OkToContinue();

private:
	QList< PhotonMapAnalysis* > m_modelAnalysisList;
};

#endif /* MAINWINDOW_H_ */
