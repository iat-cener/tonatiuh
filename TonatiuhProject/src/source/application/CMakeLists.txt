FILE(GLOB APPLICATION_SRC
        ${PROJECT_SOURCE_DIR}/src/source/application/*.cpp
        ${PROJECT_SOURCE_DIR}/src/auxiliary/*.cpp
        ${PROJECT_SOURCE_DIR}/src/plugins/PluginManager.cpp
)


find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Qt5 REQUIRED
    Core
    Xml
    Widgets
)



ADD_EXECUTABLE( Tonatiuh 
	${APPLICATION_SRC}
)

SET_TARGET_PROPERTIES( Tonatiuh PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS Tonatiuh  DESTINATION bin)

set_target_properties(Tonatiuh PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(	Tonatiuh PUBLIC
						${PROJECT_SOURCE_DIR}/src/geometry
						${PROJECT_SOURCE_DIR}/src/auxiliary
						${PROJECT_SOURCE_DIR}/src/nodes
						${PROJECT_SOURCE_DIR}/src/plugins
						${PROJECT_SOURCE_DIR}/src/raytracing
						${PROJECT_BINARY_DIR}/src/source/application
)	


if(UNIX)								
	TARGET_LINK_LIBRARIES(Tonatiuh
			dl
			stdc++fs 
	        Qt5::Core
	        Qt5::Xml
	        Qt5::Widgets
	        geometry
	        nodes
	        raytracing
	        statistics
	)

else()
	TARGET_LINK_LIBRARIES(Tonatiuh
			stdc++fs 
	        Qt5::Core
	        Qt5::Xml
	        Qt5::Widgets
	        geometry
	        nodes
	        raytracing
	        statistics
	)
endif(UNIX)	
