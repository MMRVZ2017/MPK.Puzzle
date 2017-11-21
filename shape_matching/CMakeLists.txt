cmake_minimum_required(VERSION 3.8)
project(shape_matching)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(shape_matching ${SOURCE_FILES})


include_directories(${OpenCV_INCLUDE-DIRS})
target_link_libraries(shape_matching ${OpenCV_LIBS})