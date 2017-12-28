cmake_minimum_required(VERSION 3.8)
project(MPK.Puzzle)

set(CMAKE_CXX_STANDARD 11)

set(OpenCV_DIR "C:\\UOCV\\build")
# ^^ replace with: C:\\opencv\\build


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}  )
set(SOURCE_FILES Source/cornerFindier.cpp Source/header/preproc_base.h Source/functions/preproc_base.cpp )
# ^^ replace with:
# set(SOURCE_FILES Source/straighten.cpp Source/header/preproc_base.h Source/functions/preproc_base.cpp)
# to use the preprocessing algorithm.
add_executable(MPK.Puzzle ${SOURCE_FILES})

target_link_libraries( MPK.Puzzle ${OpenCV_LIBS} )




