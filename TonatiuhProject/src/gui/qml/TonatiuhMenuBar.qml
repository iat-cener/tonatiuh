
import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
//import insertshapemenu 1.0

MenuBar { 
 	
    Menu {
    	id: fileMenu
    	objectName: "fileMenu"
        title: qsTr("File")
        
        Action { 
        	text: qsTr("New")    
        	onTriggered: 
        	{
        		windowTonatiuh.newFileCb()
        	}
        } 
        Action { 
        	text: qsTr("Open...") 
        	onTriggered: {    
        		openFileDialog.title = "Open..."
            	openFileDialog.acceptLabel = "Open"
            	openFileDialog.fileMode = FileDialog.OpenFile
            	openFileDialog.open()    	
    		}
        } 
        Menu {
			title: "Recent"
    		
    		delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
    		
    		id: recentFilesSubMenu
          	//title: qsTr("Recent Files")
          	enabled: recentFilesInstantiator.count > 0

          	Instantiator {
              	id: recentFilesInstantiator
              	model: appSettings.recentFiles
              	delegate: MenuItem {
                  	text: appSettings.displayableFilePath(modelData)
                  	onTriggered: {
                  		console.log("Trigger")
                  		windowTonatiuh.openFileCb(modelData)
                  	}
              	}

              	onObjectAdded: {
              		console.log("object added")
              		recentFilesSubMenu.insertItem(index, object)
              	}
              	onObjectRemoved:  {
              		console.log("object removed")
              		recentFilesSubMenu.removeItem(object)
          		}              		
          	}

          	MenuSeparator {}

          	MenuItem {
              	text: qsTr("Clear Recent Files")
              	onTriggered: appSettings.clearRecentFiles()
          	}    			
	    }  
	    
        MenuSeparator { }            
        Action { 
        	text: qsTr("Save")
        	onTriggered: windowTonatiuh.saveFileCb()
        	 }
        Action { 
        	text: qsTr("Save as")
        	onTriggered: {
        		 openFileDialog.title = "Save as..."
            	openFileDialog.acceptLabel = "Save"
            	openFileDialog.fileMode = FileDialog.SaveFile
            	openFileDialog.open()
        	}
    	}
        Action { 
        	text: qsTr("Save Component")
        	onTriggered: windowTonatiuh.saveComponentFileCb()
        	icon.source:"" }
        MenuSeparator { }
        Action { 
        	text: qsTr("Print")
        	enabled: false
        	icon.source:"/../qml/icons/print.png"
        	onTriggered: windowTonatiuh.printFileCb()
    	 }
        MenuSeparator { }
        Action { 
        	text: qsTr("Close") 
        	icon.source:"/../qml/icons/close.png"
        	onTriggered: Qt.quit()
        }
        	
    	delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
	}

    Menu {
        title: qsTr("Edit")
        Action { 
        	text: qsTr("Undo")
        	enabled: false
    	}
        Action { 
        	text: qsTr("Redo")
        	enabled: false
    	 }
        MenuSeparator { }
        Action { text: qsTr("Paste") }
        Action { text: qsTr("Cut") }
        MenuItem { 
        	id: copyAction
        	text: qsTr("Copy") 
        	icon.name: "copy"
        	icon.source:"/../qml/icons/copy.png"
        	icon.color : "transparent"
        	onTriggered: window.activeFocusItem.copy()          	
		}
        Action { text: qsTr("Paste") }
        Action { text: qsTr("Paste Link") }
        Action { text: qsTr("Delete") }
        Menu {
    		title: "Trackers"
	        Action { text: "Set Aiming Point Relative" }
	        Action { text: "Set Aiming Point Absolute" }
	        
	        delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
	    }
	    
	    delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    Menu {
        title: qsTr("Insert")
        MenuItem { 
        	text: qsTr("Group Node")         	
	        icon.source: "/../qml/icons/separatorKit.png"
	        icon.color : "transparent"
	        onClicked: {
	            	windowTonatiuh.addSeparatorKitCb()	               	
	    	}           
	    }
        
        MenuItem { 
        	text: qsTr("Surface Node")        	
	        icon.source: "/../qml/icons/shapeKit.png"
	        icon.color : "transparent"
	        onClicked: {
	            	windowTonatiuh.addShapeKitCb()	               	
	    	}    
	    }
        MenuSeparator { }
        
        // Update Tracker Toolbar with available plugins
        //InsertShapeMenu {		
		//	onAddShapeToolBarItem : {
		//		console.log("insert shapeeee")
		//		var component = Qt.createComponent("GenericMenuButton.qml");
		//		var object = component.createObject(shapeMenu, {"x": 0 + xposition, "y": 0, "iconSource":icon, "typeOfButton":"Shape", name:pluginName});		
		//	}			
		//	Component.onCompleted: {					
		//		updateShapeToolBar()
		//	}
		//} 
        Menu{
        	id: shapeMenu
        	objectName: "shapeMenu"
        	
			
        	title: qsTr("Shape")
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
        }
        Menu{
        	title: qsTr("Material")
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
        }
        Menu{
        	title: qsTr("Tracker")
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
        }
        MenuSeparator { }
        Action { text: qsTr("User Component") }
        Menu{
        	title: qsTr("Component")
        	Action { text: qsTr("Heliostat Field Component") }
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
        }
        
        delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
        
    }
    Menu {
        title: qsTr("Environment")
        Menu {
        	title: qsTr("Sun Light")
        	Action { 
        		property variant sunlightwin
        		text: qsTr("Defin Sun Light...")
        		
        		onTriggered: {			
        			var component = Qt.createComponent("DefineSunlight.qml");
			    	sunlightwin = component.createObject(windowTonatiuh);        
			        sunlightwin.show();
			    }
        	}
        	Action { 
        		property variant sunpositioncalc
        		text: qsTr("Sun Position Calculator...")
        		onTriggered: {			
        			var component = Qt.createComponent("SunPositionCalculator.qml");
			    	sunpositioncalc = component.createObject(windowTonatiuh);        
			        sunpositioncalc.show();
			    }
        	
        	}
        	Action { text: qsTr("Disconnect all trackers...")}    
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}       	
        }
        Menu{
        	title: qsTr("Transmissivity")
        	Action { 
        		property variant transmissivitywin
        		text: qsTr("Define Transmissivity") 
        		onTriggered: {			
        			var component = Qt.createComponent("DefineTransmissivity.qml");
			    	transmissivitywin = component.createObject(windowTonatiuh);        
			        transmissivitywin.show();
			    }
        		
        	}
        	
        	delegate : MenuDelegate{ }
    	
        	background: Rectangle {
		        implicitWidth: 200
		        implicitHeight: 40
		        color: "#ffffff"
		        border.color: "#4682b4"
		        radius: 2            	
    		}
        }
        delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    Menu {
        title: qsTr("Ray Trace")
        Action { 
        	text: qsTr("Run") 
        	property variant runwin
        	onTriggered: {			
    			var component = Qt.createComponent("Run.qml");
		    	runwin = component.createObject(windowTonatiuh);        
		        runwin.show();
		    }
        
        }
        Action { 
        	text: qsTr("Run Flux Analysis...") 
        	property variant runFluxwin
        	onTriggered: {			
    			var component = Qt.createComponent("RunFlux.qml");
		    	runFluxwin = component.createObject(windowTonatiuh);        
		        runFluxwin.show();
		    }
        }
        Action { 
        	text: qsTr("Display Rays") 
        	enabled: false
    	}
        MenuSeparator { }
        Action { text: qsTr("Ray Trace Options...") }
        Action { text: qsTr("Reset Analyzer values") }
        
        delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    Menu {
        title: qsTr("View")
        Action { text: qsTr("Axis") }
        Action {  	
        	text: qsTr("Background")
        	enabled: true
        	//checkable: true
        	icon.name: "cut"
        	icon.source: "/../qml/icons/tickIcon.png"   
        	icon.color: "transparent"     	
        	onTriggered: {
	        	windowTonatiuh.showBackground()
        	}
        	
    	 }
        Action { text: qsTr("Edit Mode") }
        MenuSeparator { }
        Action { text: qsTr("X-Y Plane") }
        Action { text: qsTr("X-Z Plane") }
        Action { text: qsTr("Z-Y Plane") }
        Action { text: qsTr("Sun Plane") }
        MenuSeparator { }
        Action { text: qsTr("Grid") }
        Action { 
        	text: qsTr("Grid Settings...")
        	enabled: false
    	 }
    	 
    	delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    Menu {
        title: qsTr("Automation")
        Action { text: qsTr("Script Editor") }
        
        delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    Menu {
        title: qsTr("Help")
        Action { text: qsTr("About...") }
        
        delegate : MenuDelegate{ }
    	
    	background: Rectangle {
	        implicitWidth: 200
	        implicitHeight: 40
	        color: "#ffffff"
	        border.color: "#4682b4"
	        radius: 2            	
		}
    }
    
    delegate: MenuBarItem {
        id: menuBarItem

        contentItem: Text {
            text: menuBarItem.text
            font: menuBarItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuBarItem.highlighted ? "#ffffff" : "#4682b4"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 40
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted ? "#4682b4" : "transparent"
        }
	}
	
	
    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        color: "#ffffff"

        Rectangle {
            color: "#4682b4"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }
    
    
       
 }