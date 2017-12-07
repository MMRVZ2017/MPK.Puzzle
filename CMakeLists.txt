cmake_minimum_required(VERSION 3.8)
project(MPK.Puzzle)

set(CMAKE_CXX_STANDARD 11)
#replace with: C:\\opencv\\build
set(OpenCV_DIR "C:\\UOCV\\build")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES Source/main.cpp)
add_executable(MPK.Puzzle ${SOURCE_FILES})

target_link_libraries( MPK.Puzzle ${OpenCV_LIBS} )




