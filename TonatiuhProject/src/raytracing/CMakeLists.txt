FILE(GLOB raytracing ${PROJECT_SOURCE_DIR}/src/raytracing/*.cpp)

ADD_LIBRARY(raytracing SHARED  ${raytracing})

INSTALL(TARGETS raytracing DESTINATION bin)

SET_TARGET_PROPERTIES(raytracing PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(raytracing PUBLIC ${PROJECT_SOURCE_DIR}/src/raytracing )

target_link_libraries( raytracing geometry 
										nodes 
										statistics )