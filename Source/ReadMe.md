# Preprocessing and display algorithms



# The headers and functions are stored at /header/preproce_base.h and /functions/preproc_base.cpp

Use straighten.cpp to process all of the raw data and write out segmented (background separated) images in Black&white (.jpg)
or BGR+Alpha (.PNG) format.

Use Display.cpp to display the results of the SolutionMatrix by calling resultImage(solveMx) function. This function will return a Mat
 image of the completed puzzle.
 
The images used for the display should be in images/input/input_display/ with filename convention: 1.jpg, 2.jpg, ... ,1007.jpg

Please beware that solution matrices that contain more than 1008 elements cannot be displayed!
