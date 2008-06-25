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
 
#ifndef SCENEMODEL_H_
#define SCENEMODEL_H_

#include <QAbstractItemModel>
#include "PathWrapper.h"
#include "TLightKit.h"
#include "tgc.h"
#include <QIcon>
#include <map>


class SoSelection;
class SoBaseKit;
class SoNodeKitPath;
class SoSceneKit;
class InstanceNode;

class SceneModel : public QAbstractItemModel
{
	
public:
	SceneModel( QObject* parent = 0);
	~SceneModel();
	
	void SetCoinRoot( SoSelection& coinRoot );	
	void SetCoinScene( SoSceneKit& coinScene );

	QModelIndex index( int row, int column, const QModelIndex& parentModelIndex = QModelIndex()) const;
	int rowCount( const QModelIndex& parentModelIndex ) const; 	
	int columnCount ( const QModelIndex& ) const;
	QModelIndex parent ( const QModelIndex& childModelIndex ) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;		
	QVariant data ( const QModelIndex& modelIndex, int role = Qt::DisplayRole ) const;                    
	Qt::ItemFlags flags(const QModelIndex &index) const; 
    Qt::DropActions supportedDropActions() const;
	Qt::DropActions supportedDragActions() const;
	
    int InsertCoinNode( SoNode& coinChild, SoBaseKit& coinParent );
	void InsertLightNode( TLightKit& coinLight );
	void RemoveCoinNode( int row, SoBaseKit& coinParent );
	void RemoveLightNode( TLightKit& coinLight );

    void Cut( SoBaseKit& coinParent, int row );
	void Paste( tgc::PasteType type, SoBaseKit& coinParent, SoNode& coinChild, int row );
	
	InstanceNode* NodeFromIndex( const QModelIndex& modelIndex ) const;
	QModelIndex IndexFromPath( const SoNodeKitPath& coinNodePath ) const;
	
private:
	void GenerateInstanceTree( InstanceNode& instanceParent );
	void DeleteInstanceTree( InstanceNode& instanceNode );
	SoNodeKitPath* PathFromIndex( const QModelIndex& modelIndex ) const;
	
	
	
private:
	SoSelection* m_coinRoot;
    SoSceneKit* m_coinScene;	
	InstanceNode* m_instanceRoot;
	std::map< SoNode*, QList<InstanceNode*> > m_mapCoinQt;
};

#endif /*SCENEMODEL_H_*/
