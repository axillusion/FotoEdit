cmake_minimum_required(VERSION 3.18)

project(FotoEdit ${LANG_LIST})

set(APP_SRC_PATH "${FotoEdit_SOURCE_DIR}/App")

file(GLOB_RECURSE APP_SRCS
    "${APP_SRC_PATH}/*.c"
    "${APP_SRC_PATH}/*.h"
)

add_executable( App ${APP_SRCS} )

add_subdirectory(LibImage)
target_link_libraries(App PUBLIC LibImage)
target_include_directories(App PUBLIC "home/calin/FotoEdit/LibImage/inc" )

find_package(Doxygen OPTIONAL_COMPONENTS dot)
if (DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doc
    ${CMAKE_SOURCE_DIR}
  )
endif(DOXYGEN_FOUND)