cmake_minimum_required(VERSION 3.2)
project (AOC++)
#version number
set (AOC++_VERSION_MAJOR 1)
set (AOC++_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/SFML/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/tgui-0.7/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(${SFML_INCLUDE_DIR}
					${CMAKE_SOURCE_DIR}
					third_party/GSL/include)

find_package(SFML REQUIRED COMPONENTS graphics audio system window)
find_package(TGUI REQUIRED)

add_subdirectory(engine/tools tools)

add_executable(test main.cpp)

target_link_libraries(test tgui sfml-graphics sfml-audio sfml-system sfml-window)