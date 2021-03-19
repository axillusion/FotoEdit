cmake_minimum_required(VERSION 3.18)

project(App ${LANG_LIST})

file(GLOB_RECURSE APP_SRCS
    "*.c"
    "*.h"
)

add_executable( App ${APP_SRCS} )
target_link_libraries(App PRIVATE libImage)

message ( ${CMAKE_SOURCE_DIR} )