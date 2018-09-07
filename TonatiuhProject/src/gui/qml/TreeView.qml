import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4


Rectangle {  
    objectName : "treeview"
    id: treeviewList
    width: parent.width
	anchors.left: parent.left
	anchors.leftMargin : 5
	anchors.right: parent.right
	anchors.rightMargin : 5
	anchors.top: treeviewTitle.bottom
	anchors.topMargin : 5
	anchors.bottom: parent.bottom
	anchors.bottomMargin : 5
	
	signal addSeparatorKit
	signal addShapeKit
	
	onAddSeparatorKit: objModel.append({"name": qsTr("TSeparatorKit1"), "icon":"/../qml/icons/separatorKit.png", "level": 1, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
	onAddShapeKit: objModel.append({"name": qsTr("TShapeKit1"), "icon":"/../qml/icons/shapeKit.png", "level": 1, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
	
	Keys.onPressed: {
		if (event.key == Qt.Key_Top) {
            event.accepted = true;
            if (listview.currentIndex > 0) {
            	listview.currentIndex = listview.currentIndex - 1
        	}
        }
        else if (event.key == Qt.Key_Bottom) {
            event.accepted = true;
            if (listview.currentIndex < listview.count) {
            	listview.currentIndex = listview.currentIndex + 1
        	}
		}
	}	
	
	property int rowHeight
	    	
    ListModel {
        id: objModel   
    }     

    Component {
        id: objRecursiveDelegate
        Column {
            id: objRecursiveColumn
            property int m_iIndex: model.index
            property var m_parentModel: model.parentModel
            clip: true
            MouseArea {
                id: objMouseArea
         	    objectName: "objMouseArea"         	
                width: objRow.implicitWidth
                height: objRow.implicitHeight 
                onDoubleClicked: {
                    for(var i = 1; i < parent.children.length - 1; ++i) {               		
                        if(parent.children[i].objectName !== "objMouseArea") {
                        console.log("object name", parent.children[i].objectName)
                        console.log("elemento i", i)
                  	    parent.children[i].visible = !parent.children[i].visible
                        }
                    }
                }
            
	            onClicked: {
	            	listview.focus = true
	            	listview.currentIndex = index 
	            	console.log("parent", model.parentIndex)
	                console.log("index", index)
	            	console.log("item", listview.currentItem)
	            	console.log("item", item)
	            }
                        
	            drag.target: objDragRect
	            onReleased: {
	               if(objDragRect.Drag.target) {
	                  objDragRect.Drag.drop()
	               }
	            }
            
	            onWheel: {
	            	objRow.x = 0
	        		console.log("x position", objRow.x)
	        		console.log("altura", treeviewList.height)
	        	}
            
	            Row {
	                id: objRow               
	                Item {
	                    height: 1.4
	                    width: model.level * 20
	                }
               
               	    Rectangle {
                        id: objDisplayRowRect                  
                        height: objText.implicitHeight + 5
                        width: objCollapsedStateIndicator.width + objText.implicitWidth + objImage.implicitWidth + 5
                        Component.onCompleted: {
        		  	        treeviewList.rowHeight= objText.implicitHeight
			  	        }
			  	 
	                    DropArea {
	                        keys: [model.parentModel]
	                        anchors.fill: parent
	                        onEntered: objValidDropIndicator.visible = true
	                        onExited: objValidDropIndicator.visible = false
	                        onDropped: {
	                            objValidDropIndicator.visible = false
	                            if(drag.source.m_objTopParent.m_iIndex !== model.index) {
	                                objRecursiveColumn.m_parentModel.move(
	                                    drag.source.m_objTopParent.m_iIndex,
	                                    model.index,
	                                    1)
	                            }
	                        }
	                     
	                        Rectangle {
	                            id: objValidDropIndicator
	                            anchors.fill: parent
	                            visible: false
	                            onVisibleChanged: {
	                                visible ? objAnim.start() : objAnim.stop()
	                            }
	                            SequentialAnimation on color {
	                                id: objAnim
	                                loops: Animation.Infinite
	                                running: false
	                                ColorAnimation { from: "#31312c"; to: "steelblue"; duration: 400 }
	                                ColorAnimation { from: "steelblue"; to: "#31312c"; duration: 400 }
	                            }
	                        }
	                    }
	                  
	                  
	                    Rectangle {
	                        id: objDragRect
	                        property var m_objTopParent: objRecursiveColumn
	                        Drag.active: objMouseArea.drag.active
	                        Drag.keys: [model.parentModel]                    
	                        opacity: .85
	                        states: State {
	                            when: objMouseArea.drag.active
	                            AnchorChanges {
	                                target: objDragRect
	                                anchors { horizontalCenter: undefined; verticalCenter: undefined }
	                            }
	                            ParentChange {
	                                target: objDragRect
	                                parent: treeviewList
	                            }
	                        }
	                        anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
	                        height: objDisplayRowRect.height
	                        width: objDisplayRowRect.width
	                        visible: Drag.active
	                        color: "steelblue"
	                        Text {
	                            anchors.fill: parent
	                            horizontalAlignment: Text.AlignHCenter
	                            verticalAlignment: Text.AlignVCenter
	                            text: model.name	                       
	                        }
	                    } 
		                Text {
		               	    id: objCollapsedStateIndicator
		               	    anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
		                    text: (objRecursiveColumn.children.length > 1 ?
		                           objRecursiveColumn.children[1].visible ?
		                           qsTr("-  ") : qsTr("+ ") : qsTr("   "))
		                }
		                Image {
		               		id: objImage
		               		anchors { left: objCollapsedStateIndicator.right; }
		               		height: 12; width: 12              		
		               		source: model.icon
		           		}
		           		Rectangle {
		               	    id: objSpace
		               	    width: 5
		               	    anchors { left: objImage.right; top: parent.top; bottom: parent.bottom }		                    
		                }
		                TextEdit {
		               	    id: objText
		               	    anchors { left: objSpace.right; top: parent.top; bottom: parent.bottom }
		                    text: model.name
		                    color: objRecursiveColumn.children.length > 2 ? "blue" : "black"
		                    font { pixelSize: 12 }
		                    overwriteMode : true
		                    focus: true
		                    onEditingFinished: {
		                    	cursorVisible : false
		                    	listview.focus = true
		                    	
	                    	}
		                    //lineHeight: 1.4    
		                    Keys.onEnterPressed: {		                 			
                 				console.log("ya he terminado de escribir")
								editingFinished()									
							}
		                    MouseArea {
	                            id: textMouseArea
	                            anchors.fill: parent
	                            hoverEnabled: true
	                            onDoubleClicked: {
	                         		objText.forceActiveFocus()	                         		                    		
                         		}                         		
                         		onClicked: {
                         			listview.focus = true
	            					listview.currentIndex = index 
                         		}
                         		
                         		                        		   		
	                        }		                              
		                } 	                                   
					}              
            	}
         	}
                
            Repeater {
         	    id: objRepeater
                model: subNode
                delegate: objRecursiveDelegate
            }
      	}
   	}
   
   	Component.onCompleted: {	     
		objModel.append({"name": qsTr("RootNode"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		/*objModel.append({"name": qsTr("Oneaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Twoaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
  		objModel.append({"name":qsTr("SomethingElse3aaaaaaaaaaaaaaaaaa") , "icon":"/../qml/icons/separatorKit.png", "parentModel": objModel, "level": 1, "subNode": [], "parentIndex": 1})
  		objModel.append({"name": qsTr("Zeroaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Oneaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Twoaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
  		objModel.append({"name": qsTr("Zeroaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Oneaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Twoaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
  		objModel.append({"name":qsTr("SomethingElse3") , "icon":"/../qml/icons/separatorKit.png", "level": 1, "parentModel": objModel, "subNode": [], "parentIndex": 1})
  		objModel.append({"name": qsTr("Zeroaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Oneaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0})
  		objModel.append({"name": qsTr("Twoaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0}) 
  		objModel.append({"name":qsTr("SomethingElse3") , "icon":"/../qml/icons/separatorKit.png", "level": 1, "parentModel": objModel, "subNode": [], "parentIndex": 2})
  		objModel.append({"name": qsTr("Twoaaaa"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0}) */
	}
	   
   
	ColumnLayout {
		anchors.fill: parent
		width: parent.width
		height: treeviewList.height 		
		Component {
	        id: highlightBar
	        Rectangle {
	            width: view2d.width; height: treeviewList.rowHeight
	            color: "steelblue"
	            y: listview.currentItem.y;
	            z:3
	            opacity: 0.2
	        }
	    }
	
		
		ListView {
			id: listview
			width: parent.width
		    height: parent.height
		    contentWidth:360
		    flickableDirection: Flickable.AutoFlickIfNeeded
		    ScrollBar.vertical: ScrollBar {}
		    ScrollBar.horizontal: ScrollBar {}	     	     
		    clip: true
		    model: objModel
		    delegate: objRecursiveDelegate
    
		    highlight: highlightBar
		    highlightFollowsCurrentItem: false		       
		}
		

	} 		
}

