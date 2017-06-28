TEMPLATE = subdirs

geometry.target = geometry
geometry.CONFIG = recursive
geometry.recurse = geometry   

nodes.target = nodes
nodes.CONFIG = recursive
nodes.depends = geometry
nodes.recurse = nodes  


plugins.target = plugins
plugins.CONFIG = recursive
plugins.depends = geometry nodes
plugins.recurse = plugins  
 


QMAKE_EXTRA_TARGETS += geometry
SUBDIRS = src/geometry \
		src/nodes \
		src/plugins \
		src/source