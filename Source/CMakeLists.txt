cmake_minimum_required(VERSION 3.8)
project(PuzzleSolver)
set(OpenCV_Found 1)
set (OpenCV_DIR C:/openCV/build)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        main.cpp
        header.h
        functions/solve/classExtension.cpp
        functions/solve/structure.cpp
        functions/AbstractionLayers/AbstraktionLayer_Base.h
        )
add_executable(PuzzleSolver ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(PuzzleSolver ${OpenCV_LIBS})