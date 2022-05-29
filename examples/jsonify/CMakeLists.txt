add_executable(jsonify src/main.cpp)

target_include_directories(jsonify PUBLIC include)

target_link_libraries(jsonify CONAN_PKG::nlohmann_json sfml-graphics)
