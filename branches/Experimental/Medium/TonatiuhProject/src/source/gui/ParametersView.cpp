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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <QString>
#include <QTabWidget>
#include <QVBoxLayout>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "FieldContainerWidget.h"
#include "ParametersView.h"

ParametersView::ParametersView( QWidget* parent , Qt::WindowFlags f )
: QWidget(parent, f), m_ptabWidget( 0 ), m_actualCoinNode( 0 )
{
	m_ptabWidget = new QTabWidget;
    m_ptabWidget->addTab( new FieldContainerWidget( 0, "" ), tr("Transform") );
    m_ptabWidget->addTab( new FieldContainerWidget( 0, "" ), tr("Shape") );

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget( m_ptabWidget );
    setLayout( layout );

}

ParametersView::~ParametersView()
{
	delete m_ptabWidget;
}
void ParametersView::ChangeParameters( SoNode* coinNode )
{
	m_ptabWidget->clear();
	m_actualCoinNode = coinNode;

	if ( coinNode )
	{
		if(coinNode->getTypeId().isDerivedFrom( SoBaseKit::getClassTypeId() ) )
		{
			QString type = coinNode->getTypeId().getName().getString();

			if ( type == "TLightKit" )
			{
				QStringList parts;
				parts<<QString( "transform" )<<QString( "icon" )<<QString( "tsunshape" );

				SelectionChanged( coinNode, parts );
			}
			else
			{
				QStringList parts;
				if( type == "TShapeKit" )
				{
					parts<<QString( "shape" )<<QString( "appearance.material" );
				}
				else
					parts<<QString( "transform" );
				SelectionChanged( coinNode, parts );
			}
		}
		else
		{
			QString typeName = coinNode->getTypeId().getName().getString();
			AddTab( coinNode, typeName );
		}
	}
	else
	{
		m_ptabWidget->addTab( new FieldContainerWidget( 0, ""  ), tr("Transform") );
	}
}

void ParametersView::SelectionChanged( SoNode* coinNode, QStringList parts )
{
	SoBaseKit* nodeKit = static_cast< SoBaseKit* > ( coinNode );
	for( int i = 0; i< parts.size(); ++i )
	{
		QString partName = parts[i];
		SoNode* coinPart = nodeKit->getPart(partName.toStdString().c_str(), false );
		if( coinPart ) AddTab( coinPart, partName );
	}
}

void ParametersView::AddTab( SoNode* coinNode, QString partName )
{
	QString type = coinNode->getTypeId().getName().getString();

	FieldContainerWidget* nodeContainer = new FieldContainerWidget( coinNode, partName );
	m_ptabWidget->addTab( nodeContainer, type );
	connect(nodeContainer, SIGNAL( valueModificated( const QStringList& , QString ) ), this, SLOT( valueModification( const QStringList& , QString ) ) );
}

void ParametersView::valueModification( const QStringList& oldValuesList, QString containerName )
{
	emit valueModificated( oldValuesList, m_actualCoinNode, containerName );
}

void ParametersView::Reset()
{
	FieldContainerWidget* containerWidget = static_cast< FieldContainerWidget* >(m_ptabWidget->currentWidget() );
	if( containerWidget )containerWidget->Reset();
}
