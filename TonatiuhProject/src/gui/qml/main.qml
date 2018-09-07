import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.11

import QtQuick 2.11

ApplicationWindow {
    id: windowTonatiuh
    objectName: "windowTonatiuh"
    visible: true
    width: 1000
    minimumWidth:600 
    height: 500  
    minimumHeight: 500
   
    property bool showBckgrnd: true
    
    signal addElement(string eleFile)
    signal addSeparatorKit()
    signal addShapeKit()
    signal showBackground()
    
 	onAddElement: content.addElement(eleFile)
 	onAddSeparatorKit: content.addSeparatorKit()
 	onAddShapeKit: content.addShapeKit()
 	onShowBackground: {
 		content.showBackground(showBckgrnd) 
 		console.log("show backgrnd", showBckgrnd)
 		showBckgrnd = ! showBckgrnd
	}	
	
	Component.onCompleted: windowTonatiuh.showBackground()
 	    
	menuBar: TonatiuhMenuBar{}
    
	header: TonatiuhToolBar{}
	
	TonatiuhContent {
		id: content
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
	
	
}