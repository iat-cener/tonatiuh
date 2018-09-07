

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {  
 	id: doUndoToolbar      
    RowLayout {
        anchors.fill: parent
        ToolButton {
            icon.source: "/../qml/icons/undo.png"
            icon.color : "transparent"
        }
        ToolButton {
            icon.source: "/../qml/icons/redo.png"
            icon.color : "transparent"
        }
    }

}