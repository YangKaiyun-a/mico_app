add_library(Basic INTERFACE)

target_link_libraries(Basic INTERFACE
    Eigen3::Eigen
    yaml-cpp::yaml-cpp
    SDL
    SDL_image
    Logger
)

# 让使用者可以直接 include
target_include_directories(Basic INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    algorithm
    map
    point
)
