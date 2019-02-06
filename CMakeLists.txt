cmake_minimum_required(VERSION 3.4)
project(LibPAK)

set(CMAKE_C_STANDARD 11)

set(PAK_SRC src/LibPAK.c)


add_library(PAK SHARED ${PAK_SRC})
target_include_directories(PAK PUBLIC include)

add_executable(Test test/Test.c)
target_link_libraries(Test PAK)
target_include_directories(Test PUBLIC include)