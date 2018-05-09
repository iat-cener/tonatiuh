SET( PLUGIN PhotonMapExportToBinaryFile )

find_package(Qt5Core REQUIRED)
find_package(Qt5 REQUIRED
    Core
)

file(GLOB ${PLUGIN}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )

add_library( ${PLUGIN}  SHARED ${${PLUGIN}_SRC} )

set_target_properties(${PLUGIN} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


target_include_directories(	${PLUGIN} PUBLIC
						${PROJECT_SOURCE_DIR}/src/geometry
						${PROJECT_SOURCE_DIR}/src/plugins
						${PROJECT_SOURCE_DIR}/src/raytracing
)

target_link_libraries( ${PLUGIN}
	        	Qt5::Core
	        	geometry
	        	raytracing
)

SET_TARGET_PROPERTIES(${PLUGIN} PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS ${PLUGIN} DESTINATION bin )
