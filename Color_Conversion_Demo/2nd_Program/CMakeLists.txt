# Add executable called "2nd_Program" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( 2nd_Program )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( 2nd_Program 2nd_program.cpp color_conversions.cpp )
target_link_libraries( 2nd_Program ${OpenCV_LIBS} )
