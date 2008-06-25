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
 
#ifndef GRAPHICVIEW_H_
#define GRAPHICVIEW_H_

#include <QAbstractItemView>

class SoSelection;
class QModelIndex;
class SoQtExaminerViewer;
class SbViewportRegion;
class TSeparatorKit;
#include <Inventor/nodes/SoCamera.h>
 
class GraphicView : public QAbstractItemView
{
    Q_OBJECT

public:
    GraphicView( QWidget* parent = 0 );
    void SetSceneGraph( SoSelection* sceneGraphRoot );
    SbViewportRegion GetViewportRegion() const;
    SoCamera* GetCamera() const;
    virtual QModelIndex indexAt ( const QPoint & point ) const;
    virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible );
    virtual QRect visualRect ( const QModelIndex & index ) const;
    
    void ViewDecoration( bool view );
    void ViewCoordinateSystem( bool view );

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );  
    
protected:
    virtual void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
    virtual int horizontalOffset () const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden ( const QModelIndex & index ) const;
    virtual QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
    virtual QRegion visualRegionForSelection ( const QItemSelection & selection ) const; 

private:
    SoSelection* m_sceneGraphRoot;
    SoQtExaminerViewer* m_myRenderArea; 
 };

#endif /*GRAPHICVIEW_H_*/
