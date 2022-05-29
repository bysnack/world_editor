add_library(core STATIC
    include/core/core.hpp
)

target_include_directories(core PUBLIC include)

target_link_libraries(core di)
