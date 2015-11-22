CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
cmake_policy(VERSION 3.2.2)

PROJECT(tiled-reader CXX C)

IF(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
	set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
ENDIF()

if((NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}) AND (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/.gitignore"))
    # Don't polute the git repository with the generated files for out-of-source builds
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.gitignore "*")
endif()

MESSAGE("== Tiled C++ Reader ==")

# Find libjson
add_subdirectory(externals/libjson)

# 
SET(TILEDREADER_SOURCE
include/exceptions.h
include/fileloader.h
include/layer.h
include/map.h
include/object.h
include/stdiofileloader.h
include/tileset.h
src/layer.cc
src/map.cc
src/object.cc
src/tileset.cc
)

add_library(tiled-reader ${TILEDREADER_SOURCE})
target_include_directories(tiled-reader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tiled-reader PUBLIC libjson)

# Add tests
add_subdirectory(test)