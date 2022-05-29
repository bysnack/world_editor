add_executable(sfml src/main.cpp)

target_include_directories(sfml PUBLIC include)

target_link_libraries(sfml CONAN_PKG::sfml core)
