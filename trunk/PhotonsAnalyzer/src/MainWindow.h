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

private slots:
	void on_openButton_clicked();
	void on_saveButton_clicked();

	void RunAnalysis( bool );

protected:
	void closeEvent( QCloseEvent* event );

private:
	void ReadSettings();
	void WriteSettings();

private:
	QList< PhotonMapAnalysis* > m_modelAnalysisList;
};

#endif /* MAINWINDOW_H_ */
