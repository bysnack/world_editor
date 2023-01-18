add_library(camera_init STATIC
    include/camera/init.hpp
    src/init.cpp
)

target_include_directories(camera_init PUBLIC include)

target_link_libraries(camera_init
PUBLIC
    level_init
    ecs
    camera
    CONAN_PKG::sfml
)
