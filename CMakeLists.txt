cmake_minimum_required(VERSION 3.19)
project(mico_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 添加 cmake 包
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=return-type")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
endif()

# 添加 QT5 库
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Concurrent Svg)
# 添加第三方库
include(GetJsonStruct)
include(GetDockWidget)
include(GetYamlCpp)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

# 编译选项
option(BUILD_WITH_CHANNEL_AUTO ON)
option(BUILD_WITH_CHANNEL_ROS1 OFF)
option(BUILD_WITH_CHANNEL_ROS2 OFF)
option(BUILD_WITH_CHANNEL_PROTOBUF OFF)
option(BUILD_WITH_TEST OFF)
set(BUILD_WITH_CHANNEL_AUTO ON)

# 不查找子目录
file(GLOB PRO_SOURCES_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

add_subdirectory(Controller)
add_subdirectory(CustomControls)
add_subdirectory(Config)
add_subdirectory(Basic)
add_subdirectory(Logger)
add_subdirectory(Channel)


add_executable(${PROJECT_NAME}
      ${PRO_SOURCES_DIR}
)

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
      Controller
      CustomControls
      Config
      Basic
      Logger
      Channel
)
