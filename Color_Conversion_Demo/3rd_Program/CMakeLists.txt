# Add executable called "3rd_Program" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( 3rd_Program )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( 3rd_Program 3rd_program.cpp color_conversions.cpp )
target_link_libraries( 3rd_Program ${OpenCV_LIBS} )
