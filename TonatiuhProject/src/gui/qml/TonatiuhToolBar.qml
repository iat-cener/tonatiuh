

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1

    
RowLayout {
	id: gridLayout
	width:1000
	//width: parent.width;
	height:40
	
	ListView {
		id: toolBarLayout
		layoutDirection: Qt.LeftToRight 
		orientation: Qt.Horizontal 
		width: parent.width; height:parent.height
			
		displaced: Transition {
			NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
	    }
	    
		model: DelegateModel {
			id: visualModel
	        model: ListModel {
	            id: colorModel	           
	            ListElement { tonatiuhToolbar: "FileToolBar.qml"; size: 288; color:"blue" } 
	            ListElement { tonatiuhToolbar: "DoUndoToolBar.qml"; size:73;  }
	            ListElement { tonatiuhToolbar: "EditToolBar.qml"; size:208; color:"red" }
	            ListElement { tonatiuhToolbar: "ViewToolBar.qml"; size:208; color:"red" }
	            ListElement { tonatiuhToolbar: "RunToolBar.qml"; size:73; color:"red" } 
	            ListElement { tonatiuhToolbar: "InsertElementToolBar.qml"; size:73; color:"red" }  
	            ListElement { tonatiuhToolbar: "InsertShapeToolBar.qml"; size:73} 
	            
            }	
	    	
	    	delegate: MouseArea {
	    		id: delegateRoot
	    		
	    		property int visualIndex: DelegateModel.itemsIndex
	    		
				hoverEnabled: true
                cursorShape: Qt.SizeAllCursor
	            width: model.size + 12
	            
	            height: 50
	            drag.target: icon   
 
	            Rectangle {
	                id: icon
	                height: 42
	                anchors {
	                	//left: parent.left
	                    horizontalCenter: parent.horizontalCenter;
	                    verticalCenter: parent.verticalCenter
	                }
	                	                	                
	                Loader {
	                	source: model.tonatiuhToolbar       		                	
	                }	
	                width: model.size	
					
	                Drag.active: delegateRoot.drag.active
	                Drag.source: delegateRoot
	                /*Drag.hotSpot.x: 36
	                Drag.hotSpot.y: 36*/
	
	                states: [
	                    State {
	                        when: icon.Drag.active
	                        ParentChange {
	                            target: icon
	                            parent: toolBarLayout
	                        }
	
	                        AnchorChanges {
	                            target: icon;
	                            anchors.horizontalCenter: undefined;
	                            anchors.verticalCenter: undefined
	                        }
	                    }
	                ]
	                
	                Rectangle {
		            	id: dradDropArea
		            	height: 40
		            	width: 2
		            	anchors {
		                    left: parent.left;
		                }
		                color: "steelBlue"
		            }
	            }
	            DropArea {
	                anchors { fill: parent; margins: 15 }
	
	                onEntered: visualModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
	            }
	        }
	    }
    }
}	