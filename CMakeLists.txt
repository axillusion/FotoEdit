cmake_minimum_required(VERSION 3.18)

# Set the root directory
set(FOTO_EDIT_DIR ${CMAKE_CURRENT_LIST_DIR})

project(FotoEdit C)

add_subdirectory(LibImage)
add_subdirectory(App)

find_package(Doxygen )
if (DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set(DOXYGEN_ALWAYS_DETAILED_SEC YES)
  set(DOXYGEN_JAVADOC_BANNER YES)
  set(DOXYGEN_JAVADOC_BLOCK YES)
  set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_SOURCE_DIR}/LibImage/src" "${CMAKE_SOURCE_DIR}/App/src" )
  doxygen_add_docs(docs
    ${CMAKE_SOURCE_DIR}
  )
endif(DOXYGEN_FOUND)