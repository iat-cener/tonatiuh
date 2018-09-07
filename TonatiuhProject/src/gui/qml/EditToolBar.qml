

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1
import QtQml.Models 2.1
import windowmain 1.0

ToolBar {  
     	id: editToolbar      
     	
        RowLayout {
        	id: editToolbarLayout
        	
            ToolButton {
                icon.source: "/../qml/icons/cut.png"
                icon.color : "transparent"                
                
                onClicked: {
                	windowTonatiuh.addElement("Torus.qml")
	               	
        		}
            }
            ToolButton {
                icon.source: "/../qml/icons/copy.png"
                icon.color : "transparent"
            }
            ToolButton {
                icon.source: "/../qml/icons/paste.png"
                icon.color : "transparent"
            }
            ToolButton {
                icon.source: "/../qml/icons/paste_link.png"
                icon.color : "transparent"
            }
            ToolButton {
                icon.source: "/../qml/icons/delete.png"
                icon.color : "transparent"
            }
    	}
    }