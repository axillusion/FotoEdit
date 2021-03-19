cmake_minimum_required(VERSION 3.18)

project(LibImage C)

file(GLOB_RECURSE LIB_IMAGE_SRCS
    "*.c"
    "*.h"
)

add_library (libImage SHARED ${LIB_IMAGE_SRCS})
set_target_properties(libImage PROPERTIES PREFIX "")

target_include_directories(libImage
    PUBLIC
        inc
)


