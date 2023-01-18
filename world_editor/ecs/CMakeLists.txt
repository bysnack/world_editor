add_library(ecs STATIC
    include/ecs/entity.hpp
    include/ecs/injector.hpp
    include/ecs/registry.hpp
    include/ecs/view.hpp
    include/ecs/type_traits.hpp
    src/injector.cpp
    src/registry.cpp
)

target_include_directories(ecs PUBLIC include)
