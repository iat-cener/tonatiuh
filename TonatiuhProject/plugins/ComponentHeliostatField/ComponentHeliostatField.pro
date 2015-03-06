TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

	
INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
                $$(TONATIUH_ROOT)/src/source/gui \
                $$(TONATIUH_ROOT)/src/source/raytracing

# Input
HEADERS = src/*.h \
                $$(TONATIUH_ROOT)/src/source/gui/PluginManager.h

SOURCES = src/*.cpp  \
                $$(TONATIUH_ROOT)/src/source/gui/PluginManager.cpp

FORMS += src/*.ui

RESOURCES += src/ComponentHeliostatField.qrc

TARGET        = ComponentHeliostatField
 
CONFIG(debug, debug|release) {
	DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/ComponentHeliostatField	
	unix {
		TARGET = $$member(TARGET, 0)_debug
	}
	else {
		TARGET = $$member(TARGET, 0)d
	}
}
else { 
	DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/ComponentHeliostatField
}