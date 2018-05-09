FILE(GLOB geometry ${PROJECT_SOURCE_DIR}/src/geometry/*.cpp)

ADD_LIBRARY(geometry SHARED  ${geometry})

INSTALL(TARGETS geometry DESTINATION bin)

SET_TARGET_PROPERTIES(geometry PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(geometry PUBLIC ${PROJECT_SOURCE_DIR}/src/geometry )

