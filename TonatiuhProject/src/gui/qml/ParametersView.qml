import QtQuick 2.11
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4


Page {
    property string recColor : "steelblue"
    
    SwipeView {
        id: swipeView
        objectName: "swipeView"
        currentIndex: tabBar.currentIndex
                
		Rectangle {
			id: rectParamView1
			objectName: "rectParamView1"
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
			id: rectParamView2
			objectName: "rectParamView2"
    		border.color: "black"
    		border.width: 2	       	
			height:  parametersView.height - tabBar.height
			width: parametersView.width
			GenericParametersView {
				id: genericParameterView2
				objectName: "genericParameterView2"
			}
		}  
       
    }

    header: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        
        TabButton {
        	id: tab1
            text: "Transform"
            width: implicitWidth
            height: 20    
            
            background: Rectangle {
             	color: tabBar.currentIndex == 0 ? "steelblue" : "powderblue"            	           
            }   
            onClicked: { 
            	console.log("clickada la primera tab")
            	console.log("swipeView.currentIndex", swipeView.currentIndex)
            	//rectParamView1.visible = true
            	//rectParamView2.visible = false   
            	
            }
            
        }
        TabButton {
        	id: tab2
            text: "Second"
            width: implicitWidth
            height: 20   
            visible : false  
           
            background: Rectangle {
            	color: tabBar.currentIndex == 1 ? "steelblue" : "powderblue"
            }                      
            onClicked: { 
            	console.log("clickada la segunda tab")        
            	//rectParamView1.visible = false
            	//rectParamView2.visible = true
            	   	
            }
        }       
        
    }
        
}