cmake_minimum_required(VERSION 3.19.3)

project(FotoEdit ${LANG_LIST})

set(APP_SRC_PATH "${FotoEdit_SOURCE_DIR}")
set(APP_BIN_PATH "${APP_SRC_PATH}/bin")

file(GLOB_RECURSE APP_SRCS
    "${APP_SRC_PATH}/*.c"
    "${APP_SRC_PATH}/*.h"
)

file ( GLOB_RECURSE APP_INC 
		"..../*h"
	)

add_subdirectory(LibColor)
target_link_libraries(FotoEdit PUBLIC LibColor)
target_include_directories(FotoEdit PUBLIC 
							$(APP_INC)
)	

add_executable( "${APP_BIN_PATH}/App" $(APP_SRCS))