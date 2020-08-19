cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(Tello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add ffmpeg dependency
add_subdirectory(libs/ffmpeg)
include_directories(${FFMPEG_HEADERS})

# Add SFML IMGUI
#add_subdirectory(libs)

# Add ImTerm
#include_directories(libs/ImTerm/include)

# Add Boost
#add_subdirectory(libs/boost-cmake)

add_executable(Tello main.cpp
    #Tello_API.cpp Tello_API.h
    #Tello_UDP.cpp Tello_UDP.h
    Tello_Video.cpp Tello_Video.h
    #Tello_UI.cpp Tello_UI.h
    #Tello_Terminal.cpp Tello_Terminal.h
    )

set_property( TARGET Tello PROPERTY
             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Link dependecies to executable
target_link_libraries(Tello PUBLIC ${FFMPEG_LIBRARIES}# ImGui-SFML::ImGui-SFML Boost::boost
    )
