cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)

project(CoreNetworkEngine)

find_package(fmt)


add_executable(MyExe main.cpp)

target_link_libraries(MyExe  fmt::fmt)