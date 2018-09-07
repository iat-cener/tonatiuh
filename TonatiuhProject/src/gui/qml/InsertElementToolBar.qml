

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11




ToolBar {  
 	id: insertToolbar      
    RowLayout {
    	id: insertToolbarRow
    	objectName: "insertToolbarRow"
        anchors.fill: parent
     
        
        ToolButton {
        	id: separatorKit
        	objectName: "separatorKit"
            icon.source: "/../qml/icons/separatorKit.png"
            icon.color : "transparent"
            onClicked: {
            	windowTonatiuh.addSeparatorKit()	               	
    		}
           
        }
        ToolButton {
            icon.source: "/../qml/icons/shapeKit.png"
            icon.color : "transparent"
            onClicked: {
            	windowTonatiuh.addShapeKit()	               	
    		}
        }
    }	
    
    	
}