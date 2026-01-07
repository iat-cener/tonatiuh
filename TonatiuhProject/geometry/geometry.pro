TEMPLATE = lib
CONFIG       += warn_on thread      
CONFIG       -= qt
QT -= core

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

