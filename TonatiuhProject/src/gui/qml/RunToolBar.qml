

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {  
 	id: runToolbar      
 	objectName : "RunToolBar"
 	property int toolbarSize : width
 	signal compressToolBar(int totalSize, int currentTotalSize, int remainingSize)
    RowLayout {
    	id: runToolBarLayout
        anchors.fill: parent
        ToolButton {
        	id: run
            icon.source: "/../qml/icons/run.png"
            icon.color : "transparent"
            background: Rectangle {
        		color: Qt.darker("#33333333", run.enabled && (run.checked || run.highlighted) ? 1.5 : 1.0)
    			opacity: enabled ? 1 : 0.3
    			visible: run.down || (run.enabled && (run.checked || run.highlighted))
			}
        }
        ToolButton {
            icon.source: "/../qml/icons/fluxdistribution.png"
            icon.color : "transparent"
            id: fluxDistribution
            background: Rectangle {
        		color: Qt.darker("#33333333", fluxDistribution.enabled && (fluxDistribution.checked || fluxDistribution.highlighted) ? 1.5 : 1.0)
    			opacity: enabled ? 1 : 0.3
    			visible: fluxDistribution.down || (fluxDistribution.enabled && (fluxDistribution.checked || fluxDistribution.highlighted))
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
                width: menuwidth
				x: parent.width - width - 16

				background: Rectangle{
	                    implicitWidth: parent.width
	                    //implicitHeight: parent.height
	                    color: "gainsboro"
	                    border.width: 1
	                    border.color: "black"
            	}
            	
                MenuItem {
                	id: menuRunItemLayout
                    ToolButton {
			        	id: runAux
			        	Layout.preferredWidth: 44
                		implicitWidth: 44
			            icon.source: "/../qml/icons/run.png"
			            icon.color : "transparent"			            
			        }
			        ToolButton {
			            icon.source: "/../qml/icons/fluxdistribution.png"
			            icon.color : "transparent"
			            id: fluxDistributionAux
			            Layout.preferredWidth: 44
                		implicitWidth: 44     
			        } 
                }
            }
        }            
    }	
    /*background: Rectangle {
		color: "white"
	}*/	
	
	onCompressToolBar: {	
		var availableSize = (totalSize - (currentTotalSize + remainingSize))
		dropDownMenu.menuwidth = availableSize
		var nButtons = Math.floor((availableSize - 16) /44)
		if (nButtons == 0)
			nButtons = 1
		console.log("nButtons", nButtons)
		for(var i = runToolBarLayout.children.length-1; i > 0 ; i--) {
			if (i <= nButtons) {
				runToolBarLayout.children[i-1].visible = true	   				 
			}
			else {
	        	runToolBarLayout.children[i-1].visible = false	
	        	dropDownMenu.close()	    
	        	dropDownButton.clicked = false    	
	    	}     
	  		
	  		if (nButtons >= runToolBarLayout.children.length-1) {
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
  			 menuRunItemLayout.children[i].x = currentWidth
  			 menuRunItemLayout.children[i].y = currentHeight
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




