add_library(engine STATIC
    include/engine/engine.hpp
)

target_include_directories(engine PUBLIC include)

target_link_libraries(engine ecs)
