import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import insertmaterialtoolbar 1.0


ToolBar {  
 	id: insertMaterialToolbar    
 	objectName : "InsertMaterialToolBar"
 	property int toolbarSize : 0
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
    RowLayout {
    	id: insertMaterialToolbarRow
    	objectName: "insertMaterialToolbarRow"
        anchors.fill: parent
        
        // Update Tracker Toolbar with available plugins
		InsertMaterialToolBar {			
			onAddMaterialToolBarItem : {
				var component = Qt.createComponent("GenericToolBarButton.qml");
				var object = component.createObject(insertMaterialToolbarRow, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});
				insertMaterialToolbar.toolbarSize = insertMaterialToolbar.toolbarSize + 44
			}		
			
			Component.onCompleted: {
				insertMaterialToolbar.toolbarSize = 0
				updateMaterialToolBar()
			}
		}     
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
                	id: menuItemLayoutMaterial
                	property int elementIndex
                	ScrollView {
                		ScrollBar.vertical.interactive: true             		
	                	InsertMaterialToolBar {			
							onAddMaterialToolBarItem : {
								var component = Qt.createComponent("GenericToolBarButton.qml");
								var object = component.createObject(menuItemLayoutMaterial, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":pluginName});
								object.visible = true
							}								
							Component.onCompleted: {
								menuItemLayoutMaterial.elementIndex = menuItemLayoutMaterial.children.length
								updateMaterialToolBar()
							}
						} 
					}
					
					                 	
		        }
	        }
        }         
    }
    onCompressToolBar: {
    	var availableSize = (totalSize - (currentTotalSize + remainingSize))
		dropDownMenu.menuwidth = availableSize - 16
		insertMaterialToolbar.toolbarSize = availableSize
		var nButtons = Math.floor((availableSize - 16) /49)	
		if (nButtons == 0)
			nButtons = 1
		var nbVisible = 0
		for(var i = insertMaterialToolbarRow.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				insertMaterialToolbarRow.children[i].visible = true	  				
				nbVisible = nbVisible + 1
			}
			else {
	        	insertMaterialToolbarRow.children[i].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false 	        	
        	}   	        
  		}   
  		insertMaterialToolbar.toolbarSize = nbVisible * 49
  		if (nButtons >= insertMaterialToolbarRow.children.length-1) {
    		dropDownButton.visible = false
    		insertMaterialToolbar.toolbarSize = insertMaterialToolbar.toolbarSize - 5
		}
    	else {
    		dropDownButton.visible = true
    		insertMaterialToolbarRow.children[1].anchors.left = insertMaterialToolbarRow.left
    		for ( var i = 2; i < nbVisible + 1 ; i++) {
    			insertMaterialToolbarRow.children[i].x = (i-1) * 49
    		}    		
  			dropDownButton.anchors.left = insertMaterialToolbarRow.children[nbVisible].right
  			insertMaterialToolbar.toolbarSize = insertMaterialToolbar.toolbarSize + 16
		}
		
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		
  		for(var i = (menuItemLayoutMaterial.elementIndex); i < menuItemLayoutMaterial.children.length ; i++) {
  			 menuItemLayoutMaterial.children[i].x = currentWidth
  			 menuItemLayoutMaterial.children[i].y = currentHeight
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