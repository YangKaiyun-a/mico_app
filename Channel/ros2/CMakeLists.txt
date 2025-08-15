set(TARGET_NAME channel_ros2)

file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_SOURCE_DIR}  *.h *.hpp)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_kdl REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

# 自定义消息接口
find_package(example_interfaces REQUIRED)


# 动态库会放入 lib/
add_library(${TARGET_NAME} SHARED
    ${SOURCE_FILE} 
    ${INCLUDE_FILE}
)
target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ament_target_dependencies(${TARGET_NAME}
    PUBLIC
        rclcpp
        Boost
        nav_msgs
        tf2
        tf2_geometry_msgs
        tf2_ros sensor_msgs
        tf2_kdl
        cv_bridge
        example_interfaces
)

target_link_libraries(${TARGET_NAME}
    PRIVATE Qt5::Core
    PUBLIC
        Basic
        Config
        Logger
)
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
install(TARGETS ${TARGET_NAME} DESTINATION lib/${PROJECT_NAME})
install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION bin/lib
    LIBRARY DESTINATION bin/lib
    ARCHIVE DESTINATION bin/lib
)
# ament_package()
# install(TARGETS ${TARGET_NAME} DESTINATION lib/${PROJECT_NAME})
