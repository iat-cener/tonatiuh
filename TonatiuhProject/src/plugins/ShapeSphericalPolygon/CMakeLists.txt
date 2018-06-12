SET( PLUGIN ShapeSphericalPolygon )

file(GLOB ${PLUGIN}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )

add_library( ${PLUGIN}  SHARED ${${PLUGIN}_SRC} )

target_include_directories(	${PLUGIN} PUBLIC
						${PROJECT_SOURCE_DIR}/src/geometry
						${PROJECT_SOURCE_DIR}/src/nodes
						${PROJECT_SOURCE_DIR}/src/plugins
)

target_link_libraries( ${PLUGIN}
	        	geometry
	        	nodes
)

SET_TARGET_PROPERTIES(${PLUGIN} PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS ${PLUGIN} DESTINATION bin )
