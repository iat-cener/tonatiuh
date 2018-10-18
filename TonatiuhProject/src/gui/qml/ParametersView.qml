import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4


Page {
    property string recColor : "steelblue"
    
     
    //contentWidth : parametersView.width
    //contentHeight: parent.height
    //anchors.fill: parent
    //anchors.margins: 4

    SwipeView {
        id: swipeView
        objectName: "swipeView"
        //anchors.fill: parent
        //width: parametersView.width -4
        //currentIndex: tabBar.currentIndex
        //anchors.margins: 4
        
		Rectangle {	
			objectName: "rectParamView"
    		border.color: "steelblue"
    		border.width: 2	   
    	
			height:  parametersView.height - tabBar.height
			width: parametersView.width
			GenericParametersView {
				id: genericParameterView
				objectName: "genericParameterView"
			}
		}  
		
		Rectangle {	
			objectName: "rectParamView2"
    		border.color: "steelblue"
    		border.width: 2	   
    	
			height:  parametersView.height - tabBar.height
			width: parametersView.width
			GenericParametersView {
				id: genericParameterView2
				objectName: "genericParameterView2"
			}
		}     
        /*Repeater {
        	objectName : "repeater"
            model: 2
			Rectangle {	
				objectName: "rectParamView"
	    		border.color: "steelblue"
	    		border.width: 2	   
	    	
				height:  parametersView.height - tabBar.height
				width: parametersView.width
				GenericParametersView {
					id: genericParameterView
					objectName: "genericParameterView"
				}
			}            
        }*/
    }

    header: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        
        TabButton {
            text: "Transform"
            width: implicitWidth
            height: 20    
            
            background: Rectangle {
             	color: tabBar.currentIndex == 0 ? "steelblue" : "powderblue"            	           
            }   
            onClicked: { 
            	console.log("clickada la primera tab")
            	page.recColor = "steelblue"
            }
            
        }
        TabButton {
            text: "Second"
            width: implicitWidth
            height: 20   
            visible : false  
           
            background: Rectangle {
            	color: tabBar.currentIndex == 1 ? "steelblue" : "powderblue"
    
            }       
               
            onClicked: { 
            	console.log("clickada la segunda tab")
            	page.recColor = "red"
            }
        }
        
    }
}