# Add executable called "Detect_Fingers" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( Detect_Fingers )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Detect_Fingers DetectFingers.cpp )
target_link_libraries( Detect_Fingers ${OpenCV_LIBS} )
