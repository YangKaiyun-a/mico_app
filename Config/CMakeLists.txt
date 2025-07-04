cmake_minimum_required(VERSION 3.14)

project(Config LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)

add_library(Config SHARED
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
)

target_link_libraries(Config
    ${Boost_LIBRARIES}
    Qt5::Core
    Basic
    json_struct::json_struct
    yaml-cpp::yaml-cpp
)

target_include_directories(Config PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


