cmake_minimum_required(VERSION 3.14)

project(Task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc )
file(GLOB_RECURSE UI_FILE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}  *.ui)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}  *.h *.hpp)

add_library(Task STATIC
    ${SOURCE_FILE}
    ${UI_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Task
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
    PRIVATE Qt5::Sql
    Basic
    Config
    CustomControls
    Controller
)

# 让使用者可以直接 include
target_include_directories(Task PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
