import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import insertshapetoolbar 1.0

ToolBar {  
 	id: insertShapeToolbar  
 	objectName: "InsertShapeToolBar"
 	property int toolbarSize : 0
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
 	
 	// Update Tracker Toolbar with available plugins
	InsertShapeToolBar {		
		onAddShapeToolBarItem : {
			var component = Qt.createComponent("GenericToolBarButton.qml");
			var object = component.createObject(insertShapeToolbarRow, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});		
			insertShapeToolbar.toolbarSize = insertShapeToolbar.toolbarSize + 44
			
		}			
		Component.onCompleted: {
			insertShapeToolbar.toolbarSize = 0
			updateShapeToolBar()
		}
	}  
	
    RowLayout {
    	id: insertShapeToolbarRow    	
    	objectName: "insertToolbarRow"
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
                	id: menuItemLayoutShape
                	property int elementIndex   
                	
					             		
                	InsertShapeToolBar {		
						onAddShapeToolBarItem : {
							var component = Qt.createComponent("GenericToolBarButton.qml");
							var object = component.createObject(menuItemLayoutShape, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});
							insertShapeToolbar.toolbarSize = insertShapeToolbar.toolbarSize + object.width
						}			
						Component.onCompleted: {
							menuItemLayoutShape.elementIndex = menuItemLayoutShape.children.length
							updateShapeToolBar()
						}
					} 
						 
		        }
	        }
        }          
    }	
    
    onCompressToolBar: {
    	var availableSize = (totalSize - (currentTotalSize + remainingSize))	
		dropDownMenu.menuwidth = availableSize - 16
		insertShapeToolbar.toolbarSize = availableSize
		var nButtons = Math.floor((availableSize - 16) /49)	
		if (nButtons == 0)
			nButtons = 1
		var nbVisible = 0
		for(var i = insertShapeToolbarRow.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				insertShapeToolbarRow.children[i].visible = true	  				
				nbVisible = nbVisible + 1
			}
			else {
	        	insertShapeToolbarRow.children[i].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false 	        	
        	}   	        
  		}   
  		insertShapeToolbar.toolbarSize = nbVisible * 49
  		if (nButtons >= insertShapeToolbarRow.children.length-1) {
    		dropDownButton.visible = false
    		insertShapeToolbar.toolbarSize = insertShapeToolbar.toolbarSize - 5
		}
    	else {
    		dropDownButton.visible = true
    		insertShapeToolbarRow.children[1].anchors.left = insertShapeToolbarRow.left
    		for ( var i = 2; i < nbVisible + 1 ; i++) {
    			insertShapeToolbarRow.children[i].x = (i-1) * 49
    		}    		
  			dropDownButton.anchors.left = insertShapeToolbarRow.children[nbVisible].right
  			insertShapeToolbar.toolbarSize = insertShapeToolbar.toolbarSize + 16
		}
		
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		
  		for(var i = (menuItemLayoutShape.elementIndex); i < menuItemLayoutShape.children.length ; i++) {
  			 menuItemLayoutShape.children[i].x = currentWidth
  			 menuItemLayoutShape.children[i].y = currentHeight
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