

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.1


ToolBar {  
     	id: runToolbar      
        RowLayout {
            anchors.fill: parent
            ToolButton {
            	id: run
                icon.source: "/../qml/icons/run.png"
                icon.color : "transparent"
                background: Rectangle {
	        		color: Qt.darker("#33333333", run.enabled && (run.checked || run.highlighted) ? 1.5 : 1.0)
        			opacity: enabled ? 1 : 0.3
        			visible: run.down || (run.enabled && (run.checked || run.highlighted))
    			}
            }
            ToolButton {
                icon.source: "/../qml/icons/fluxdistribution.png"
                icon.color : "transparent"
                id: fluxDistribution
                background: Rectangle {
	        		color: Qt.darker("#33333333", fluxDistribution.enabled && (fluxDistribution.checked || fluxDistribution.highlighted) ? 1.5 : 1.0)
        			opacity: enabled ? 1 : 0.3
        			visible: fluxDistribution.down || (fluxDistribution.enabled && (fluxDistribution.checked || fluxDistribution.highlighted))
    			}
                
            }            
        }	
        background: Rectangle {
    		color: "white"
		}	
    }