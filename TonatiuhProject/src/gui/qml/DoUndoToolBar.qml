

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {  
 	id: doUndoToolbar  
 	objectName: "doUndoToolBar"
 	property int toolbarSize : width  
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
    RowLayout {
        id: doUndoToolBarLayout
        ToolButton {
        	Layout.preferredWidth: 44
        	implicitWidth: 44
            icon.source: "/../qml/icons/undo.png"
            icon.color : "transparent"
        }
        ToolButton {
        	Layout.preferredWidth: 44
        	implicitWidth: 44
            icon.source: "/../qml/icons/redo.png"
            icon.color : "transparent"
        }
        ToolButton {
        	id : dropDownButton
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
                width: menuwidth
				x: parent.width - width - 16

                MenuItem {
                	id: menuItemLayout
                    ToolButton {
                    	Layout.preferredWidth: 44
                    	implicitWidth: 44
			            icon.source: "/../qml/icons/undo.png"
			            icon.color : "transparent"
			        }
                }
                MenuItem {
                	ToolButton {
                		Layout.preferredWidth: 44
                		implicitWidth: 44
			            icon.source: "/../qml/icons/redo.png"
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
		console.log("nButtons", nButtons)
		for(var i = doUndoToolBarLayout.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				doUndoToolBarLayout.children[i-1].visible = true	   				 
			}
			else {
	        	doUndoToolBarLayout.children[i-1].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false    	
	    	}     
	  		
	  		if (nButtons >= doUndoToolBarLayout.children.length-1) {
	    		dropDownButton.visible = false
			}
	    	else {
	    		
	    		dropDownButton.visible = true
			}
		}	
		  
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		for(var i = 0; i < menuItemLayout.children.length-1 ; i++) {
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