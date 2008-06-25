/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
#include <Inventor/nodes/SoNode.h> 
#include <QVBoxLayout>

#include "FieldContainerWidget.h"
#include "ParametersDelegate.h"
#include "ParametersView.h"
#include "Trace.h"
#include "TShapeKit.h"

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
	Trace trace( "ParametersView::~ParametersView", false );
	
}
void ParametersView::ChangeParameters( SoBaseKit* coinNode )
{
	Trace trace( "ParametersView::ChangeParameters", false );
	
	if ( coinNode )
	{
		QString type = coinNode->getTypeId().getName().getString();
		
		SoBaseKit* nodeKit = static_cast< SoBaseKit* >( coinNode );
		if ( type == "TLightKit" )
		{
			QStringList parts;
			parts<<QString( "transform" )<<QString( "icon" )<<QString( "tsunshape" );
			
			SelectionChanged( nodeKit, parts, false );
	
				
		}
		else	
		{
			QStringList parts;
			parts<<QString( "transform" );
			
			SelectionChanged( nodeKit, parts );
			
			if( type == "TShapeKit" )  
			{
				parts<<QString( "shape" )<<QString( "appearance.material" );
			}
			SelectionChanged( nodeKit, parts );
		}
		

	}
	else
	{
		m_ptabWidget->addTab( new FieldContainerWidget( 0, ""  ), tr("Transform") );
	}
}

void ParametersView::SelectionChanged( SoBaseKit* coinNode, QStringList parts, bool editable )
{
	Trace trace( "ParametersView::SelectionChanged", false );
	
	m_ptabWidget->clear();
	m_actualCoinNode = coinNode;
	
	for( int i = 0; i< parts.size(); i++ )
	{
		QString partName = parts[i];
		SoNode* coinPart = coinNode->getPart(partName.toStdString().c_str(), false );
		if( coinPart ) AddTab( coinPart, partName, editable );
	}
}

void ParametersView::AddTab( SoNode* coinNode, QString partName, bool editable )
{
	Trace trace( "ParametersView::AddTab", false );
	
	QString type = coinNode->getTypeId().getName().getString();
	
	FieldContainerWidget* nodeContainer = new FieldContainerWidget( coinNode, partName, this );
	nodeContainer->SetEditable( editable );
	m_ptabWidget->addTab( nodeContainer, type );
	connect(nodeContainer, SIGNAL( valueModificated( const QStringList& , QString ) ), this, SLOT( valueModification( const QStringList& , QString ) ) );	
		
}

void ParametersView::valueModification( const QStringList& oldValuesList, QString containerName )
{
	Trace trace( "ParametersView::valueModification", false );
	emit valueModificated( oldValuesList, m_actualCoinNode, containerName );
}

void ParametersView::Reset()
{
	Trace trace( "ParametersView::Reset", false );
	FieldContainerWidget* containerWidget = static_cast< FieldContainerWidget* >(m_ptabWidget->currentWidget() );
	if( containerWidget )containerWidget->Reset();
}
