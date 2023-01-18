add_library(level STATIC
    include/level/data.hpp
    include/level/map.hpp
    include/level/tile_map.hpp
    src/tile_map.cpp
)

target_include_directories(level PUBLIC include)

target_link_libraries(level
PUBLIC
    ecs
    CONAN_PKG::sfml
    graphics
)
