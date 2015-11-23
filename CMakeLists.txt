CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
cmake_policy(VERSION 3.0.0)

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
src/fileloader.cc
src/layer.cc
src/map.cc
src/object.cc
src/tileset.cc
)

add_library(tiled-reader ${TILEDREADER_SOURCE})
target_include_directories(tiled-reader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tiled-reader PUBLIC libjson)

    macro (tiledreader_add_compile_flag target flag)
        set_property(TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}")
    endmacro (tiledreader_add_compile_flag)

if (UNIX OR MINGW)
    tiledreader_add_compile_flag(tiled-reader -U_FORTIFY_SOURCE)
    tiledreader_add_compile_flag(tiled-reader -std=c++14)
    tiledreader_add_compile_flag(tiled-reader -Wall)
    tiledreader_add_compile_flag(tiled-reader -Wshadow) # GLM causes many warnings with this)
    #tiledreader_add_compile_flag(tiled-reader -Wextra) # implies -Wunused-parameter
    tiledreader_add_compile_flag(tiled-reader -Wmissing-field-initializers)
    #tiledreader_add_compile_flag(tiled-reader -Wswitch-default) # Currently ignored.
    tiledreader_add_compile_flag(tiled-reader -Wconversion) # GLM causes many warnings with this
    tiledreader_add_compile_flag(tiled-reader -Wzero-as-null-pointer-constant)
    tiledreader_add_compile_flag(tiled-reader -Wtype-limits)
    tiledreader_add_compile_flag(tiled-reader -Wsign-compare)
    tiledreader_add_compile_flag(tiled-reader -Wignored-qualifiers)
    tiledreader_add_compile_flag(tiled-reader -Wuninitialized
    tiledreader_add_compile_flag(tiled-reader -Winit-self)
    )
endif()

if (MSVC)
    target_compile_definitions(tiled-reader PRIVATE
        /D_CRT_SECURE_NO_WARNINGS
    )
    tiledreader_add_compile_flag(tiled-reader /W4)
    tiledreader_add_compile_flag(tiled-reader /wd4100)
    tiledreader_add_compile_flag(tiled-reader /wd4127)
    tiledreader_add_compile_flag(tiled-reader /wd4201)
    tiledreader_add_compile_flag(tiled-reader /wd4211)
    tiledreader_add_compile_flag(tiled-reader /wd4250)
    tiledreader_add_compile_flag(tiled-reader /wd4706)
    tiledreader_add_compile_flag(tiled-reader /MP)
endif ()

# Add tests
add_subdirectory(test)
