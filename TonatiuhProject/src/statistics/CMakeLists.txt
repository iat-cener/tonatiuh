FILE(GLOB statistics ${PROJECT_SOURCE_DIR}/src/statistics/*.cpp)

ADD_LIBRARY(statistics SHARED  ${statistics})

INSTALL(TARGETS statistics DESTINATION bin)

SET_TARGET_PROPERTIES(statistics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(statistics PUBLIC ${PROJECT_SOURCE_DIR}/src/statistics )

