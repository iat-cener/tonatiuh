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

RESOURCES += src/TransmissivityBallestrin.qrc
TARGET        = TransmissivityBallestrin

CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TransmissivityBallestrin
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TransmissivityBallestrin
}
