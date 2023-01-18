add_library(graphics_init STATIC
    include/graphics/init.hpp
    src/init.cpp
)

target_include_directories(graphics_init PUBLIC include)

target_link_libraries(graphics_init
PUBLIC
    ecs
    graphics
    camera
)
