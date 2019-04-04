/*
 * CodeEditArea.h
 *
 *  Created on: 14/04/2011
 *      Author: amutuberria
 */

#ifndef CODEEDITAREA_H_
#define CODEEDITAREA_H_

#include <QPlainTextEdit>

class CodeEditArea : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditArea( QWidget* parent = 0 );
	~CodeEditArea();

	int BlockTop( const QTextBlock & block ) const;
	int BlockHeight( const QTextBlock & block ) const;

//public slots:
	//void UpdateCodeEditAreaWidth( int width );

//protected:
	//void resizeEvent(QResizeEvent* event );

private slots:
	void UpdateCurrentLine();

//private:
	//LineNumberWidget* m_LineNumberWidget;
};

#endif /* CODEEDITAREA_H_ */
