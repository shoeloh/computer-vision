# Add executable called "Detect_Wink" that is built from the source files
# The extensions are automatically found.
cmake_minimum_required( VERSION 2.8 )
Project( Detect_Wink )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Detect_Wink DetectWink.cpp )
target_link_libraries( Detect_Wink ${OpenCV_LIBS} )
