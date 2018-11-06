import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4


Rectangle {  
    id: treeviewList
    property string selectedNodeURL
    property int selectedNodeIdx
    width: parent.width
	anchors.left: parent.left
	anchors.leftMargin : 5
	anchors.right: parent.right
	anchors.rightMargin : 5
	anchors.top:  treeviewTitle.bottom
	anchors.topMargin : 5
	anchors.bottom: parent.bottom
	anchors.bottomMargin : 5
	
	signal addSeparatorKit(string name, string nodeUrl)
	signal addShapeKit(string name, string nodeUrl)
	signal addNode(string name, string icon, string nodeUrl, string pluginName)
	signal deleteNodeCb(string nodeUrl)
	signal copyNodeCb(string nodeUrl)
	signal pasteNodeCb(string nodeUrl)
	signal pasteRefNodeCb(string nodeUrl)
	
	signal deleteNode(string nodeUrl)
	signal getSelectedNode
	
	
	onAddSeparatorKit: {
		objModel.insert(listview.currentIndex + 1, {"name": qsTr(name), "icon":"/../qml/icons/separatorKit.png", "level": (objModel.get(listview.currentIndex).level + 1), "parentModel": objModel, "subNode": [], "parentIndex": listview.currentIndex, url:nodeUrl, pluginName:"separatorKit"}) 
	}
	onAddShapeKit: {
		objModel.insert(listview.currentIndex + 1, {"name": qsTr(name), "icon":"/../qml/icons/shapeKit.png", "level": (objModel.get(listview.currentIndex).level + 1), "parentModel": objModel, "subNode": [], "parentIndex": listview.currentIndex, url:nodeUrl, pluginName:"shapeKit"}) 
	}
	onAddNode: {
		objModel.insert(listview.currentIndex + 1, {"name": qsTr(name), "icon":icon, "level": (objModel.get(listview.currentIndex).level + 1), "parentModel": objModel, "subNode": [], "parentIndex": listview.currentIndex, url:nodeUrl, pluginName:pluginName}) 
	}
	
	onDeleteNode: {
		objModel.remove(listview.currentIndex)
	}
	
	
	onGetSelectedNode: {
		selectedNodeURL = objModel.get(listview.currentIndex).url
		selectedNodeIdx = objModel.get(listview.currentIndex).parentIndex
	}
	
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
        objectName : "objModel" 
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
                  	    parent.children[i].visible = !parent.children[i].visible
                        }
                    }
                }
            
	            onClicked: {
	            	listview.focus = true
	            	listview.currentIndex = index 
	            	
	            }
                        
	            drag.target: objDragRect
	            onReleased: {
	               if(objDragRect.Drag.target) {
	                  objDragRect.Drag.drop()
	               }
	            }
            
	            onWheel: {
	            	objRow.x = 0
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
	                            acceptedButtons: Qt.LeftButton | Qt.RightButton
	                            onDoubleClicked: {
	                         		objText.forceActiveFocus()	                         		                    		
                         		}                         		
                         		onClicked: {
                         			listview.focus = true
	            					listview.currentIndex = index 
	            					if (mouse.button == Qt.LeftButton)
							        {
							        	console.log("listview.currentIndex", listview.currentIndex)
							            console.log("SelectedNode", objModel.get(listview.currentIndex).pluginName)
							            if (objModel.get(listview.currentIndex).pluginName == "shapeKit")
							           	{
							           		//Check if has one child
							           		if  (objModel.get(listview.currentIndex + 1).parentIndex == listview.currentIndex)
							           		{							           			
							           			console.log("tiene 1 hijo")
							           			// load properties of that children and check if has other
							           			if  (objModel.get(listview.currentIndex + 2).parentIndex == listview.currentIndex)
							           			{
							           				//load properties of that children
							           				console.log("tiene 2 hijos")
							           				windowTonatiuh.loadChildParametersViewCb(objModel.get(listview.currentIndex).nodeUrl, 
							           					objModel.get(listview.currentIndex + 1).nodeUrl, objModel.get(listview.currentIndex + 1).name, objModel.get(listview.currentIndex + 1).pluginName,
							           					objModel.get(listview.currentIndex + 2).nodeUrl, objModel.get(listview.currentIndex + 2).name, objModel.get(listview.currentIndex + 2).pluginName)
							           			}
							           			else
							           			{
							           				windowTonatiuh.loadChildParametersViewCb(objModel.get(listview.currentIndex).nodeUrl, objModel.get(listview.currentIndex + 1).nodeUrl, 
							           					objModel.get(listview.currentIndex + 1).name, objModel.get(listview.currentIndex + 1).pluginName, null, null, null)
							           			}
							           		}
							           	}
							           	else
							           	{
								            treeviewList.selectedNodeType = objModel.get(listview.currentIndex).icon
								            content.changeSelectionNode(objModel.get(listview.currentIndex).name, objModel.get(listview.currentIndex).pluginName)
							            }
							        }
							        else if (mouse.button == Qt.RightButton)
							        {
							            console.log("Right")
							            contextMenu.popup()
							        }	            					
                         		}
                         		Menu {
							        id: contextMenu
							        MenuItem { 
							        	text: "Cut"
							        	onTriggered: {			
						        			console.log("Call to cut action")
									    } 
								    }
							        MenuItem { 
							        	text: "Copy"
							        	onTriggered: {	
							        		treeviewList.copyNodeCb(objModel.get(listview.currentIndex).url)		
						        			console.log("Call to copy action")
									    } 
						        	 }
							        MenuItem { 
							        	text: "Paste" 
							        	onTriggered: {	
							        		treeviewList.pasteNodeCb(objModel.get(listview.currentIndex).url)			
						        			console.log("Call to paste action")
									    } 
									}
							        MenuItem { 
							        	text: "PasteLink" 
							        	onTriggered: {	
							        		treeviewList.pasteRefNodeCb(objModel.get(listview.currentIndex).url)			
						        			console.log("Call to paste link action")
									    } 
								    }
							        MenuItem { text: "Delete"
							        	onTriggered: {		
							        		treeviewList.deleteNodeCb(objModel.get(listview.currentIndex).url)
						        			console.log("Call to delete action")
									    } 
									}
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
		objModel.append({"name": qsTr("RootNode"), "icon":"/../qml/icons/separatorKit.png", "level": 0, "parentModel": objModel, "subNode": [], "parentIndex": 0, url:"/RootNode/"})
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

