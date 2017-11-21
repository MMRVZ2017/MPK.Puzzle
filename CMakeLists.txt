cmake_minimum_required(VERSION 3.8)
project(MPK_Puzzleloeser_Gruppe3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp Source/Input/Part.cpp Source/Creator/PuzzleCreator.cpp Source/Solver/PuzzleSolverGrp3.cpp Source/Output/SolutionElement.cpp Source/Helper/HelperFunctions.cpp)
add_executable(MPK_Puzzleloeser_Gruppe3 ${SOURCE_FILES})