TEMPLATE      = lib
CONFIG       += plugin debug_and_release

include( ../../config.pri )

INCLUDEPATH +=  . \
				src  \
                $$(TONATIUH_ROOT)/src/source/geometry \
                $$(TONATIUH_ROOT)/src/source/raytracing

# Input
HEADERS = src/*.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.h \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.h 

SOURCES = src/*.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTracker.cpp \
            $$(TONATIUH_ROOT)/src/source/raytracing/TTrackerForAiming.cpp 

RESOURCES += src/TrackerLinearFresnel.qrc

TARGET        = TrackerLinearFresnel

LIBS +=-L$$(TDE_ROOT)/local/lib -lCoin -lSoQt
    
CONFIG(debug, debug|release) {
    DESTDIR       = $$(TONATIUH_ROOT)/bin/debug/plugins/TrackerLinearFresnel    
}
else { 
    DESTDIR       = $$(TONATIUH_ROOT)/bin/release/plugins/TrackerLinearFresnel
}   