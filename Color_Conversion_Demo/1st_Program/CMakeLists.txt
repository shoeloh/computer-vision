# Add executable called "1st_Program" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( 1st_Program )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( 1st_Program 1st_program.cpp color_conversions.cpp )
target_link_libraries( 1st_Program ${OpenCV_LIBS} )
