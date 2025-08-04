cmake_minimum_required(VERSION 3.14)

project(AccountManagement LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc)
file(GLOB_RECURSE INCLUDE_FILE CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB_RECURSE UI_FILE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}  *.ui)
file(GLOB_RECURSE QRC_FILE  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}  resource/*.qrc)

add_library(AccountManagement STATIC
    ${SOURCE_FILE}
    ${INCLUDE_FILE}
    ${UI_FILE}
    ${QRC_FILE}
)

target_link_libraries(AccountManagement
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
)

target_include_directories(AccountManagement PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


