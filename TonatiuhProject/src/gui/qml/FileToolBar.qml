

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {
	id: fileToolbar		
		    		    
    RowLayout {
        anchors.fill: parent
        ToolButton {
            icon.source: "/../qml/icons/new.png"
            icon.color : "transparent"
            ToolTip.visible: hovered
			ToolTip.text: qsTr("New")
            
        }
        ToolButton {
            icon.source: "/../qml/icons/open.png"
            icon.color : "transparent"

        }
        ToolButton {
            icon.source: "/../qml/icons/save.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/saveas.png"
            icon.color : "transparent"
        }
        ToolSeparator {}
        ToolButton {
            icon.source: "/../qml/icons/print.png"
            icon.color : "transparent"
        }
        ToolSeparator {}
        ToolButton {
            icon.source: "/../qml/icons/close.png"
            icon.color : "transparent"
        }
    }
}	