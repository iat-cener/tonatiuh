import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1
import tonatiuhtoolbarupdater 1.0

    
RowLayout {
	id: gridLayout	
	height:45
	property int totalSize: 0
	property int itemIndex: 0
	property bool fullComprex: false
	
	signal updateSizeToolBar(int width)
	
	onUpdateSizeToolBar: {
		
		tonatiuhToolbarUpdater.updateToolBar(width)
	}
	
	TonatiuhToolBarUpdater {
		id: tonatiuhToolbarUpdater
		
		signal updateToolBar(int width)
		
		onUpdateToolBar: {
			fileToolBarUpdater(width)
		}	
	}
	Rectangle {
		height:parent.height - 1
		width: parent.width
		anchors.left:parent.left
		anchors.right: parent.right
		//anchors.bottomMargin : 4
		//anchors.topMargin : 4
		anchors.rightMargin : 6
		anchors.leftMargin: 6
		color: "whitesmoke"
		
		ListView {
			id: toolBarLayout
			objectName: "toolBarLayout"
			layoutDirection: Qt.LeftToRight 
			orientation: Qt.Horizontal 
			anchors.top:parent.top
			anchors.left:parent.left
			anchors.right: parent.right
			width: parent.width
						
			signal insertTrackerButton()
		
			onInsertTrackerButton: {
				visualModel.insertTrackerButton()
			}
				
			signal updateSizeToolBar()
		
			onUpdateSizeToolBar: {
			
				visualModel.updateSizeToolBar()
			
			}
			displaced: Transition {
				NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
		    }
		         
			model: DelegateModel {
				id: visualModel
				signal updateSizeToolBar()
		
				onUpdateSizeToolBar: {
				
					loader.updateSizeToolBar()
				
				}		
		        model: ListModel {
		            id: toolbarModel
		            objectName: "toolbarModel"	
		              
		            ListElement { tonatiuhToolbar: "FileToolBar.qml";} 
		            ListElement { tonatiuhToolbar: "DoUndoToolBar.qml";}
		            ListElement { tonatiuhToolbar: "EditToolBar.qml";}
		            ListElement { tonatiuhToolbar: "ViewToolBar.qml";}
		            ListElement { tonatiuhToolbar: "RunToolBar.qml"; size:73;} 
		            ListElement { tonatiuhToolbar: "InsertElementToolBar.qml"; size:73;}  
		            ListElement { tonatiuhToolbar: "InsertTrackerToolBar.qml";}   
		            ListElement { tonatiuhToolbar: "InsertShapeToolBar.qml";} 	                
		            ListElement { tonatiuhToolbar: "InsertMaterialToolBar.qml";}  
		            
	            }	
		    	
		    	delegate: MouseArea {
		    		id: delegateRoot
		    		
		    		property int visualIndex: DelegateModel.itemsIndex
		    		
					hoverEnabled: true
	                cursorShape: Qt.SizeAllCursor
	  
		            height: 50
		            drag.target: icon   
	 
	 				signal updateSizeToolBar()
		
					onUpdateSizeToolBar: {
					
						icon.updateSizeToolBar()
					
					}
		            Rectangle {	            	
		                id: icon
		                height: 42
		                width: loader.item.toolbarSize	
		                anchors {
		                    horizontalCenter: parent.horizontalCenter;
		                    verticalCenter: parent.verticalCenter	                    
		                }	
		                
		                signal updateSizeToolBar()
		
						onUpdateSizeToolBar: {
						
							loader.updateSizeToolBar()
						
						}                	                	                
		                Loader {
		                	id: loader
		                	source: model.tonatiuhToolbar  
		                	
		                	onLoaded: {
		                		gridLayout.itemIndex = gridLayout.itemIndex + 1    
		                		    	                		
		                		// Check if the current toolbar fits in the toolbar row
		                		var remainingSize = 0
		                		remainingSize = (9 - gridLayout.itemIndex) * 65	  			
								loader.item.compressToolBar(windowTonatiuh.width - 12, gridLayout.totalSize, remainingSize)
			                	gridLayout.totalSize = gridLayout.totalSize + loader.item.toolbarSize	
			                	tonatiuhToolbarUpdater.saveToolbarRef(loader.item)
	                		} 	
		                }
		                
		                Drag.active: delegateRoot.drag.active
		                Drag.source: delegateRoot
		
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
			            	z: 1
			            	anchors {
			                    left: parent.left;
			                }
			                color: "steelBlue"
			                
			            }
		            }
		            width: loader.item.toolbarSize
		           
		            DropArea {
		            	id: dropAreaToolBar
		                anchors { fill: parent; margins: 15 }
		
		                onEntered: {
		                	visualModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
		                }
		            }
		        }
		    }
	    }
    }
}	
