import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4


Rectangle {  
    objectName : "parametersView"
    //id: parametersView
    /*width: parent.width
	anchors.left: parent.left
	anchors.leftMargin : 5
	anchors.right: parent.right
	anchors.rightMargin : 5
	anchors.top: parent.bottom
	anchors.topMargin : 5
	anchors.bottom: parent.bottom
	anchors.bottomMargin : 5*/
	
	TabBar {
		TabButton  {
	        text: "Transform"
	        //Rectangle { color: "red" }   
		
			/*Grid {
    			columns: 3
    			spacing: 2
			    Rectangle { color: "red"; height:25 }
			    Rectangle { color: "green"; height:25 }
			    Rectangle { color: "blue"; height:25 }
			    Rectangle { color: "cyan"; height:25 }
			    Rectangle { color: "magenta"; height:25 }
			/*}
			
			/*ListView {
		        id: listView
		        anchors.fill: parent
		
		        contentWidth: headerItem.width
		        flickableDirection: Flickable.HorizontalAndVerticalFlick
		
		        header: Row {
		            spacing: 1
		            function itemAt(index) { return repeater.itemAt(index) }
		            Repeater {
		                id: repeater
		                model: ["Quisque", "Posuere", "Curabitur", "Vehicula", "Proin"]
		                Label {
		                    text: modelData
		                    font.bold: true
		                    font.pixelSize: 20
		                    padding: 10
		                    background: Rectangle { color: "silver" }
		                }
		            }
		        }
		        
		        model: 100
		        delegate: Column {
		            id: delegate
		            property int row: index
		            Row {
		                spacing: 1
		                Repeater {
		                    model: 5
		                    ItemDelegate {
		                        property int column: index
		                        text: qsTr("%1x%2").arg(delegate.row).arg(column)
		                        width: listView.headerItem.itemAt(column).width
		                    }
		                }
		            }
		            Rectangle {
		                color: "silver"
		                width: parent.width
		                height: 1
		            }
		        }
		
		        ScrollIndicator.horizontal: ScrollIndicator { }
		        ScrollIndicator.vertical: ScrollIndicator { }
		    }*/
	    }
    }
}

