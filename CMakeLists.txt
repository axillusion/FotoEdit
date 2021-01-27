cmake_minimum_required(VERSION 3.18)

project(FotoEdit ${LANG_LIST})

set(APP_SRC_PATH "${FotoEdit_SOURCE_DIR}/App")

file(GLOB_RECURSE APP_SRCS
    "${APP_SRC_PATH}/*.c"
    "${APP_SRC_PATH}/*.h"
)

file ( GLOB_RECURSE APP_INC 
		"${APP_SRC_PATH}/*h"
	)

add_executable( App ${APP_SRCS} )

add_subdirectory(LibImage)
target_link_libraries(App PUBLIC LibImage)
target_include_directories(App PUBLIC 
							${APP_INC}
)	