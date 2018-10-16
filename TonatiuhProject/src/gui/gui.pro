TEMPLATE = lib
CONFIG       += warn_on thread debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )


QT +=core	
QT +=gui	
QT +=widgets
QT += printsupport
QT += 3dcore 3drender 3dinput 3dextras datavisualization qml quick widgets quickwidgets declarative


TARGET = gui

INCLUDEPATH += 	. \
			   $$(TONATIUH_ROOT)/src/auxiliary \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \               
               $$(TONATIUH_ROOT)/src/statistics \
               $$(TONATIUH_ROOT)/src/common \
               $$(TONATIUH_ROOT)/src/gui \
               $$(TONATIUH_ROOT)/src/gui/qml 
               
               


CONFIG += console

CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lcommon
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lcommon
}



# Input
HEADERS += *.h 
			

SOURCES += *.cpp  
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}

