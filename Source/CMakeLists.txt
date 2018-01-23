cmake_minimum_required(VERSION 3.8)
project(PuzzleSolver)
set(OpenCV_Found 1)
set (OpenCV_DIR C:/openCV/build)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        main.cpp
        header.h
        functions/solve/structure.cpp
        functions/AbstractionLayers/AbstraktionLayer_Base.h
        functions/AbstractionLayers/Layer1/AbstractionLayer_1.cpp
        functions/AbstractionLayers/LayerHistogram/AbstractionLayer_Histogram.cpp
        functions/AbstractionLayers/LayerMeanDifference/AbstractionLayer_MeanDifference.cpp
        functions/AbstractionLayers/DestructionPower/DestructionPower.cpp
        header/solve.h
        header/input.h
        functions/solve/puzzleExtension.cpp)
add_executable(PuzzleSolver ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(PuzzleSolver ${OpenCV_LIBS})