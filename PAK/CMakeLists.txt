set(PAKTOOL_SRC pak.cpp)

add_executable(PAKTOOL ${PAKTOOL_SRC})
target_compile_features(PAKTOOL PUBLIC cxx_std_17)
target_link_libraries(PAKTOOL stdc++fs PAK)