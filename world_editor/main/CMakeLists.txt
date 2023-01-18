add_executable(main src/main.cpp)

target_include_directories(main PUBLIC include)

target_link_libraries(main PUBLIC
    CONAN_PKG::sfml
    engine
    graphics
    graphics_init
    level_init
    ecs
    camera_init
    events
    events_init
)
