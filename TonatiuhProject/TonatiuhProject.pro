TEMPLATE = subdirs

geometry.target = geometry
geometry.CONFIG = recursive
geometry.recurse = geometry   


src.target = src
src.CONFIG = recursive
src.recurse = src	
tests.depends = geometry

plugins.target = plugins
plugins.CONFIG = recursive
plugins.recurse = plugins	

tests.target = tests
tests.CONFIG = recursive
tests.recurse = tests

QMAKE_EXTRA_TARGETS += src plugins tests
SUBDIRS = geometry \
src \
          plugins \
          tests
            