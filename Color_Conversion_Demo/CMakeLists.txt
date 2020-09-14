# CMakeLists files in this project 
cmake_minimum_required (VERSION 2.8)
project (PROJECT_1)

# Recurse into the subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (1st_Program)
add_subdirectory (2nd_Program)
add_subdirectory (3rd_Program)
add_subdirectory (4th_Program)
