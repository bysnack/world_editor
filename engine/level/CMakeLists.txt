add_library(level STATIC
    include/level/tile_map.hpp
    src/tile_map.cpp
)

target_include_directories(level PUBLIC include)

target_link_libraries(level
PUBLIC
    di
)
