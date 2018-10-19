import QtQuick 2.0
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Layouts 1.1

Rectangle {
	id: content
    objectName : "content"
    anchors.fill: parent
    anchors.margins: 6
     
    
    signal showBackground(bool show)
    onShowBackground: sceneRoot.showBackground(show)
    
    signal addElement(string eleFile)
	onAddElement: sceneRoot.addElement(eleFile) 
	
	signal changeSelectionNode(string nodeName, string pluginName)
	onChangeSelectionNode: { parametersLayout.changeSelectionNode(nodeName, pluginName)}
	
	
	RowLayout {
		id: contentLayout
		objectName: "contentLayout"
		anchors.fill: parent
		spacing: 6
		
		property int yParameter: ((parent.height / 2) + 3)
		property int yTreeview: 0
		property real parameterHeight: ((parent.height / 2) - 6)
		property real treeviewHeight: ((parent.height / 2) - 6)
		
		Rectangle {
		    id: scene
		    objectName : "scene"
		    color: 'white'
		    Layout.preferredWidth : parent.width / 2 + parent.width / 4 
        	Layout.fillHeight: true
        	border {
                width: 2
                color: "steelblue"
            }
            //signal addElement
            
            Rectangle {
            	id: resizeArea
                width: 2
                height: parent.height
                color: "steelblue"
                anchors.horizontalCenter: parent.right 
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    drag{ target: parent; axis: Drag.XAxis }
                    hoverEnabled: true
                    cursorShape: Qt.SizeHorCursor
                    onMouseXChanged: {
                        if(drag.active){							
                            scene.width = scene.width + mouseX
                            view2d.width = view2d.width - mouseX
                            view2d.x = view2d.x + mouseX
                   
                            if(scene.width < 50)
                            {
                                scene.width = 50
                                view2d.width = contentLayout.width - scene.width - 6
                            	view2d.x = 56
                        	}
                            else if(scene.width > contentLayout.width-56)
                            {
                                scene.width = contentLayout.width - 56
                                view2d.width = 50
                                view2d.x = contentLayout.width - 50
                        	}                   
                        }
                    }
                }
            }
        		
		    Scene3D {
		        id: scene3d
		        objectName : "scene3d"
		        anchors.fill: parent
		        anchors.margins: 10
		        //focus: true
		        aspects: ["input", "logic"]
		        enabled: true
		        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
		
				Entity {
					id : viewScene
					objectName : "viewScene"
					
					Camera {
				        id: camera
				        objectName : "camera"
				        projectionType: CameraLens.PerspectiveProjection
				        fieldOfView: 25
				        nearPlane : 0.1
				        farPlane : 1000.0
				        position: Qt.vector3d( 0.0, 0.0, 20.0 )
				        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
				        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
				    }
		
				    //FirstPersonCameraController { camera: camera }
					OrbitCameraController {
				        camera: camera
		    		}
		
				    components: [
				        RenderSettings {
				            activeFrameGraph: ForwardRenderer {
				            	id: framegraphid
				                camera: camera
				                clearColor: "transparent"
				            }
				        },
				        InputSettings { }
				       
				    ]
		            AnimatedEntity {
		            	id: sceneRoot
		            	objectName : "sceneRoot"
		            }  
		        }
				
		    }	
		}
				
		ColumnLayout {
			id: view2d
			Layout.fillWidth: true
        	Layout.fillHeight: true
        	implicitHeight : parent.height
			spacing: 6
			
			Rectangle {
			    id: treeview
			    objectName : "treeview"
			    Layout.fillWidth: true
        		Layout.fillHeight: true
			    color: "white"
			    border {
	                width: 2
	                color: "steelblue"
        		}	
        		
        		ColumnLayout {
        			id: treeviewLayout
        			Layout.fillHeight: true
        			width : parent.width - 6
        			height: treeview.height
        			x: parent.x + 3
        			y: parent.y + 3
        			
        			spacing: 2        			
        			
        			Rectangle {
						id: treeviewTitle
						height: 15
						Layout.fillWidth: true
        				Layout.alignment: Qt.AlignHCenter
        				color: "whitesmoke"   	
        				anchors.left: parent.left
						anchors.leftMargin : 5
						anchors.right: parent.right
						anchors.rightMargin : 5
						anchors.top: parent.top
						anchors.topMargin : 5
								
        					
						Text {
					        text: 'Node'
					        font { bold: true; pixelSize: 12 }
						}
					}
					
					TreeView { 
						id: treeviewList
						objectName: "treeviewList"
						height: treeview.height - 35
						property string selectedNodeType: "SeparatorKit"
						
					}
					
        		}
        		
    		}
					
			Rectangle {
			    id: parameters
			    objectName : "parameters"
			    Layout.fillWidth: true
        		Layout.fillHeight: true
			    color: "white"
			    border {
	                width: 2
	                color: "steelblue"
            	}
            	
            	
				ColumnLayout {
        			id: parametersLayout
        			Layout.fillHeight: true
        			width : parent.width - 6
        			height: parameters.height
					anchors.top:  parent.top
					anchors.bottom: parent.bottom
        			x: parent.x + 3
        			y: parent.y + 3
        		        			
        			spacing: 2
        			
        			signal changeSelectionNode(string nodeName, string pluginName)
        			
        			        			
        			
        			Rectangle {
						id: parametersTitle
						height: 15
						Layout.fillWidth: true
        				Layout.alignment: Qt.AlignHCenter
        				color: "whitesmoke"   	
        				anchors.left: parent.left
						anchors.leftMargin : 5
						anchors.right: parent.right
						anchors.rightMargin : 5
						anchors.top: parent.top
						anchors.topMargin : 5								
        					
						Text {
					        text: 'Parameters'
					        font { bold: true; pixelSize: 12 }
						}
					}
					Loader { 
						id: parametersViewLoader
						objectName: "parametersViewLoader"
						sourceComponent: ParametersView { 
							id: parametersView			
							objectName: "parametersView"			
						}
						//source: "/../qml/ParametersView.qml"
						height: parameters.height - 35
						width: parameters.width
						anchors.left: parent.left
						anchors.leftMargin : 5
						anchors.right: parent.right
						anchors.rightMargin : 5
						anchors.top: parametersTitle.bottom
						anchors.topMargin: 5
						anchors.bottom: parent.bottom
						anchors.bottomMargin : 5
						
					 }
					 
					 onChangeSelectionNode: {
					 	// Depending on the node selected, load one parameter view or other.
					 	//console.log("entra aqui selected node type", treeviewList.selectedNodeType)
					 	windowTonatiuh.updateParameterViewCb(nodeName, pluginName)
					 	/*switch(treeviewList.selectedNodeType) {
                            case "/../qml/icons/separatorKit.png": {
                                parametersView.setSource("/../qml/qml/ParametersView.qml")
                                break
                            }
                            case "/../qml/icons/shapeKit.png": {
                            	console.log("Depending on the children")
                            	//parametersView.setSource("/../qml/qml/ParametersViewShapeKit.qml")
                                break
                            }
                            default:
                            	console.log("load depending on plugin")
                            	break*/
                            /*case "Tracker": {
                            	parametersView.setSource("/../qml/qml/ParametersTracker.qml")
                                break
                            }
                            
                            case "Shape": {
                            	parametersView.setSource("/../qml/qml/ParametersViewShape.qml")
                                break
                            }
                            case "Material": {
                            	parametersView.setSource("/../qml/qml/ParametersViewMaterial.qml")
                                break
                            }*/
                        //}    
				 	}
					
					/*ParametersView { 
						id: parametersView
						height: parameters.height - 35
					}*/
				}	
				
				
				Rectangle {
	            	id: resizeArea2d
	                height: 2
	                width: parent.width
	                color: "steelblue"
	                anchors.horizontalCenter: parent.horizontalCenter
	                anchors.verticalCenter: parent.top
	
	                MouseArea {
	                    anchors.fill: parent
	                    drag{ target: parent; axis: Drag.YAxis }
	                    hoverEnabled: true
	                    cursorShape: Qt.SizeVerCursor
	                    onMouseXChanged: {
	                        if(drag.active){
	                            parameters.height = parameters.height - mouseY
	                            parameters.y = parameters.y + mouseY	          
	                            treeview.height = treeview.height + mouseY
   
	                            if(parameters.height < 20)
	                            {
	                                parameters.height = 20
	                                treeview.height = contentLayout.height - 20 - 6
	                            	parameters.y = contentLayout.height - 20
	                        	}
	                            else if(parameters.height > contentLayout.height-26)
	                            {
	                                parameters.height = contentLayout.height - 20 - 6
	                                treeview.height = 20
	                                parameters.y = 26
	                        	}      
        						contentLayout.yParameter = parameters.y
        						contentLayout.parameterHeight = parameters.height
        						contentLayout.yTreeview = treeview.y
        						contentLayout.treeviewHeight = treeview.height  
        						treeviewLayout.height = treeview.height 
        						treeviewList.height = treeview.height - 35   
	                        }
	                    }
	                }
	            }
	        
			}
		
		}
		
	}	
		
}