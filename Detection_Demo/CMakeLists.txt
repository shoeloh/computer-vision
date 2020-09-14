# CMakeLists files in this project 
cmake_minimum_required (VERSION 2.8)
project (PROJECT_2)

# Recurse into the subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (DetectFingers)
add_subdirectory (DetectWink)
