cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(Tello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# An awesome script to interact with vcpkg from cmake: https://github.com/REGoth-project/Automate-VCPKG
include(${CMAKE_SOURCE_DIR}/cmake/automate-vcpkg.cmake)

vcpkg_bootstrap()
vcpkg_install_packages(ffmpeg boost-asio imgui-sfml)

if(WIN32)
    # This is not really modern cmake, but it is the only way i know to link ffmpeg through cmake
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        file(GLOB STATIC_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/debug/lib/*.lib")
    else()
        file(GLOB STATIC_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/lib/*.lib")
    endif()

    set(INCLUDE_DIRS "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/include")
endif()

add_subdirectory(libs/imterm)

# You would think vcpkg would add these automaticly, perhabs im doing something wrong?
set(ImGui-SFML_DIR "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/share/imgui-sfml")
set(ImGui_DIR "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/share/imgui")
set(SFML_DIR "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/share/sfml")

find_package(SFML COMPONENTS system window graphics REQUIRED)
find_package(ImGui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)


add_executable(Tello main.cpp
    Tello_API.cpp Tello_API.h
    Tello_UDP.cpp Tello_UDP.h
    Tello_Video.cpp Tello_Video.h
    Tello_UI.cpp Tello_UI.h
    Tello_Terminal.cpp Tello_Terminal.h
    )

target_include_directories( Tello PRIVATE ${INCLUDE_DIRS} libs/imterm/include)
target_link_libraries( Tello PRIVATE ${STATIC_LIBRARIES}  ImGui-SFML::ImGui-SFML)

# MSVC weirdness when linking staticly compiled libraries
set_property(TARGET Tello PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
