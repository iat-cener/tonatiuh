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

#ifndef MFVEC2_H_
#define MFVEC2_H_

#include <QStringList>

#include <Inventor/SbVec2d.h>

#include <Inventor/fields/SoSubField.h>

#include "ContainerEditorMFVec2.h"
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
			ContainerEditorMFVec2* editor = new ContainerEditorMFVec2();
			editor->SetTitles( GetNames() );
			return (editor) ;
		}

	private:
		QString m_firstDimensionName;
		QString m_secondtDimensionName;
		FieldEditor* m_pEditor;
	};

#else

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
			ContainerEditorMFVec2* editor = new ContainerEditorMFVec2();
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
