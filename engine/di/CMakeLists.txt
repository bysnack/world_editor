add_library(di STATIC
    include/di/entity.hpp
    include/di/injector.hpp
    include/di/registry.hpp
    include/di/view.hpp
    src/injector.cpp
    src/registry.cpp
)

target_include_directories(di PUBLIC include)

target_link_libraries(di
PUBLIC
    meta
)
