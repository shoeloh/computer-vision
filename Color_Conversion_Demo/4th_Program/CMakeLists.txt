# Add executable called "4th_Program" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( 4th_Program )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( 4th_Program 4th_program.cpp color_conversions.cpp )
target_link_libraries( 4th_Program ${OpenCV_LIBS} )
