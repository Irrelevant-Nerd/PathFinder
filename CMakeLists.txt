cmake_minimum_required(VERSION 3.16)

project(Path_Finder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Path_Finder ${SRC_FILES})

target_include_directories(Path_Finder PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(Path_Finder PRIVATE -Wall -Wextra)

target_link_libraries(Path_Finder PRIVATE SFML::Graphics)
