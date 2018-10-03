import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import inserttrackertoolbar 1.0


ToolBar {  
 	id: insertTrackerToolbar      
 	objectName: "InsertTrackerToolBar"
 	property int toolbarSize : 0
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
 	
 	// Update Tracker Toolbar with available plugins
	InsertTrackerToolBar {
		id: insertTrackerObject
		onAddTrackerToolBarItem : {
			var component = Qt.createComponent("GenericToolBarButton.qml");	
			var object = component.createObject(insertTrackerToolbarRow, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});
			insertTrackerToolbar.toolbarSize = insertTrackerToolbar.toolbarSize + 44
		}			
		Component.onCompleted: {
			insertTrackerToolbar.toolbarSize = 0
			updateTrackerToolBar()
		}			
	} 
 	
    RowLayout {
    	id: insertTrackerToolbarRow
    	objectName: "insertTrackerToolbarRow"
        anchors.fill: parent  		
		
		ToolButton {
        	id: dropDownButton
        	property bool clicked: false
        	implicitWidth: 16   
        	visible : false    
        	text: qsTr("⋮")
            onClicked: {
            	if (clicked == false) {
            		dropDownMenu.open()
            		clicked = true
        		}
        		else {
        			dropDownMenu.close()
        			clicked = false
    			}
			}
            Menu {
                id: dropDownMenu
                property int menuwidth
                height: 0
                width: menuwidth
                x: parent.width - width - 16    
                            
				background: Rectangle{
	                    implicitWidth: parent.width
	                    implicitHeight: parent.height
	                    color: "gainsboro"
	                    border.width: 1
	                    border.color: "black"
            	}
                MenuItem {
                	id: menuItemLayoutTracker
                	property int elementIndex
                	InsertTrackerToolBar {
						onAddTrackerToolBarItem : {
							var component = Qt.createComponent("GenericToolBarButton.qml");	
							var object = component.createObject(menuItemLayoutTracker, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});
							object.visible = true							
						}			
						Component.onCompleted: {
							menuItemLayoutTracker.elementIndex = menuItemLayoutTracker.children.length
							updateTrackerToolBar()
						}			
					}	                	
		        }
	        }
        }              
    }
    
    onCompressToolBar: {
  		var availableSize = (totalSize - (currentTotalSize + remainingSize))
		dropDownMenu.menuwidth = availableSize - 16
		insertTrackerToolbar.toolbarSize = availableSize
		var nButtons = Math.floor((availableSize - 16) /49)	
		if (nButtons == 0)
			nButtons = 1
		var nbVisible = 0
		for(var i = insertTrackerToolbarRow.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				insertTrackerToolbarRow.children[i].visible = true	  				
				nbVisible = nbVisible + 1
			}
			else {
	        	insertTrackerToolbarRow.children[i].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false 	        	
        	}   	        
  		}   
  		insertTrackerToolbar.toolbarSize = nbVisible * 49
  		if (nButtons >= insertTrackerToolbarRow.children.length-1) {
    		dropDownButton.visible = false
    		insertTrackerToolbar.toolbarSize = insertTrackerToolbar.toolbarSize - 5
		}
    	else {
    		dropDownButton.visible = true
    		insertTrackerToolbarRow.children[1].anchors.left = insertTrackerToolbarRow.left
    		for ( var i = 2; i < nbVisible + 1 ; i++) {
    			insertTrackerToolbarRow.children[i].x = (i-1) * 49
    		}
  			dropDownButton.anchors.right = insertTrackerToolbarRow.right
  			insertTrackerToolbar.toolbarSize = insertTrackerToolbar.toolbarSize + 16 
		}
		
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		
  		for(var i = (menuItemLayoutTracker.elementIndex); i < menuItemLayoutTracker.children.length ; i++) {
  			 menuItemLayoutTracker.children[i].x = currentWidth
  			 menuItemLayoutTracker.children[i].y = currentHeight
  			 if (widthIndex < (nButtons - 1) ) {
  			 	currentWidth = currentWidth + 49
  			 	widthIndex = widthIndex + 1
  			 }
  			 else {
  			 	widthIndex = 0
  			 	currentWidth = 0
  			 	currentHeight = currentHeight + 49
		 	 }
  		}  		
  		if (widthIndex != 0)
  		{
  			currentHeight = currentHeight + 49
  		}
		
  		dropDownMenu.height = currentHeight 
  		dropDownMenu.width = nButtons * 49    		
    }   	
    
}