/*
 * v.h
 *
 *  Created on: 27/11/2015
 *      Author: amutuberria
 */

#ifndef MFVEC2_H_
#define MFVEC2_H_

#include <QStringList>

#include <Inventor/SbVec2d.h>

#include <Inventor/fields/SoSubField.h>

#include "ContainerEditor.h"
#include "FieldEditor.h"
#include "UserMField.h"


#if unix//( defined(Q_WS_X11) || defined(Q_WS_MAC) )

	class MFVec2 : public UserMField
	{
		  typedef UserMField inherited;
		  SO_MFIELD_HEADER( MFVec2, SbVec2d, const SbVec2d &);



	public:
	  static void initClass(void);

	  void setValues(int start, int num, const double xy[][2]);
	  void set1Value(int idx, double x, double y);
	  void set1Value(int idx, const double xy[2]);
	  void setValue(double x, double y);
	  void setValue(const double xy[2]);


		QStringList GetNames( ) const;
		void SetNames( QString firstDimension, QString secondDimension );

		FieldEditor* GetEditor() const
		{
			ContainerEditor* editor = new ContainerEditor();
			editor->SetTitles( GetNames() );
			return (editor) ;
		}

	private:
		QString m_firstDimensionName;
		QString m_secondtDimensionName;
		FieldEditor* m_pEditor;
	};

#else if

	#include <Inventor/SbVec2f.h>
	class MFVec2 : public UserMField
	{
		typedef UserMField inherited;

		  SO_MFIELD_HEADER(MFVec2, SbVec2f, const SbVec2f &);

	public:
		  static void initClass(void);

		  void setValues(int start, int num, const float xy[][2]);
		  void set1Value(int idx, float x, float y);
		  void set1Value(int idx, const float xy[2]);
		  void setValue(float x, float y);
		  void setValue(const float xy[2]);


		QStringList GetNames( ) const;
		void SetNames( QString firstDimension, QString secondDimension );

		FieldEditor* GetEditor() const
		{
			ContainerEditor* editor = new ContainerEditor();
			editor->SetTitles( GetNames() );
			return (editor) ;
		}

	private:
		QString m_firstDimensionName;
		QString m_secondtDimensionName;
		FieldEditor* m_pEditor;
	};

#endif


#endif /* MFVEC2_H_ */
