cmake_minimum_required(VERSION 3.14)

project(Map LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE UI_FILE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}  *.ui)
file(GLOB_RECURSE QRC_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}  resource/*.qrc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}  *.h *.hpp)

# 添加第三方库
find_package(OpenCV REQUIRED)

add_library(Map STATIC
    ${SOURCE_FILE}
    ${UI_FILE}
    ${QRC_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Map
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
    PRIVATE Qt5::Concurrent
    PRIVATE Qt5::Svg
    Eigen3::Eigen
    dockwidget::dockwidget
    json_struct
    ${OpenCV_LIBS}
    Basic
    Config
    Controller
    Channel
)

# 让使用者可以直接 include
target_include_directories(Map PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
