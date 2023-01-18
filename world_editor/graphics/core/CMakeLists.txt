add_library(graphics STATIC
    include/graphics/atlas.hpp
    src/atlas.cpp
)

target_include_directories(graphics PUBLIC include)

target_link_libraries(graphics
PUBLIC
    CONAN_PKG::nlohmann_json
    CONAN_PKG::sfml
)
