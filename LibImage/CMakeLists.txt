cmake_minimum_required(VERSION 3.18)

project(LibImage ${LANG_LIST})

set(LIBIMAGE_SRC_PATH "${LibImage_SOURCE_DIR}")

file(GLOB_RECURSE LIB_SRCS
    "${LIBIMAGE_SRC_PATH}/src/*.c"
    "${LIBIMAGE_SRC_PATH}/inc/*.h"
)

add_library (LibImage SHARED ${LIB_SRCS})

target_include_directories(LibImage
    PUBLIC
        ${LIBIMAGE_SRC_PATH}/inc
)