cmake_minimum_required(VERSION 3.12)
project(LibPAK)

set(CMAKE_C_STANDARD 11)

add_library(PAK SHARED src/LibPAK.c include/LibPAK.h include/Definitions.h)

add_executable(Test test/Test.c)
target_link_libraries(Test PAK)
target_include_directories(Test PUBLIC include)