FILE(GLOB APPLICATION_SRC
        ${PROJECT_SOURCE_DIR}/src/source/application/*.cpp
        ${PROJECT_SOURCE_DIR}/src/auxiliary/*.cpp
        ${PROJECT_SOURCE_DIR}/src/plugins/PluginManager.cpp
)

#target_include_directories( ${APPLICATION_SRC} PUBLIC 	${PROJECT_SOURCE_DIR}/src/source/application
#														${PROJECT_SOURCE_DIR}/src/auxiliary
# )


FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)

find_package(Qt5 REQUIRED
    Core
    Xml
    Widgets
)


#find_package(Qt5Core  REQUIRED)
#find_package(Qt5XML  REQUIRED)

#FILE(GLOB APPLICATION_MOC_RES
#        ${PROJECT_SOURCE_DIR}/src/source/application/*.qrc
#        ${PROJECT_SOURCE_DIR}/src/tonatiuh.qrc
#)


#QT4_ADD_RESOURCES(APPLICATION_RES ${APPLICATION_MOC_RES})

#FILE(GLOB APPLICATION_UIS
#        ${PROJECT_SOURCE_DIR}/src/source/application/*.ui
#)
#QT4_WRAP_UI(APPLICATION_UI_HEADERS ${APPLICATION_UIS} )

#FILE(GLOB APPLICATION_MOC_HDRS
#        ${PROJECT_SOURCE_DIR}/src/source/application/*.h
#)
#QT4_WRAP_CPP(APPLICATION_MOC_SRCS ${APPLICATION_MOC_HDRS})


ADD_EXECUTABLE(tonatiuh 
#	${APPLICATION_UI_HEADERS}
	${APPLICATION_SRC}
#	${APPLICATION_MOC_SRCS}
#	${APPLICATION_RES}
)

SET_TARGET_PROPERTIES(tonatiuh PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS tonatiuh  DESTINATION bin)

set_target_properties(tonatiuh PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(	tonatiuh PUBLIC
						${PROJECT_SOURCE_DIR}/src/geometry
						${PROJECT_SOURCE_DIR}/src/auxiliary
						${PROJECT_SOURCE_DIR}/src/nodes
						${PROJECT_SOURCE_DIR}/src/plugins
						${PROJECT_SOURCE_DIR}/src/raytracing
						${PROJECT_BINARY_DIR}/src/source/application
)						
						
TARGET_LINK_LIBRARIES(tonatiuh
		stdc++fs 
        Qt5::Core
        Qt5::Xml
        Qt5::Widgets
        GEOMETRYLIBRARY
        NODELIBRARY
        RAYTRACINGLIBRARY
        STATISTICSLIBRARY
)
