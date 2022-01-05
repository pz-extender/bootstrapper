cmake_minimum_required(VERSION 3.21)
project(bootstrap_installer)

set(CMAKE_CXX_STANDARD 20)
set(BOOTSTRAP_SOURCES "main.cpp")

if(WIN32)
    list(APPEND BOOTSTRAP_SOURCES "steam_install_dir_win32.cpp")
endif()

add_executable(bootstrap_installer "${BOOTSTRAP_SOURCES}")

set_property(TARGET bootstrap_installer PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(bootstrap_installer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/vdf_parser")