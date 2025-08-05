include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_libraries(Basic Config Logger)

if( BUILD_WITH_CHANNEL_AUTO STREQUAL "ON")
    message("BUILD_WITH_CHANNEL_AUTO")
    if(DEFINED ENV{ROS_VERSION} AND "$ENV{ROS_VERSION}" STREQUAL "1")
        message("Auto USE ROS1 channel")
        add_subdirectory(ros1)
    elseif(DEFINED ENV{ROS_VERSION} AND "$ENV{ROS_VERSION}" STREQUAL "2")
        message("Auto USE ROS2 channel")
        add_subdirectory(ros2)
    else()
        message("Auto Use protobuf channel")
        add_subdirectory(protobuf)
    endif()
elseif(BUILD_WITH_CHANNEL_ROS2 STREQUAL "ON")
    message("Manual USE ROS2 channel")
    add_subdirectory(ros2)
elseif(BUILD_WITH_CHANNEL_ROS1 STREQUAL "ON")
    message("Manual USE ROS1 channel")
    add_subdirectory(ros1)
elseif(BUILD_WITH_CHANNEL_PROTOBUF STREQUAL "ON")
    message("Manual USE protobuf channel")
    add_subdirectory(protobuf)
endif()

# manager
find_package(Boost REQUIRED COMPONENTS filesystem)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_SOURCE_DIR}  manager/*.h manager/*.hpp)
file(GLOB_RECURSE SOURCE_FILE CONFIGURE_DEPENDS  ${CMAKE_SOURCE_DIR}  manager/*.cpp manager/*.cc)

add_library(Channel STATIC
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Channel PUBLIC
    Boost::filesystem
    pthread
    dl
    Config
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
