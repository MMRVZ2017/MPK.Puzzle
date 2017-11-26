cmake_minimum_required(VERSION 3.8)
project(2017_11_17_PuzzleSolver)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp position.h constrMatrix.h constrMatrix.cpp puzzleBox.h puzzleBox.cpp step.h step.cpp path.h path.cpp defines.h test_puzzle_long40x40.h puzzleGenerator.cpp puzzleGenerator.hpp)
add_executable(2017_11_17_PuzzleSolver ${SOURCE_FILES})