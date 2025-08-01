cmake_minimum_required(VERSION 3.14)

project(Controller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)

add_library(Controller SHARED
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Controller
    Qt5::Core
    Basic
    Config
)

target_include_directories(Controller PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


