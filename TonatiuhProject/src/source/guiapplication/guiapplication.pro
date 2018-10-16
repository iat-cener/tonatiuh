TEMPLATE = app

include( $$(TONATIUH_ROOT)/config.pri )

#TARGET = guiapplication   

              
INCLUDEPATH += 	. \
			   $$(TONATIUH_ROOT)/src/auxiliary \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \
               $$(TONATIUH_ROOT)/src/plugins \
               $$(TONATIUH_ROOT)/src/raytracing \
               $$(TONATIUH_ROOT)/src/statistics \
               $$(TONATIUH_ROOT)/src/common \
               $$(TONATIUH_ROOT)/src/gui 
               
               
              
QT += core xml widgets quickwidgets qml quick 3dinput 


CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry -lnodes -lstatistics -lraytracing -lcommon -lstdc++fs 
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry -lnodes -lstatistics -lraytracing -lcommon -lstdc++fs 
}

unix {
   	LIBS += -ldl
}



# Input
HEADERS += 	$$(TONATIUH_ROOT)/src/auxiliary/Trace.h \
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.h \
			$$(TONATIUH_ROOT)/src/plugins/PluginManager.h \
			$$(TONATIUH_ROOT)/src/source/guiapplication/*.h \
			$$(TONATIUH_ROOT)/src/gui/*.h
                
SOURCES +=	$$(TONATIUH_ROOT)/src/auxiliary/Trace.cpp \
			$$(TONATIUH_ROOT)/src/auxiliary/Tracer.cpp \
			$$(TONATIUH_ROOT)/src/plugins/PluginManager.cpp \
			$$(TONATIUH_ROOT)/src/source/guiapplication/*.cpp \
			$$(TONATIUH_ROOT)/src/gui/*.cpp

RESOURCES += ../../../tonatiuh.qrc \
			 ../../gui/gui.qrc

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}
	
QMAKE_CLEAN -= *.rc  

src.target= src
QMAKE_EXTRA_TARGETS += src