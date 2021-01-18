cmake_minimum_required(VERSION 3.19.3)

file ( LibColor_SRCS 
		"{LIBCOLOR_SRC_PATH}/*.c"
)

add_library ( Bin/LibColor SHARED $(LibColor_SRCS))