import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.11

import QtQuick 2.11
import windowmain 1.0


ApplicationWindow {
    id: windowTonatiuh
    objectName: "windowTonatiuh"
    visible: true
    width: 1000
    minimumWidth:600 
    height: 600  
    minimumHeight: 500
   
    property bool showBckgrnd: true
    
    signal addElement(string eleFile)
    signal addSeparatorKit()
    signal addShapeKit()
    
    signal showBackground()
    
    onWidthChanged: { 
    	console.log("2width", windowTonatiuh.width)
    	tonatiuhToolBar.updateSizeToolBar(windowTonatiuh.width - 12)
    }
            
 	onAddElement: content.addElement(eleFile)
 	onAddSeparatorKit: content.addSeparatorKit()
 	onAddShapeKit: content.addShapeKit()
 	onShowBackground: {
 		content.showBackground(showBckgrnd) 
 		showBckgrnd = ! showBckgrnd
	}		
	
	signal addNode(string nodeType)
	
	onAddNode: console.log("add node", nodeType)
	
	signal addHeliostatTracker()
    signal addLinearFresnelTracker()
    signal addOneAxisTracker()
    signal addZAxisTracker()
	
	onAddHeliostatTracker: console.log("add tracker")
    onAddLinearFresnelTracker: console.log("add lf tracker")
    onAddOneAxisTracker: console.log("add 1 axis tracker")
    onAddZAxisTracker: console.log("add z axix tracker")
	
	Component.onCompleted: windowTonatiuh.showBackground()
 	    
	menuBar: TonatiuhMenuBar{
		id: tonatiuhMenuBar
	}
    
	header: TonatiuhToolBar{
		id: tonatiuhToolBar
	}
	
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