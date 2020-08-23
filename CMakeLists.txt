cmake_minimum_required(VERSION 3.15)

project(Tello LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs)

add_subdirectory(libs/imterm)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
	libavdevice
	libavfilter
	libavformat
	libavcodec
	libswresample
	libswscale
	libavutil
)


add_executable(Tello main.cpp
	Tello_API.cpp Tello_API.h
	Tello_UDP.cpp Tello_UDP.h
	Tello_Video.cpp Tello_Video.h
	Tello_UI.cpp Tello_UI.h
	Tello_Terminal.cpp Tello_Terminal.h
	)

target_include_directories( Tello PRIVATE libs/imterm/include ${FFMPEG_INCLUDE_DIRS})
target_link_libraries( Tello PUBLIC ImGui-SFML::ImGui-SFML PkgConfig::LIBAV uv::uv-static uvw::uvw-static tinyfiledialogs)

# MSVC weirdness when linking staticly compiled libraries
set_property(TARGET Tello PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
