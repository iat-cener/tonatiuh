FILE(GLOB nodes ${PROJECT_SOURCE_DIR}/src/nodes/*.cpp)

ADD_LIBRARY(nodes SHARED  ${nodes})

INSTALL(TARGETS nodes DESTINATION bin)

SET_TARGET_PROPERTIES(nodes PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(nodes PUBLIC ${PROJECT_SOURCE_DIR}/src/nodes
 ${PROJECT_SOURCE_DIR}/src/geometry )
 
target_link_libraries( nodes geometry )
