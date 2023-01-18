add_library(events_init STATIC
    include/events/init.hpp
    src/init.cpp
)

target_include_directories(events_init PUBLIC include)

target_link_libraries(events_init
PUBLIC
    camera
    ecs
    events
)
