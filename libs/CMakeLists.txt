include(FetchContent)

option(USE_SYSTEM_DEPS CACHE OFF)
option(LINK_DEPS_STATIC CACHE ON)

# Don't build shared libs if we want to link to deps statically
if(LINK_DEPS_STATIC)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

message(STATUS "Fetching third party libraries")
#======================================

set(SFML_VERSION 2.5.1)
set(IMGUI_VERSION 1.68)
set(IMGUI_SFML_VERSION 2.0)
set(UVW_VERSION "v2.6.0_libuv-v1.38")
set(TINY_FILE_DIALOGS_VERSION 2.9.3)

FetchContent_Declare(
	TINY_FILE_DIALOGS
	URL "https://github.com/native-toolkit/tinyfiledialogs/archive/${TINY_FILE_DIALOGS_VERSION}.zip"
)

FetchContent_Declare(
  UVW
  GIT_REPOSITORY https://github.com/skypjack/uvw.git
)

FetchContent_Declare(
  SFML
  URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)

FetchContent_Declare(
  imgui
  URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
  GIT_TAG        develop
  # GIT_COMMIT 5f54b69b6aee18db846c81633f86f78c2586dded
  # ^ or like this - sometimes it's better because FetchContent won't look
  # into remote to see if branch head was updated or not - good for stable
  # tags like 'vX.X' corresponding to releases
)

add_subdirectory(uvw)
add_subdirectory(sfml)
add_subdirectory(imgui)
add_subdirectory(imgui-sfml)
add_subdirectory(tinyfiledialogs)

#======================================
message(STATUS "Fetching thirdparty libraries done")
