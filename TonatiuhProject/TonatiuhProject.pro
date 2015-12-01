TEMPLATE = subdirs

geometry.target = geometry
geometry.CONFIG = recursive
geometry.recurse = geometry   


fields.target = fields
fields.CONFIG = recursive
fields.recurse = fields   

src.target = src
src.CONFIG = recursive
src.recurse = src	
src.depends = geometry

plugins.target = plugins
plugins.CONFIG = recursive
plugins.recurse = plugins	
plugins.depends = geometry

tests.target = tests
tests.CONFIG = recursive
tests.recurse = tests
tests.depends = geometry

QMAKE_EXTRA_TARGETS += src plugins tests
SUBDIRS = geometry \
		fields \
		src \
          plugins \
          tests
            