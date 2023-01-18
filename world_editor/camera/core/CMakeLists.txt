add_library(camera STATIC
    include/camera/components.hpp
)

target_include_directories(camera PUBLIC include)

target_link_libraries(camera
PUBLIC
    ecs
    CONAN_PKG::sfml
    graphics
)
