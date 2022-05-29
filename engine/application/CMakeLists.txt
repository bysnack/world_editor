find_package(SFML REQUIRED system window graphics network audio PATHS ${CMAKE_SOURCE_DIR}/thirdparty)
find_package(TGUI REQUIRED)

add_executable(main src/application.cpp src/main.cpp)

target_include_directories(main PUBLIC include)

target_link_libraries(main PUBLIC tgui sfml-graphics core level graphics)
