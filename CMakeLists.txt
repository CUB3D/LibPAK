cmake_minimum_required(VERSION 3.4)
project(LibPAK)

set(CMAKE_C_STANDARD 11)

option(PAK_BUILD_TEST "Build tests" ON)
option(PAK_BUILD_TOOLS "Build tools" ON)

add_library(PAK SHARED src/LibPAK.c)
target_include_directories(PAK PUBLIC include)

if(PAK_BUILD_TEST)
    add_executable(Test test/Test.c)
    target_link_libraries(Test PAK)
    target_include_directories(Test PUBLIC include)
endif(PAK_BUILD_TEST)

if(PAK_BUILD_TOOLS)
    add_subdirectory(PAK)
endif(PAK_BUILD_TOOLS)