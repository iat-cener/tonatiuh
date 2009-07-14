/*
 * MainWindow.h
 *
 *  Created on: Jul 14, 2009
 *      Author: mblanco
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <ui_MainWindow.h>


class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
	MainWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
    ~MainWindow();

public slots:
	//File menu actions
	void on_actionOpen_triggered();

private:
	bool StartOver( const QString& fileName );
	bool OkToContinue();
};

#endif /* MAINWINDOW_H_ */
