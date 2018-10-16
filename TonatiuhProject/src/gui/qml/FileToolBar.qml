import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1
import tonatiuhtoolbarupdater 1.0

ToolBar {
	id: fileToolbar	
	objectName: "fileToolBar"	
	property int toolbarSize : width 	
	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)		
			    
    RowLayout {
        id: fileToolbarLayout
        objectName: "fileToolbarLayout"
        ToolButton {
            icon.source: "/../qml/icons/new.png"
            icon.color : "transparent"
            ToolTip.visible: hovered
			ToolTip.text: qsTr("New")  
			onClicked: {				
				windowTonatiuh.newFileCb()
			}
        }
        ToolButton {
            icon.source: "/../qml/icons/open.png"
            icon.color : "transparent"
            onClicked: 
            {
            	openFileDialog.title = "Open..."
            	openFileDialog.acceptLabel = "Open"
            	openFileDialog.fileMode = FileDialog.OpenFile
            	openFileDialog.open()
            }

        }
        ToolButton {
            icon.source: "/../qml/icons/save.png"
            icon.color : "transparent"
            onClicked: windowTonatiuh.saveFileCb()
        }
        ToolButton {
            icon.source: "/../qml/icons/saveas.png"
            icon.color : "transparent"
            onClicked: {
            	openFileDialog.title = "Save as..."
            	openFileDialog.acceptLabel = "Save"
            	openFileDialog.fileMode = FileDialog.SaveFile
            	openFileDialog.open()
            }
        }
        //ToolSeparator {}
        ToolButton {
            icon.source: "/../qml/icons/print.png"
            icon.color : "transparent"
            onClicked: windowTonatiuh.printFileCb()
        }
        //ToolSeparator {}
        ToolButton {
            icon.source: "/../qml/icons/close.png"
            icon.color : "transparent"
            onClicked: Qt.quit()
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
			            icon.source: "/../qml/icons/new.png"
			            icon.color : "transparent"
			            ToolTip.visible: hovered
						ToolTip.text: qsTr("New")
						onClicked: windowTonatiuh.newFileCb()
			            
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/open.png"
			            icon.color : "transparent"
			            onClicked: windowTonatiuh.openFileCb()
			
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/save.png"
			            icon.color : "transparent"
			            onClicked: windowTonatiuh.saveFileCb()
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/saveas.png"
			            icon.color : "transparent"
			            onClicked: windowTonatiuh.saveFileAsCb()
			        }
			        //ToolSeparator {}
			        ToolButton {
			            icon.source: "/../qml/icons/print.png"
			            icon.color : "transparent"
			            onClicked: windowTonatiuh.printFileCb()
			        }
			        //ToolSeparator {}
			        ToolButton {
			            icon.source: "/../qml/icons/close.png"
			            icon.color : "transparent"
			            onClicked: windowTonatiuh.closeFileCb()
			        }
                }
            }
        }  
    }
    
    onCompressToolBar: {   
    	var availableSize = (totalSize - (currentTotalSize + remainingSize))
		dropDownMenu.menuwidth = availableSize - dropDownButton.width

		var nButtons = Math.floor((dropDownMenu.menuwidth - 16) /44)
		if (nButtons == 0)
			nButtons = 1
		for(var i = fileToolbarLayout.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				fileToolbarLayout.children[i-1].visible = true	   				 
			}
			else {
	        	fileToolbarLayout.children[i-1].visible = false		        	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false    	
        	}     
  		} 
  		if (nButtons >= fileToolbarLayout.children.length-1) {
    		dropDownButton.visible = false
		}
    	else {
    		dropDownButton.visible = true
		}
  	  			
  		var currentWidth = 0
  		var currentHeight = 0 
  		var widthIndex = 0  	
  		for(var i = 0; i < fileToolbarLayout.children.length-1; i++) {
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