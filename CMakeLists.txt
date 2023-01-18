cmake_minimum_required(VERSION 3.2)
project (argentum)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES sfml/2.5.1 nlohmann_json/3.10.5
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
   GENERATORS cmake_find_package cmake_paths
   OPTIONS qt:qtdeclarative=True qt:shared=True qt:qtshadertools=True)


include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

#version number
set (argentum_VERSION_MAJOR 1)
set (argentum_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(world_editor)
add_subdirectory(support)

