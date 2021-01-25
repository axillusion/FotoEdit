cmake_minimum_required(VERSION 3.18)

project(FotoEdit ${LANG_LIST})

set(APP_SRC_PATH "${FotoEdit_SOURCE_DIR}/App")
set(APP_BIN_PATH "${APP_SRC_PATH}/bin")

file(GLOB_RECURSE APP_SRCS
    "${APP_SRC_PATH}/*.c"
    "${APP_SRC_PATH}/*.h"
)

file ( GLOB_RECURSE APP_INC 
		"${FotoEdit_SOURCE_DIR}/*h"
	)

add_subdirectory(LibImage)
target_link_libraries(FotoEdit PUBLIC LibImage)
target_include_directories(FotoEdit PUBLIC 
							${APP_INC}
)	

add_executable( App $(APP_SRCS))