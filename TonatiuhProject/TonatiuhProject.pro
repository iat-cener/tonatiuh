TEMPLATE = subdirs

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
		
SUBDIRS = src \
          plugins \
          tests
            