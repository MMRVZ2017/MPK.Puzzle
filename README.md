# MPK.Puzzle--Team6-Cntrl: Preprocessing and display algorithms

### The headers and functions are stored at /header/preproce_base.h and /functions/preproc_base.cpp

Usage: 
See instructions in CMakeLists.txt as well

## Straighten.cpp


Use straighten.cpp to process all of the raw images and write out segmented (background separated) and straightened images in Black&white (.jpg)
or BGR+Alpha (.PNG) format.
## Display.cpp


Use Display.cpp to display the results of the SolutionMatrix by calling resultImage(solveMx) function. This function will return a Mat
image of the completed puzzle.
 
The images used for the display should be in ../images/input/input_display/ with filename convention: 0.jpg, 1.jpg, 2.jpg, ... ,1007.jpg

Please beware solutionMx.columns * solutionMx.rows cannot be greater than 1008 


