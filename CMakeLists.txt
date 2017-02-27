# this version of cmake was selected arbitrarily. if you find that this
# version number needs to change for some reason, please let me know.
cmake_minimum_required(VERSION 2.8.7)
project(fizzbuzz_swiftnav)

# Strictifying compiler options
set(CMAKE_C_FLAGS "-Wall -Werror ${CMAKE_C_FLAGS}")

# these may be used before long. as of now, things are very simple.
#add_subdirectory(src)
#add_subdirectory(test)

add_executable(fizzbuzz fizzbuzz.c)    
target_link_libraries(fizzbuzz gmp)    

