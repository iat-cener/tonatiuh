

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {  
 	id: viewToolbar      
 	
    RowLayout {
        anchors.fill: parent
        ToolButton {
            icon.source: "/../qml/icons/viewer_viewXZ.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/viewer_viewXZ.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/viewer_viewYZ.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/viewer_viewSun.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/grid.png"
            icon.color : "transparent"
        }
    }		
} 
  