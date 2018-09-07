
import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

MenuBar { 
 	
    Menu {
        title: qsTr("File")
        Action { 
        	text: qsTr("New")           	
        	} 
        Action { 
        	text: qsTr("Open...") 
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
	    }
	    
	    
        MenuSeparator { }            
        Action { 
        	text: qsTr("Save")
        	 }
        Action { 
        	text: qsTr("Save as")
        	}
        Action { 
        	text: qsTr("Save Component")
        	icon.source:"" }
        MenuSeparator { }
        Action { 
        	text: qsTr("Print")
        	enabled: false
        	icon.source:"/../qml/icons/print.png"
    	 }
        MenuSeparator { }
        Action { 
        	text: qsTr("Close") 
        	icon.source:"/../qml/icons/close.png"
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
        Action { 
        	id: copyAction
        	text: qsTr("Copy") 
        	icon.name: "copy"
        	icon.source:"/../qml/icons/copy.png"
        	shortcut: StandardKey.Copy
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
        Action { text: qsTr("Group Node") }
        Action { text: qsTr("Surface Node") }
        MenuSeparator { }
        Menu{
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
        	Action { text: qsTr("Defin Sun Light...")}
        	Action { text: qsTr("Su Position Calculator...")}
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
        	Action { text: qsTr("Define Transmissivity") }
        	
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
        Action { text: qsTr("Run") }
        Action { text: qsTr("Run Flux Analysis...") }
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