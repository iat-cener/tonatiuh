TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( $$(TONATIUH_ROOT)/config.pri )

INCLUDEPATH +=  . \
                src \
                $$(TONATIUH_ROOT)/plugins \
                $$(TONATIUH_ROOT)/src \
                $$(TONATIUH_ROOT)/src/raytracing

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/trt.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivity.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTransmissivityFactory.h


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
