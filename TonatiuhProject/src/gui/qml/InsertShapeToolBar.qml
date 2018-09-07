

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11




ToolBar {  
 	id: insertShapeToolbar      
    RowLayout {
    	id: insertToolbarRow
    	objectName: "insertToolbarRow"
        anchors.fill: parent
     
        
        ToolButton {
        	id: insertTorus
        	objectName: "insertTorus"
            icon.source: "/../qml/icons/torusIcon.png"
            icon.color : "transparent"
            onClicked: {
            	windowTonatiuh.addSeparatorKit()	               	
    		}
           
        }
        
    }	
    
    	
}