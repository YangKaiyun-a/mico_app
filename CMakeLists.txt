cmake_minimum_required(VERSION 3.19)
project(ros2_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 cmake 包
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=return-type")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
endif()

# 把当前 CMakeLists.txt 所在目录加入到编译器的 include 搜索路径
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# 添加 ROS2 库
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# 添加 QT5 库
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Svg)
# 添加第三方库
include(GetJsonStruct)
include(GetDockWidget)
include(GetYamlCpp)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

# 编译选项，默认 ROS2
option(BUILD_WITH_CHANNEL_AUTO OFF)
option(BUILD_WITH_CHANNEL_ROS1 OFF)
option(BUILD_WITH_CHANNEL_ROS2 OFF)
option(BUILD_WITH_CHANNEL_PROTOBUF OFF)
option(BUILD_WITH_TEST OFF)
set(BUILD_WITH_CHANNEL_ROS2 ON)



# 不添加此条的话，xxxx.cpp 中 #include "ui_xxxx.h"会报错
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
file(GLOB PRO_FORM_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB PRO_RESOURCES_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resource/*.qrc)
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h )
file(GLOB_RECURSE PRO_SOURCES_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)



add_subdirectory(CustomControls)
add_subdirectory(Config)
add_subdirectory(Basic)
add_subdirectory(Logger)
add_subdirectory(Channel)


# main 节点
add_executable(${PROJECT_NAME}
      ${PRO_FORM_DIR}
      ${PRO_RESOURCES_DIR}
      ${PRO_INCLUDE_DIR}
      ${PRO_SOURCES_DIR}
)

# 搜索头文件的位置
target_include_directories(${PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include
)

ament_target_dependencies(${PROJECT_NAME} rclcpp std_msgs)

# 链接库
target_link_libraries(${PROJECT_NAME}
      Qt5::Core
      Qt5::Gui
      Qt5::Widgets
      Qt5::Concurrent
      Qt5::Svg
      Eigen3::Eigen
      dockwidget::dockwidget
      json_struct
      ${OpenCV_LIBS}
      CustomControls
      Config
      Basic
      Logger
      Channel
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})


ament_package()
