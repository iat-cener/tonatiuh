import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import QtQuick 2.11

Entity {
    id: torusEntity  
    objectName: "torusEntity"
    property color torusColor : "black"
    
    TorusMesh {
        id: torusMesh
        radius: 5
        minorRadius: 1
        rings: 100
        slices: 20    
    }

    PhongMaterial {
        id: material
        property color torusMaterialColor : torusColor
        diffuse : "green"
        ambient : torusColor
        
        MouseArea {
        id: materialArea
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        
       // hoverEnabled: true
        //property string buttonID
        onPressed: {
        	if (mouse.button == Qt.LeftButton)  {      
        		torusMaterialColor:"red"
            	console.log("little-press")
            	}
        }
     }
    }

    Transform {
        id: torusTransform
        scale3D: Qt.vector3d(1.5, 1, 0.5)
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
    }

    components: [torusMesh, material, torusTransform ]
    
    ObjectPicker {
        id: picker
        onClicked: {
            console.log("Picked")
            torusMaterialColor:"red"
            torusColor:"red"
        }
        onPressed: {
            console.log("Picked")
            torusMaterialColor:"red"
            torusColor:"red"
            console.log("Pressed at: " + pick.worldIntersection)
     		//If using triangle picking, you can also see index of the pressed triangle
     		console.log("Triangle index: " + pick.triangleIndex)
            
        }
    }

    
     
    
}

