TEMPLATE = lib
CONFIG       += debug_and_release unversioned_libname skip_target_version_ext
include( $$(TONATIUH_ROOT)/config.pri )


DEFINES += GEOMETRYLIBRARY_EXPORTS

QT -=core	
QT -=gui	


TARGET = geometry   

DEPENDPATH += . \
                $$(TONATIUH_ROOT)

 
# Input
HEADERS += *.h \

SOURCES += *.cpp \
          
 

CONFIG(debug, debug|release) {
	DESTDIR = $$(TONATIUH_ROOT)/bin/debug
}
else{
	DESTDIR= $$(TONATIUH_ROOT)/bin/release
}

