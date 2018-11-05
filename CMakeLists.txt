cmake_minimum_required(VERSION 3.4)
project(LibPAK)

set(CMAKE_C_STANDARD 11)

option(PAK_BUILD_TEST "Build tests" ON)
add_library(PAK SHARED src/LibPAK.c include/LibPAK.h include/Definitions.h)

if(PAK_BUILD_TEST)
    add_executable(Test test/Test.c)
    target_link_libraries(Test PAK)
    target_include_directories(Test PUBLIC include)
endif(PAK_BUILD_TEST)