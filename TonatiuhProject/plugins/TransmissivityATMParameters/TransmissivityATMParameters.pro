TEMPLATE      = lib
CONFIG       += plugin
include( ../../config.pri )

INCLUDEPATH +=  . \
                src \
                $$(TONATIUH_ROOT)/plugins \
                $$(TONATIUH_ROOT)/src \
                $$(TONATIUH_ROOT)/src/raytracing

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.h 

SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.cpp

RESOURCES += src/TransmissivityATMParameters.qrc
TARGET        = TransmissivityATMParameters

CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TransmissivityATMParameters
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TransmissivityATMParameters
}
