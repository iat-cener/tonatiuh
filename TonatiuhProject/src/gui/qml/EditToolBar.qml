import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1
import QtQml.Models 2.1

ToolBar {  
 	id: editToolbar 
 	objectName: "editToolBar"
 	property int toolbarSize : width 
 	property int iconsCount : 0 
 	property int minToolbarSize: 52
 	signal compressToolBar(int totalSize, int currentTotalSize,  int remainingSize)
    RowLayout {
    	id: editToolbarLayout    	
        ToolButton {
            icon.source: "/../qml/icons/cut.png"
            icon.color : "transparent"                
            
            onClicked: {
            	windowTonatiuh.addElement("Torus.qml")               	
    		}
        }
        ToolButton {
            icon.source: "/../qml/icons/copy.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/paste.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/paste_link.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/delete.png"
            icon.color : "transparent"
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
                	id: menuItemLayout
                	ToolButton {
			            icon.source: "/../qml/icons/cut.png"
			            icon.color : "transparent"                
			            
			            onClicked: {
			            	windowTonatiuh.addElement("Torus.qml")               	
			    		}
			        }
                    ToolButton {
			            icon.source: "/../qml/icons/copy.png"
			            icon.color : "transparent"
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/paste.png"
			            icon.color : "transparent"
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/paste_link.png"
			            icon.color : "transparent"
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/delete.png"
			            icon.color : "transparent"
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
		for(var i = editToolbarLayout.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				editToolbarLayout.children[i-1].visible = true	       
			}
			else
	        	editToolbarLayout.children[i-1].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false    	        
  		}   
  		if (nButtons >= editToolbarLayout.children.length-1) {
    		dropDownButton.visible = false
		}
    	else {
    		dropDownButton.visible = true
		}
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		for(var i = 0; i < editToolbarLayout.children.length-1 ; i++) {
  			 menuItemLayout.children[i].x = currentWidth
  			 menuItemLayout.children[i].y = currentHeight
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
  		currentWidth = nButtons * 44 
    }	
    
}