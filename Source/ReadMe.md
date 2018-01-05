# Preprocessing and display algorithms

### The headers and functions are stored at /header/preproce_base.h and /functions/preproc_base.cpp

Usage: 
See instructions in CMakeLists.txt as well

## Straighten.cpp


Use straighten.cpp to process all of the raw images and write out segmented (background separated) and straightened images in Black&white (.jpg)
or BGR+Alpha (.PNG) format.
## Display.cpp


Use Display.cpp to visualize the solution to the puzzle. Call Mat solvedImage = resultImage(solveMx) function.
 Then imshow("solution",solvedImage); waitKey(0); and done!
 
The images used for the display should be in ../images/input/input_display/ with filename convention: 0.jpg, 1.jpg, 2.jpg, ... ,1007.jpg
They can be black&white or color as well.

Please beware solutionMx.columns * solutionMx.rows cannot be greater than 1008 
