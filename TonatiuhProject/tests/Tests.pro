TEMPLATE = app

TARGET = TonatiuhTests

CONFIG += console

include( ../config.pri )

INCLUDEPATH += . \
				src \
                $$(TONATIUH_ROOT)/plugins \
				$$(TONATIUH_ROOT)/src \
                $$(TDE_ROOT)/local/include
SOURCES += *.cpp \
           $$(TONATIUH_ROOT)/src/source/geometry/*.cpp
           
LIBS += -L$$(TDE_ROOT)/local/lib -lgtest
