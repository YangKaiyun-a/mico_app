include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_WITH_CHANNEL_ROS2 STREQUAL "ON")
    message("Manual USE ROS2 channel")
    add_subdirectory(ros2)
endif()


find_package(Boost REQUIRED COMPONENTS filesystem)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_SOURCE_DIR} manager/*.h manager/*.hpp)
file(GLOB_RECURSE SOURCE_FILE CONFIGURE_DEPENDS  ${CMAKE_SOURCE_DIR} manager/*.cpp manager/*.cc)

add_library(Channel STATIC
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Channel
    PRIVATE Qt5::Core
    PUBLIC
        Boost::filesystem
        pthread
        dl
        Basic
        Config
        Logger
)

# 让使用者可以直接 include
target_include_directories(Channel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    manager
)

set_target_properties(Channel PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
