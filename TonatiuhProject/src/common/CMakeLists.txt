FILE(GLOB common ${PROJECT_SOURCE_DIR}/src/common/*.cpp)

ADD_LIBRARY(common SHARED  ${common})

INCLUDE_DIRECTORIES("../nodes" "../geometry")

INSTALL(TARGETS common DESTINATION bin)

SET_TARGET_PROPERTIES(common PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(common PUBLIC  ${PROJECT_SOURCE_DIR}/src/nodes ${PROJECT_SOURCE_DIR}/src/common )

target_link_libraries( common nodes  )