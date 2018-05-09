TEMPLATE = lib
CONFIG       += warn_on thread debug_and_release

CONFIG       += unversioned_libname
CONFIG       += skip_target_version_ext

include( $$(TONATIUH_ROOT)/config.pri )


QT -=core	
QT -=gui	

TARGET = nodes

DEFINES += NODELIBRARY_EXPORTS   

INCLUDEPATH += 	. \
			   $$(TONATIUH_ROOT)/src/auxiliary \
               $$(TONATIUH_ROOT)/src/geometry \
               $$(TONATIUH_ROOT)/src/nodes \
               $$(TONATIUH_ROOT)/src/statistics


CONFIG(debug, debug|release) {
   	LIBS += -L$$(TONATIUH_ROOT)/bin/debug -lgeometry 
}else{
   	LIBS += -L$$(TONATIUH_ROOT)/bin/release -lgeometry
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

