# this version of cmake was selected arbitrarily. if you find that this
# version number needs to change for some reason, please let me know.
cmake_minimum_required(VERSION 2.8)
project(fizzbuzz_swiftnav)

# Strictifying compiler options
set(CMAKE_C_FLAGS "-g -Wall -Werror ${CMAKE_C_FLAGS}")

add_executable(fizzbuzz fizzbuzz.c)    
target_link_libraries(fizzbuzz gmp)    

