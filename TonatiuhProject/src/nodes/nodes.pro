TEMPLATE = lib
CONFIG       += debug_and_release

CONFIG       += qt warn_on thread debug_and_release


CONFIG(debug, debug|release) {
	OBJECTS_DIR = $$(TONATIUH_ROOT)/debug
	MOC_DIR = $$(TONATIUH_ROOT)/debug
	OBJMOC = $$(TONATIUH_ROOT)/debug
	RCC_DIR = $$(TONATIUH_ROOT)/debug
}
else { 
	OBJECTS_DIR = $$(TONATIUH_ROOT)/release
	MOC_DIR = $$(TONATIUH_ROOT)/release
	OBJMOC = $$(TONATIUH_ROOT)/release
	RCC_DIR  = $$(TONATIUH_ROOT)/release
}


QT += xml
	

TARGET = nodes   

INCLUDEPATH += 	. \
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

