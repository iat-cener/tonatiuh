import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

ToolBar {  
 	id: insertToolbar   
 	objectName: "InsertElementToolbar"
 	property int toolbarSize : width   
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
    RowLayout {
    	id: insertToolbarRow
    	objectName: "insertToolbarRow"
        anchors.fill: parent     
        
        ToolButton {
        	id: separatorKit
        	visible: true
        	objectName: "separatorKit"
            icon.source: "/../qml/icons/separatorKit.png"
            icon.color : "transparent"
            onClicked: {
            	windowTonatiuh.addSeparatorKitCb()	               	
    		}
           
        }
        ToolButton {
        	id: shapeKit
        	visible: true
            icon.source: "/../qml/icons/shapeKit.png"
            icon.color : "transparent"
            onClicked: {
            	windowTonatiuh.addShapeKitCb()	               	
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
                //y: parent.height
                //transformOrigin: Menu.BottomLeft
				property int menuwidth
                width: menuwidth
                height : 0
				x: parent.width - width - 16
				background: Rectangle{
	                    implicitWidth: parent.width
	                    implicitHeight: parent.height
	                    color: "gainsboro"
	                    border.width: 1
	                    border.color: "black"
            	}
				MenuItem {
				 	id: menuItemElementLayout
	                ToolButton {
	                	id: separatorKitAux			      
			        	visible: true
			        	objectName: "separatorKit"
			            icon.source: "/../qml/icons/separatorKit.png"
			            icon.color : "transparent"
			            onClicked: {
			            	windowTonatiuh.addSeparatorKitCb()	               	
			    		}
			           
			        }
			        ToolButton {		
                    	id: shapeKitAux	    
			        	visible: true
			            icon.source: "/../qml/icons/shapeKit.png"
			            icon.color : "transparent"
			            onClicked: {
			            	windowTonatiuh.addShapeKitCb()	               	
			    		}
			        }       
		        }              
            }
        }  
    }	
    onCompressToolBar: {
    	var availableSize = (totalSize - (currentTotalSize + remainingSize))
		dropDownMenu.menuwidth = availableSize
		var nButtons = Math.floor((availableSize - 16) /44)
		if (nButtons == 0)
			nButtons = 1
		for(var i = insertToolbarRow.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				insertToolbarRow.children[i-1].visible = true	   				 
			}
			else {
	        	insertToolbarRow.children[i-1].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false    	
	    	}     
	  		
	  		if (nButtons >= insertToolbarRow.children.length-1) {
	    		dropDownButton.visible = false
			}
	    	else {
	    		
	    		dropDownButton.visible = true
			}
		}	
		  
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		for(var i = 0; i < 2 ; i++) {
  			 menuItemElementLayout.children[i].x = currentWidth
  			 menuItemElementLayout.children[i].y = currentHeight
  			 if (widthIndex < (nButtons - 1) ) {
  			 	currentWidth = currentWidth + 44
  			 	widthIndex = widthIndex + 1
  			 }
  			 else {
  			 	widthIndex = 0
  			 	currentWidth = 0
  			 	currentHeight = currentHeight + 44
		 	 }
  		}
  		if (widthIndex != 0)
  		{
  			currentHeight = currentHeight + 44
  		}
  		dropDownMenu.height = currentHeight
  		dropDownMenu.menuwidth = nButtons * 44 
  		//currentWidth = nButtons * 44  
    }   	
    
}