cmake_minimum_required(VERSION 3.4)
project(LibPAK)

set(CMAKE_C_STANDARD 11)

option(PAK_BUILD_TEST "Build tests" ON)
option(PAK_BUILD_TOOLS "Build tools" ON)
option(PAK_BUILD_STATIC "Build static lib" ON)

set(PAK_SRC src/LibPAK.c)

if(PAK_BUID_STATIC)
    add_library(PAK STATIC ${PAK_SRC})
else()
    add_library(PAK SHARED ${PAK_SRC})
endif()
target_include_directories(PAK PUBLIC include)

if(PAK_BUILD_TEST)
    add_executable(Test test/Test.c)
    target_link_libraries(Test PAK)
    target_include_directories(Test PUBLIC include)
endif(PAK_BUILD_TEST)

if(PAK_BUILD_TOOLS)
    add_subdirectory(PAK)
endif(PAK_BUILD_TOOLS)