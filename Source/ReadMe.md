# Preprocessing and display algorithms

## The headers and functions are stored at /header/preproce_base.h and /functions/preproc_base.cpp

Usage: 
See instructions in CMakeLists.txt as well

Use straighten.cpp to process all of the raw images and write out segmented (background separated) and straightened images in Black&white (.jpg)
or BGR+Alpha (.PNG) format.

Use Display.cpp to display the results of the SolutionMatrix by calling resultImage(solveMx) function. This function will return a Mat
image of the completed puzzle.
 
The images used for the display should be in images/input/input_display/ with filename convention: 0.jpg, 1.jpg, 2.jpg, ... ,1007.jpg

Please beware that solution can only be displayed if there are enough images in the input_display directory. 
