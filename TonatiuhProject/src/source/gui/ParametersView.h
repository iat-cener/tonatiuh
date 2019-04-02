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

#ifndef PARAMETERSVIEW_H_
#define PARAMETERSVIEW_H_

#include <QTabWidget>

class SoBaseKit;
class SoNode;

//! The ParametersView class provides a tabbed view for nodes parameters.
/*!
 * ParametersView provides a page for each of the parts of a node. The tab bar shows the node part name and the tab area shows the part parameters list.
 *
 * The tab area containes a FieldContainerWidget.
 *
 * \sa FieldContainerWidget
 *
*/

class ParametersView : public QTabWidget
{
	Q_OBJECT

public:
	ParametersView( QWidget* parent = 0 );
	~ParametersView();

	void SelectionChangedToPart( SoNode* coinPart );
	void SelectionChangedToKit( SoBaseKit* coinNode );
	void UpdateView();

signals:
	 /*!
	 * Emmits a signal when a parameters of the view is modified.
	 *
	 * The modified paramenters is a \a coinNode paramenter named \a parameterName.
	 *
	 * The new value for the parameter is \a newValue.
	 * */
	 void valueModificated( SoNode* coinNode, QString parameterName, QString newValue );

public slots:
	void SetValue( SoNode* node, QString paramenterName, QString newValue );

private:
	void AddTab( SoNode* coinNode, QString partName );
	QStringList ContainerNodeParts( SoBaseKit* coinNode );

	SoNode* m_actualCoinNode;
	bool m_isPart;

};

#endif /*PARAMETERSVIEW_H_*/
