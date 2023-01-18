add_library(events INTERFACE
    include/events/handler.hpp
)

target_include_directories(events INTERFACE include)

target_link_libraries(events
INTERFACE
    ecs
    CONAN_PKG::sfml
)
