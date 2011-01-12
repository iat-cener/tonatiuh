TEMPLATE = subdirs

src.target = src
src.CONFIG = recursive
src.recurse = src	

plugins.target = plugins
plugins.CONFIG = recursive
plugins.recurse = plugins	

tests.target = tests
tests.CONFIG = recursive
tests.recurse = tests

QMAKE_EXTRA_TARGETS += src plugins tests
SUBDIRS = src \
          plugins \
          tests
            