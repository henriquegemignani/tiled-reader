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
if(NOT TARGET libjson)
	add_subdirectory(externals/libjson)
endif()

# 
SET(TILEDREADER_SOURCE
include/tiled-reader/exceptions.h
include/tiled-reader/fileloader.h
include/tiled-reader/layer.h
include/tiled-reader/map.h
include/tiled-reader/object.h
include/tiled-reader/stdiofileloader.h
include/tiled-reader/tileset.h
src/exceptions.cc
src/layer.cc
src/map.cc
src/object.cc
src/tileset.cc
)

if (UNIX OR MINGW)
    add_definitions(-ansi -U_FORTIFY_SOURCE -std=c++14)
    #add_definitions(-msse2)
    add_definitions(-Wall
        # TODO: would like these but they produce overwhelming amounts of warnings
        -Wshadow                       # GLM causes many warnings with this
        #-Wextra                        # implies -Wunused-parameter
        -Wmissing-field-initializers
        #-Wswitch-default               # Currently ignored.
        -Wconversion                   # GLM causes many warnings with this
        -Wzero-as-null-pointer-constant
        -Wtype-limits
        -Wsign-compare
        -Wignored-qualifiers
        -Wuninitialized
        -Winit-self
    )
        
    set_source_files_properties(${GENERATED_SRC}
        PROPERTIES 
            COMPILE_FLAGS
                "-fpermissive -Wno-unused-value")
            
endif()

# Disable some warnings for Clang.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(
        -Wno-self-assign
    )
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4")
        add_definitions(
            -Wno-deprecated-register
        )
    endif()
endif ()

if (MSVC) 
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /W4 /wd4100 /wd4127 /wd4201 /wd4211 /wd4250 /wd4706 /MP)
endif ()

add_library(tiled-reader ${TILEDREADER_SOURCE})
target_include_directories(tiled-reader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tiled-reader PUBLIC libjson)

# Add tests
add_subdirectory(test)