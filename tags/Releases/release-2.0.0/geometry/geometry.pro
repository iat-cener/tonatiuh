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
	
#include( ../config.pri )

TARGET = geometry   

DEPENDPATH += . \
                $$(TONATIUH_ROOT)

 



# Input
HEADERS += *.h \

SOURCES += *.cpp \
          
 

CONFIG(debug, debug|release) {
	DESTDIR = ../bin/debug
}
else{
	DESTDIR= ../bin/release
}

