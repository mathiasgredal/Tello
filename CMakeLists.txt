cmake_minimum_required(VERSION 3.15)

project(Tello LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(Tello main.cpp
	    Tello_API.cpp Tello_API.h
		Tello_UDP.cpp Tello_UDP.h
		Tello_Video.cpp Tello_Video.h
		Tello_UI.cpp Tello_UI.h
		Tello_Terminal.cpp Tello_Terminal.h
		Tello_Telemetry.cpp Tello_Telemetry.h
		)

if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/automate-vcpkg.cmake)
    vcpkg_bootstrap()
    vcpkg_install_packages(ffmpeg imgui-sfml uvw tinyfiledialogs freetype)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            file(GLOB FFMPEG_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/debug/lib/*.lib")
            set(FREETYPE_LIBRARY "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/debug/lib/freetyped.lib")
    else()
            file(GLOB FFMPEG_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/lib/*.lib")
            set(FREETYPE_LIBRARY "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/lib/freetype.lib")

    endif()

    set(INCLUDE_DIRS "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/include"  "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/include/tinyfiledialogs")
    set(FREETYPE_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/include")

    target_link_libraries(Tello PUBLIC ${FFMPEG_LIBRARIES})

    set(FIND_PREFIX "${CMAKE_BINARY_DIR}/vcpkg/installed/x64-windows-static/share/")

    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/sfml/")
    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/imgui/")
    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/imgui-sfml/")
    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/uvw/")
    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/unofficial-libuv/")
    list(APPEND CMAKE_PREFIX_PATH "${FIND_PREFIX}/tinyfiledialogs/")

    find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(ImGui-SFML CONFIG REQUIRED)
    find_package(tinyfiledialogs CONFIG REQUIRED)
    find_package(uvw CONFIG REQUIRED)
    find_package(Threads REQUIRED)

    target_link_libraries(Tello PRIVATE ImGui-SFML::ImGui-SFML tinyfiledialogs::tinyfiledialogs uvw::uvw)
    target_include_directories(Tello PUBLIC ${INCLUDE_DIRS})

    # MSVC weirdness when linking staticly compiled libraries
    set_property(TARGET Tello PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # An attempt to speed up the extremely slow compile times on windows
    #target_precompile_headers(Tello PRIVATE ${CMAKE_SOURCE_DIR}/Tello_Video.h)
    #add_compile_options("/debug:fastlink /MP /incremental /verbose")
else()
    add_subdirectory(libs)

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

    target_link_libraries( Tello PUBLIC PkgConfig::LIBAV ImGui-SFML::ImGui-SFML uv::uv-static uvw::uvw-static tinyfiledialogs)
endif()

add_subdirectory(libs/imterm)
include_directories(libs/stb)


target_include_directories( Tello PRIVATE ${LIBAV_INCLUDE_DIRS} libs/imterm/include )

