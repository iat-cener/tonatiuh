import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4


Rectangle {  
    objectName : "parametersView"	
	TabView {
		anchors.fill: parent
    	anchors.margins: 4
    	
    	ListModel {
		    id: libraryModel
		    ListElement {
		        parameter: "translation"
		        value: "0 0 0"
		    }
		    ListElement {
		        parameter: "rotation"
		        value: "0 0 1 0"
		    }
		    ListElement {
		        parameter: "scaleFactor"
		        value: "1 1 1"
		    }
		    ListElement {
		        parameter: "scaleOrientation"
		        value: "0 0 1 0"
		    }
		    ListElement {
		        parameter: "center"
		        value: "0 0 0"
		    }
		}

	    Tab {
	        title: "Transform"
	        id: tab
	        
	
			TableView {
			    TableViewColumn {
			        role: "parameter"
			        title: "Parameter"
			        width: 100
			    }
			    TableViewColumn {
			        role: "value"
			        title: "Value"
			        width: 200
			        delegate:
	                    TextEdit {
		                    text: styleData.value
		                   
		                }

			    }
			    model: libraryModel
			}
	        
	    }
	  
	}	
    
}

