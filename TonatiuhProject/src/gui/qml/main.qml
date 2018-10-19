import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.11
import Qt.labs.settings 1.0

import QtQuick 2.11
//import windowmaincontroller 1.0


ApplicationWindow {
    id: windowTonatiuh
    objectName: "windowTonatiuh"
    visible: true
    width: 1000
    minimumWidth:600 
    height: 600  
    minimumHeight: 500
   
    property bool showBckgrnd: true
    
    // File callbacks
    signal newFileCb()
	signal openFileCb(string fileUrl)
	signal saveFileCb()
	signal saveFileAsCb(string fileUrl)
	signal printFileCb()
	signal closeFileCb()
    
    //Insert Elements callbacks
    signal addElement(string eleFile)
    signal addSeparatorKitCb()
    signal addShapeKitCb()
    signal addNodeCb(string nodeType, string nodeName)
    
    //Update Parameter View
    signal updateParameterViewCb(string nodeName, string pluginName)
    
    signal loadChildParametersViewCb(string nodeUrl, string childUrl1, string childName1, string childPluginName1,
    	string childUrl2, string childName2, string childPluginName2)
      
    signal showBackground()
    
    onWidthChanged: { 	
    	tonatiuhToolBar.updateSizeToolBar(windowTonatiuh.width - 12)
    }
            
 	onAddElement: tonatiuhContent.addElement(eleFile)

 	
 	onShowBackground: {
 		tonatiuhContent.showBackground(showBckgrnd) 
 		showBckgrnd = ! showBckgrnd
	}		
		
	Component.onCompleted: {
		windowTonatiuh.showBackground()
		//windowMainController.init(windowTonatiuh)
	}
 	    
	menuBar: TonatiuhMenuBar{
		id: tonatiuhMenuBar
		objectName: "tonatiuhMenuBar"
	}
    
	header: TonatiuhToolBar{
		id: tonatiuhToolBar
		objectName: "tonatiuhToolBar"
	}
	
	TonatiuhContent {
		id: tonatiuhContent
		objectName: "tonatiuhContent"
	}
	
	Item {
        states: [
            State {
                when: windowTonatiuh.fullscreen
                name: "Full"
                PropertyChanges {
                    target: windowTonatiuh
                    // [1] using Window.FullScreen => still have black flashes.
                    //     have to use "FullScreen"
                    // visibility: Window.FullScreen
                    visibility: "FullScreen"

                    // [2] The unfamous workaround
                    flags: Qt.WindowFullScreen
                    
                }
            }
        ]
    }
    
    FileDialog {
    	id: openFileDialog
    	objectName: "openfileDialog"
    }
    
    
    Settings {
    	id: settings
    	
    	
    	property alias windowX: windowTonatiuh.x 
        property alias windowY: windowTonatiuh.y 
        /*property alias windowWidth: windowTonatiuh.width 
        property alias windowHeight: windowTonatiuh.height */

        property alias windowVisibility: windowTonatiuh.visibility 

    }   
    
    
    Component.onDestruction: {
        //Save Settings
    }    
    
    
    
	/*WindowMainController {
		id: windowMainController		
	}*/
	
}