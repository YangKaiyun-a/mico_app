
file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)

add_library(Basic INTERFACE
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Basic INTERFACE
    Eigen3::Eigen
    yaml-cpp::yaml-cpp
    SDL
    SDL_image
    Config
)

# 让使用者可以直接 include
target_include_directories(Basic INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    algorithm
    map
    point
)
