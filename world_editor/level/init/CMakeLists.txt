add_library(level_init STATIC
    include/level/init.hpp
    src/init.cpp
)

target_include_directories(level_init PUBLIC include)

target_link_libraries(level_init
PUBLIC
    level
    ecs
)
